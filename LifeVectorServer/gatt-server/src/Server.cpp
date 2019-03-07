// Copyright 2017 Paul Nettle.
//
// This file is part of Gobbledegook.
//
// Gobbledegook is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Gobbledegook is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Gobbledegook.  If not, see <http://www.gnu.org/licenses/>.

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// >>
// >>>  INSIDE THIS FILE
// >>
//
// This is the money file. This is your server description and complete implementation. If you want to add or remove a Bluetooth
// service, alter its behavior, add or remove characteristics or descriptors (and more), then this is your new home.
//
// >>
// >>>  DISCUSSION
// >>
//
// The use of the term 'server', as it is used here, refers a collection of BlueZ services, characteristics & Descripors (plus
// a little more.)
//
// Our server needs to be described in two ways. Why two? Well, think about it like this: We're communicating with Bluetooth
// clients through BlueZ, and we're communicating with BlueZ through D-Bus. In essence, BlueZ and D-Bus are acting as tunnels, one
// inside the other.
//
// Here are those two descriptions in a bit more detail:
//
// 1. We need to describe ourselves as a citizen on D-Bus: The objects we implement, interfaces we provide, methods we handle, etc.
//
//    To accomplish this, we need to build an XML description (called an 'Introspection' for the curious readers) of our DBus
//    object hierarchy. The code for the XML generation starts in DBusObject.cpp (see `generateIntrospectionXML`) and carries on
//    throughout the other DBus* files (and even a few Gatt* files).
//
// 2. We also need to describe ourselves as a Bluetooth citizen: The services we provide, our characteristics and descriptors.
//
//    To accomplish this, BlueZ requires us to implement a standard D-Bus interface ('org.freedesktop.DBus.ObjectManager'). This
//    interface includes a D-Bus method 'GetManagedObjects', which is just a standardized way for somebody (say... BlueZ) to ask a
//    D-Bus entity (say... this server) to enumerate itself. This is how BlueZ figures out what services we offer. BlueZ will
//    essentially forward this information to Bluetooth clients.
//
// Although these two descriptions work at different levels, the two need to be kept in sync. In addition, we will also need to act
// on the messages we receive from our Bluetooth clients (through BlueZ, through D-Bus.) This means that we'll have yet another
// synchronization issue to resolve, which is to ensure that whatever has been asked of us, makes its way to the correct code in
// our description so we do the right thing.
//
// I don't know about you, but when dealing with data and the concepts "multiple" and "kept in sync" come into play, my spidey
// sense starts to tingle. The best way to ensure sychronization is to remove the need to keep things sychronized.
//
// The large code block below defines a description that includes all the information about our server in a way that can be easily
// used to generate both: (1) the D-Bus object hierarchy and (2) the BlueZ services that occupy that hierarchy. In addition, we'll
// take that a step further by including the implementation right inside the description. Everything in one place.
//
// >>
// >>>  MANAGING SERVER DATA
// >>
//
// The purpose of the server is to serve data. Your application is responsible for providing that data to the server via two data
// accessors (a getter and a setter) that implemented in the form of delegates that are passed into the `ggkStart()` method.
//
// While the server is running, if data is updated via a write operation from the client the setter delegate will be called. If your
// application also generates or updates data periodically, it can push those updates to the server via call to
// `ggkNofifyUpdatedCharacteristic()` or `ggkNofifyUpdatedDescriptor()`.
//
// >>
// >>>  UNDERSTANDING THE UNDERLYING FRAMEWORKS
// >>
//
// The server description below attempts to provide a GATT-based interface in terms of GATT services, characteristics and
// descriptors. Consider the following sample:
//
//     .gattServiceBegin("text", "00000001-1E3C-FAD4-74E2-97A033F1BFAA")
//         .gattCharacteristicBegin("string", "00000002-1E3C-FAD4-74E2-97A033F1BFAA", {"read", "write", "notify"})
//
//             .onReadValue(CHARACTERISTIC_METHOD_CALLBACK_LAMBDA
//             {
//                 // Abbreviated for simplicity
//                 self.methodReturnValue(pInvocation, myTextString, true);
//             })
//
//             .onWriteValue(CHARACTERISTIC_METHOD_CALLBACK_LAMBDA
//             {
//                 // Abbreviated for simplicity
//                 myTextString = ...
//             })
//
//             .gattDescriptorBegin("description", "2901", {"read"})
//                 .onReadValue(DESCRIPTOR_METHOD_CALLBACK_LAMBDA
//                 {
//                     self.methodReturnValue(pInvocation, "Returns a test string", true);
//                 })
//
//             .gattDescriptorEnd()
//         .gattCharacteristicEnd()
//     .gattServiceEnd()
//
// The first thing you may notice abpout the sample is that all of the lines begin with a dot. This is because we're chaining
// methods together. Each method returns the appropriate type to provide context. For example, The `gattServiceBegin` method returns
// a reference to a `GattService` object which provides the proper context to create a characteristic within that service.
// Similarly, the `gattCharacteristicBegin` method returns a reference to a `GattCharacteristic` object which provides the proper
// context for responding to requests to read the characterisic value or add descriptors to the characteristic.
//
// For every `*Begin` method, there is a corresponding `*End` method, which returns us to the previous context. Indentation helps us
// keep track of where we are.
//
// Also note the use of the lambda macros, `CHARACTERISTIC_METHOD_CALLBACK_LAMBDA` and `DESCRIPTOR_METHOD_CALLBACK_LAMBDA`. These
// macros simplify the process of including our implementation directly in the description.
//
// The first parameter to each of the `*Begin` methods is a path node name. As we build our hierarchy, we give each node a name,
// which gets appended to it's parent's node (which in turns gets appended to its parent's node, etc.) If our root path was
// "/com/gobbledegook", then our service would have the path "/com/gobbledegook/text" and the characteristic would have the path
// "/com/gobbledegook/text/string", and the descriptor would have the path "/com/gobbledegook/text/string/description". These paths
// are important as they act like an addressing mechanism similar to paths on a filesystem or in a URL.
//
// The second parameter to each of the `*Begin` methods is a UUID as defined by the Bluetooth standard. These UUIDs effectively
// refer to an interface. You will see two different kinds of UUIDs: a short UUID ("2901") and a long UUID
// ("00000002-1E3C-FAD4-74E2-97A033F1BFAA").
//
// For more information on UUDSs, see GattUuid.cpp.
//
// In the example above, our non-standard UUIDs ("00000001-1E3C-FAD4-74E2-97A033F1BFAA") are something we generate ourselves. In the
// case above, we have created a custom service that simply stores a mutable text string. When the client enumerates our services
// they'll see this UUID and, assuming we've documented our interface behind this UUID for client authors, they can use our service
// to read and write a text string maintained on our server.
//
// The third parameter (which only applies to dharacteristics and descriptors) are a set of flags. You will find the current set of
// flags for characteristics and descriptors in the "BlueZ D-Bus GATT API description" at:
//
//     https://git.kernel.org/pub/scm/bluetooth/bluez.git/plain/doc/gatt-api.txt
//
// In addition to these structural methods, there are a small handful of helper methods for performing common operations. These
// helper methods are available within a method (such as `onReadValue`) through the use of a `self` reference. The `self` reference
// refers to the object at which the method is invoked (either a `GattCharacteristic` object or a `GattDescriptor` object.)
//
//     methodReturnValue and methodReturnVariant
//         These methods provide a means for returning values from Characteristics and Descriptors. The `-Value` form accept a set
//         of common types (int, string, etc.) If you need to provide a custom return type, you can do so by building your own
//         GVariant (which is a GLib construct) and using the `-Variant` form of the method.
//
//     sendChangeNotificationValue and sendChangeNotificationVariant
//         These methods provide a means for notifying changes for Characteristics. The `-Value` form accept a set of common types
//         (int, string, etc.) If you need to notify a custom return type, you can do so by building your own GVariant (which is a
//         GLib construct) and using the `-Variant` form of the method.
//
// For information about GVariants (what they are and how to work with them), see the GLib documentation at:
//
//     https://www.freedesktop.org/software/gstreamer-sdk/data/docs/latest/glib/glib-GVariantType.html
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/*
 * @filename Server.cpp
 * @author Samar Sajnani (ssajnani@uwo.ca)
 * @brief Server endpoints file
 *
 */
#include <algorithm>

#include "Server.h"
#include "ServerUtils.h"
#include "Utils.h"
#include "Globals.h"
#include "DBusObject.h"
#include "DBusInterface.h"
#include "GattProperty.h"
#include "GattService.h"
#include "GattUuid.h"
#include "GattCharacteristic.h"
#include "GattDescriptor.h"
#include "Logger.h"

namespace ggk {

// There's a good chance there will be a bunch of unused parameters from the lambda macros
#if defined(__GNUC__) && defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
#endif
#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

// Global variables needed to hold session data
std::map< std::string, std::map<std::string, std::string> > bufferStorage;
EncryptionModule em;



// ---------------------------------------------------------------------------------------------------------------------------------
// Globals
// ---------------------------------------------------------------------------------------------------------------------------------

// Our one and only server. It's global.
    std::shared_ptr <Server> TheServer = nullptr;
//Used to remove the [START] and [END] substrings from received messages
/*
 * @brief Utility function to remove bluetooth frame delimiters 
 * @param string to remove the delimiters from 
 * @return string without delimiters
 */
std::string removeStartAndEnd(std::string start_end_string){
    string startDEL = "[START]";
    string endDEL = "[END]";
    unsigned first = start_end_string.find(startDEL);
    unsigned last = start_end_string.find(endDEL);
    string pub_key_str = start_end_string.substr(first+startDEL.size(),last-endDEL.size()-2);
    return pub_key_str;
}

// ---------------------------------------------------------------------------------------------------------------------------------
// Object implementation
// ---------------------------------------------------------------------------------------------------------------------------------

// Our constructor builds our entire server description
//
// serviceName: The name of our server (collectino of services)
//
//     This is used to build the path for our Bluetooth services. It also provides the base for the D-Bus owned name (see
//     getOwnedName.)
//
//     This value will be stored as lower-case only.
//
//     Retrieve this value using the `getName()` method.
//
// advertisingName: The name for this controller, as advertised over LE
//
//     IMPORTANT: Setting the advertisingName will change the system-wide name of the device. If that's not what you want, set
//     BOTH advertisingName and advertisingShortName to as empty string ("") to prevent setting the advertising
//     name.
//
//     Retrieve this value using the `getAdvertisingName()` method.
//
// advertisingShortName: The short name for this controller, as advertised over LE
//
//     According to the spec, the short name is used in case the full name doesn't fit within Extended Inquiry Response (EIR) or
//     Advertising Data (AD).
//
//     IMPORTANT: Setting the advertisingName will change the system-wide name of the device. If that's not what you want, set
//     BOTH advertisingName and advertisingShortName to as empty string ("") to prevent setting the advertising
//     name.
//
//     Retrieve this value using the `getAdvertisingShortName()` method.
//
    Server::Server(const std::string &serviceName, const std::string &advertisingName,
                   const std::string &advertisingShortName,
                   GGKServerDataGetter getter, GGKServerDataSetter setter) {
        // Save our names
        this->serviceName = serviceName;
        std::transform(this->serviceName.begin(), this->serviceName.end(), this->serviceName.begin(), ::tolower);
        this->advertisingName = advertisingName;
        this->advertisingShortName = advertisingShortName;

        // Register getter & setter for server data
        dataGetter = getter;
        dataSetter = setter;
        // Adapter configuration flags - set these flags based on how you want the adapter configured
        enableBREDR = false;
        enableSecureConnection = false;
        enableConnectable = true;
        enableAdvertising = true;
        enableBondable = false;


        //
        // Define the server
        //

        // Create the root D-Bus object and push it into the list
        objects.push_back(DBusObject(DBusObjectPath() + "com" + getServiceName()));

        // We're going to build off of this object, so we need to get a reference to the instance of the object as it resides in the
        // list (and not the object that would be added to the list.)
        objects.back()


#if 1         // Service: Device Information (0x180A)
                        //
                        // See: https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.service.device_information.xml
                .gattServiceBegin("device", "180A")

                        // Characteristic: Manufacturer Name String (0x2A29)
                        //
                        // See: https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.characteristic.manufacturer_name_string.xml
                .gattCharacteristicBegin("mfgr_name", "2A29", {"read"})

                        // Standard characteristic "ReadValue" method call
                .onReadValue(CHARACTERISTIC_METHOD_CALLBACK_LAMBDA
                                     {
                                             self.methodReturnValue(pInvocation, "Acme Inc.", true);
                                     })

                .gattCharacteristicEnd()

                        // Characteristic: Model Number String (0x2A24)
                        //
                        // See: https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.characteristic.model_number_string.xml
                .gattCharacteristicBegin("model_num", "2A24", {"read"})

                        // Standard characteristic "ReadValue" method call
                .onReadValue(CHARACTERISTIC_METHOD_CALLBACK_LAMBDA
                                     {
                                             self.methodReturnValue(pInvocation, "Marvin-PA", true);
                                     })

                .gattCharacteristicEnd()

                .gattServiceEnd()

                        // Current Time Service (0x1805)
                        //
                        // This is a time service that conforms to org.bluetooth.service.current_time. For details, see:
                        //
                        //    https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.service.current_time.xml
                        //
                        // Like the battery service, this also makes use of events. This one updates the time every tick.
                        //
                        // This showcases the use of events (see the call to .onEvent() below) for periodic actions. In this case, the action
                        // taken is to update time every tick. This probably isn't a good idea for a production service, but it has been quite
                        // useful for testing to ensure we're connected and updating.
                .gattServiceBegin("time", "1805")

                        // Characteristic: Current Time (0x2A2B)
                        //
                        // See: https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.characteristic.current_time.xml
                .gattCharacteristicBegin("current", "2A2B", {"read", "notify"})

                        // Standard characteristic "ReadValue" method call
                .onReadValue(CHARACTERISTIC_METHOD_CALLBACK_LAMBDA
                                     {
                                             self.methodReturnVariant(pInvocation, ServerUtils::gvariantCurrentTime(),
                                                                      true);
                                     })

                        // Update the time every tick of the periodic timer
                        //
                        // We'll send an change notification to any subscribed clients with the latest value
                .onEvent(1, nullptr, CHARACTERISTIC_EVENT_CALLBACK_LAMBDA
                        {
                                self.sendChangeNotificationVariant(pConnection, ServerUtils::gvariantCurrentTime());
                        })

                .gattCharacteristicEnd()

                        // Characteristic: Local Time Information (0x2A0F)
                        //
                        // See: https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.characteristic.local_time_information.xml
                .gattCharacteristicBegin("local", "2A0F", {"read"})

                        // Standard characteristic "ReadValue" method call
                .onReadValue(CHARACTERISTIC_METHOD_CALLBACK_LAMBDA
                                     {
                                             self.methodReturnVariant(pInvocation, ServerUtils::gvariantLocalTime(),
                                                                      true);
                                     })

                .gattCharacteristicEnd()
                .gattServiceEnd()

                        // Moovel FareConnect account (0xCC01)
                        //
                .gattServiceBegin("fareconnect", "cc01")

                        // Characteristic: Object ID (0x2AC3)
                        //
                        // What is the right characteristic?
                .gattCharacteristicBegin("account", "2ac3", {"write"})

                        // Standard characteristic "WriteValue" method call
                .onWriteValue(CHARACTERISTIC_METHOD_CALLBACK_LAMBDA
                                      {
                                              //
                                              GVariant * pAyBuffer = g_variant_get_child_value(pParameters, 0);
                                              std::string account = Utils::stringFromGVariantByteArray(pAyBuffer);
                                              std::cout << account << "\n";
                                      })

                .gattCharacteristicEnd()
                .gattServiceEnd()
/*
 * @brief Non-secure channel used to pass public key and encrypted AES symmetric key, 
 * before setting up secure channel
 * 	Key_transfer - non-secure function used by the client to write the public key for AES symmetric key 
 * 	transfer, triggers the on-update function. These functions for write, read and notify are all defined by 
 * 	lambda return functions. On update the key_transfer function generates and returns an encrypted symmetric 
 * 	key that can only be decrypted by the client's private key
 * 	@param public_key string
 * 	@return AES encrypted string
 *
 * 	test_key - After client receives the symmetric key, it encrypts 'Pass back' with the the key and sends the encrypted value to the server, the server decryptes the value and if it matches, the server encrypts 'Received' and sends the encrypted text to the client. The client verifies with itself if it has received 'Received' after decrypting the encrypted text.
 * 	@param encrypted string 
 * 	@return encrypted string
 *
 */
                        // Custom read/write text string service (00000001-1E3C-FAD4-74E2-97A033F1BFAA)
                        //
                        // This service will return a text string value (default: 'Hello, world!'). If the text value is updated, it will notify
                        // that the value has been updated and provide the new text from that point forward.
                .gattServiceBegin("non_secure", "00000001-1E3C-FAD4-74E2-97A033F1BFAA")


                        // Characteristic: String value (custom: 00000002-1E3C-FAD4-74E2-97A033F1BFAA)
                .gattCharacteristicBegin("key_transfer", "00000003-1E3C-FAD4-74E2-97A033F1BFAA", {"read", "write","notify"})

                        // Standard characteristic "WriteValue" method call
                .onWriteValue(CHARACTERISTIC_METHOD_CALLBACK_LAMBDA
                                      {
                                              // Receive text string from the client from a buffer
                                              GVariant * pAyBuffer = g_variant_get_child_value(pParameters, 0);
                                              std::string writeValue = Utils::stringFromGVariantByteArray(pAyBuffer);
					      /*
					       * Data is buffered so it needs to be defragmented as it is received us						    * ing a packaging protocol
					       *
					       * unique_client_id(32 bytes):[START]Text ......[END] (terminator)
					       *
					       */
                                              std::string public_key_fragment;
					      // Test if received string follows a specific format before processing
                                              if (writeValue.substr(36, 1) == ":" && writeValue.substr(8,1) == "-" && writeValue.substr(13,1) == "-" && writeValue.substr(18,1) == "-"){
                                                  std::string dev_id = writeValue.substr (0,36);
						  //Check if the fragmented array is a fragmented piece
                                                  if ( bufferStorage.find(dev_id) == bufferStorage.end() || bufferStorage[dev_id].find("status") == bufferStorage[dev_id].end() || bufferStorage[dev_id]["status"] == "not authenticated") {
                                                     //If fragmented append the pieces to bufferStorage 
						     if (writeValue.substr(37, 7) != "[START]"){
                                                          if (bufferStorage.find(dev_id) != bufferStorage.end() && bufferStorage[dev_id].find("pub_key") != bufferStorage[dev_id].end() && bufferStorage[dev_id]["pub_key"].find("[END]") != std::string::npos){
                                                              return;
                                                          }
                                                      }
                                                      public_key_fragment = writeValue.substr(37);
                                                      if ( bufferStorage.find(dev_id) == bufferStorage.end() && bufferStorage[dev_id].find("pub_key") == bufferStorage[dev_id].end()) {
                                                          bufferStorage[dev_id]["pub_key"] = "";
                                                      }

                                                      if (writeValue.find("[START]") != std::string::npos) {
                                                          bufferStorage[dev_id]["pub_key"] = "";
                                                      }
                                                      bufferStorage[dev_id]["pub_key"] += public_key_fragment;
                                                      // IF the packet is completely transferred and was reassembled
						      if (writeValue.find("[END]") != std::string::npos) {
                                                          //Create encryption module and generate the key and cipher for AES
                                                          // Load the necessary cipher

                                                          byte key[KEY_SIZE], iv[BLOCK_SIZE];
							
							  //Generate an AES key and iv for the AES_256_CTR cipher
                                                          em.gen_params(key, iv);
							  //Store the key and iv for future use by the same client
                                                          std::string charAESKey(reinterpret_cast<char*>(key));
                                                          bufferStorage[dev_id]["key"] = charAESKey;
                                                          std::string charAESiv(reinterpret_cast<char*>(iv));
                                                          bufferStorage[dev_id]["iv"] = charAESiv;

                                                          OPENSSL_cleanse(key, KEY_SIZE);
                                                          OPENSSL_cleanse(iv, BLOCK_SIZE);
							  //Call the update function if the aes cipher was 
							  //	succesfully generated

                                                          self.callOnUpdatedValue(pConnection, pUserData, dev_id);
                                                      }
                                                  } else {
							 //If the user is logged in let them know
                                                      self.callOnUpdatedValue(pConnection, pUserData, "already logged in");
                                                  }
                                              }


                                      })
                        // Here we use the onUpdatedValue to set a callback that isn't exposed to BlueZ, but rather allows us to manage
                        // updates to our value. These updates may have come from our own server or some other source.
                        //
                        // We can handle updates in any way we wish, but the most common use is to send a change notification.
                .onUpdatedValue(CHARACTERISTIC_UPDATED_VALUE_CALLBACK_LAMBDA
                                        {
                                        /*
					 * If a user is already logged in ignore this function call because the
					 * secure channel is already created
					 */        
					if (id != "already logged in"){
                                            byte key[KEY_SIZE], iv[BLOCK_SIZE];
					    //Reassemble the key and iv from the storageBuffer
                                            memcpy(key, bufferStorage[id]["key"].data(), bufferStorage[id]["key"].length());
                                            memcpy(iv, bufferStorage[id]["iv"].data(), bufferStorage[id]["iv"].length());

                                            //Base 64 encode the key and the block values
					    char * key_char = em.base64((unsigned char*)key, KEY_SIZE);
                                            char * iv_char = em.base64((unsigned char*)iv, BLOCK_SIZE);
						//Create the plaintxt message for transferring the key and block size
                                            std::string charAESKey(key_char);
                                            std::string charAESiv(iv_char);
                                            std::string msg;
                                            msg = "[key:]";
                                            msg += charAESKey;
                                            msg += "[iv:]";
                                            msg += charAESiv;
					    // Gather the public key 
                                            char* first_message = const_cast<char*>(msg.c_str());
                                            string pub_key_str = removeStartAndEnd(bufferStorage[id]["pub_key"]);
					    
					    // Encrypt the plaintext AES cipher key and block with the 
					    // client public key

                                            int key_length = pub_key_str.length();
                                            char key1[key_length];
                                            strcpy(key1, pub_key_str.c_str());
                                            BIO* mem = BIO_new_mem_buf(key1, (int)sizeof(key1));
                                            EVP_PKEY* pkey = PEM_read_bio_PUBKEY(mem, NULL, NULL, NULL);
                                            RSA * public_key = EVP_PKEY_get1_RSA(pkey);
                                            char *encrypt = NULL;
					    // Max size of the RSA encrypted value is 256
                                            encrypt = (char*)malloc(256);
                                            int encrypt_length = em.public_encrypt(strlen(first_message), (unsigned char*)first_message, (unsigned char*)encrypt, public_key, RSA_PKCS1_PADDING);
                                            if(encrypt_length == -1) {
                                                printf("An error occurred in public_encrypt() method");
                                            }
					    //Create the encrypted string that will be sent to the client
                                            std::string send(encrypt, encrypt_length);
					    //Send the string
                                            self.sendChangeNotificationValue(pConnection, send);
                                            BIO_free_all(mem);
                                            EVP_PKEY_free(pkey);
                                            RSA_free(public_key);
                                            free(encrypt);
                                            OPENSSL_cleanse(key, KEY_SIZE);
                                            OPENSSL_cleanse(iv, BLOCK_SIZE);

                                        } else {
                                            //Send the already logged in if so
						self.sendChangeNotificationValue(pConnection, id);
                                                }
                                                return true;

					})

                        // GATT Descriptor: Characteristic User Description (0x2901)
                        //
                        // See: https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.descriptor.gatt.characteristic_user_description.xml
                .gattDescriptorBegin("description", "2901", {"read"})

                        // Standard descriptor "ReadValue" method call
                .onReadValue(DESCRIPTOR_METHOD_CALLBACK_LAMBDA
                                     {
                                             const char *pDescription = "A mutable text string used for testing. Read and write to me, it tickles!";
                                             self.methodReturnValue(pInvocation, pDescription, true);
                                     })

                .gattDescriptorEnd()

                .gattCharacteristicEnd()
                        // Characteristic: String value (custom: 00000002-1E3C-FAD4-74E2-97A033F1BFAA)
                .gattCharacteristicBegin("test_key", "00000004-1E3C-FAD4-74E2-97A033F1BFAA", {"read","write","notify"})

                        // Standard characteristic "WriteValue" method call
                .onWriteValue(CHARACTERISTIC_METHOD_CALLBACK_LAMBDA
                                      {
                                              // Update the text string value
                                              GVariant * pAyBuffer = g_variant_get_child_value(pParameters, 0);
                                              std::string writeValue = Utils::stringFromGVariantByteArray(pAyBuffer);
                                              std::string public_key_fragment;
                                              if (writeValue.substr(36, 1) == ":" && writeValue.substr(8,1) == "-" && writeValue.substr(13,1) == "-" && writeValue.substr(18,1) == "-" && writeValue.find("[START]") != std::string::npos && writeValue.find("[END]") != std::string::npos){
                                          std::string dev_id = writeValue.substr (0,36);
                                          if ( bufferStorage.find(dev_id) != bufferStorage.end() && bufferStorage[dev_id].find("key") != bufferStorage[dev_id].end() && bufferStorage[dev_id].find("iv") != bufferStorage[dev_id].end()) {
                                              std::string startAndEnd = writeValue.substr(37);
                                              std::string withoutStartAndEnd = removeStartAndEnd(startAndEnd);
                                              int first_close = withoutStartAndEnd.find("]");
                                              int first_colon = withoutStartAndEnd.find(":");
                                              std::string sizeNumber = withoutStartAndEnd.substr(first_colon+1, first_close-first_colon);
                                              int sizeOfDecrypted = std::stoi(sizeNumber);
                                              std::string removeSize = withoutStartAndEnd.substr(first_close+1);
                                              std::cout << removeSize << std::endl;

                                              //Create encryption module and generate the key and cipher for AES
                                              // Load the necessary cipher

                                              byte key[KEY_SIZE], iv[BLOCK_SIZE];
                                              memcpy(key, bufferStorage[dev_id]["key"].data(), bufferStorage[dev_id]["key"].length());
                                              memcpy(iv, bufferStorage[dev_id]["iv"].data(), bufferStorage[dev_id]["iv"].length());

                                              std::string result = em.base64_decode(removeSize);
                                              std::cout << "Decoded: " << result << std::endl;
                                              secure_string ctext = result.c_str();
                                              secure_string rtext;


                                              em.aes_decrypt(key, iv, sizeOfDecrypted, ctext, rtext);

                                              OPENSSL_cleanse(key, KEY_SIZE);
                                              OPENSSL_cleanse(iv, BLOCK_SIZE);
                                              if (rtext.find("Pass back") != std::string::npos){
                                                  self.callOnUpdatedValue(pConnection, pUserData, dev_id);
                                              }
                                      }
                                      }


                                      })
                        // Here we use the onUpdatedValue to set a callback that isn't exposed to BlueZ, but rather allows us to manage
                        // updates to our value. These updates may have come from our own server or some other source.
                        //
                        // We can handle updates in any way we wish, but the most common use is to send a change notification.
                .onUpdatedValue(CHARACTERISTIC_UPDATED_VALUE_CALLBACK_LAMBDA
                                        {


                                                byte key[KEY_SIZE], iv[BLOCK_SIZE];
                                                memcpy(key, bufferStorage[id]["key"].data(), bufferStorage[id]["key"].length());
                                                memcpy(iv, bufferStorage[id]["iv"].data(), bufferStorage[id]["iv"].length());
                                                // plaintext, ciphertext, recovered text
                                                std::string report = "Received";
                                                secure_string ptext = report.c_str();
                                                secure_string ctext, rtext;


                                                em.aes_encrypt(key, iv, ptext, ctext);
                                                const char * ret = ctext.c_str();
                                                OPENSSL_cleanse(key, KEY_SIZE);
                                                OPENSSL_cleanse(iv, BLOCK_SIZE);
                                                self.sendChangeNotificationValue(pConnection, ret);
                                                return true;

                                        })

                        // GATT Descriptor: Characteristic User Description (0x2901)
                        //
                        // See: https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.descriptor.gatt.characteristic_user_description.xml
                .gattDescriptorBegin("description", "2901", {"read"})

                        // Standard descriptor "ReadValue" method call
                .onReadValue(DESCRIPTOR_METHOD_CALLBACK_LAMBDA
                                     {
                                             const char *pDescription = "A mutable text string used for testing. Read and write to me, it tickles!";
                                             self.methodReturnValue(pInvocation, pDescription, true);
                                     })

                .gattDescriptorEnd()

                .gattCharacteristicEnd()
                .gattServiceEnd()

                        /*
 * @brief Secure channel used for login, settings, transferring raw gps data, and retrieving reports for the client
 */
                .gattServiceBegin("secure", "00000001-1E3D-FAD4-74E2-97A033F1BFEE")

                        // Characteristic: String value (custom: 00000002-1E3C-FAD4-74E2-97A033F1BFAA)
                .gattCharacteristicBegin("login", "00000002-1E3D-FAD4-74E2-97A033F1BFEE", {"write", "notify"})

                        // Standard characteristic "ReadValue" method call
                        // Standard characteristic "WriteValue" method call
                .onWriteValue(CHARACTERISTIC_METHOD_CALLBACK_LAMBDA
                                      {
                                              // Update the text string value
					      UserController uc("server", "LifeVector123");
                                              GVariant * pAyBuffer = g_variant_get_child_value(pParameters, 0);
					      
					      std::string ss(Utils::stringFromGVariantByteArray(pAyBuffer).c_str());
					      uc.createUser(ss.substr(0, ss.find(",")), "1111231", ss.substr(1, ss.find(","))); 
					      
                                              // Since all of these methods (onReadValue, onWriteValue, onUpdateValue) are all part of the same
                                              // Characteristic interface (which just so happens to be the same interface passed into our self
                                              // parameter) we can that parameter to call our own onUpdatedValue method
                                              self.callOnUpdatedValue(pConnection, pUserData);
                                      })
                        // Here we use the onUpdatedValue to set a callback that isn't exposed to BlueZ, but rather allows us to manage
                        // updates to our value. These updates may have come from our own server or some other source.
                        //
                        // We can handle updates in any way we wish, but the most common use is to send a change notification.
                .onUpdatedValue(CHARACTERISTIC_UPDATED_VALUE_CALLBACK_LAMBDA
                                        {
                                                const char *pTextString = self.getDataPointer<const char *>("text/string", "Logged in Successfully");
                                                self.sendChangeNotificationValue(pConnection, pTextString);
                                                return true;
                                        })
                .gattCharacteristicEnd()
                        // Characteristic: String value (custom: 00000002-1E3C-FAD4-74E2-97A033F1BFAA)
                .gattCharacteristicBegin("confirmDeviceID", "00000003-1E3D-FAD4-74E2-97A033F1BFEE", {"write", "notify"})

                        // Standard characteristic "ReadValue" method call
                        // Standard characteristic "WriteValue" method call
                .onWriteValue(CHARACTERISTIC_METHOD_CALLBACK_LAMBDA
                                      {
                                              // Update the text string value
                                              GVariant * pAyBuffer = g_variant_get_child_value(pParameters, 0);
                                              self.setDataPointer("text/string", Utils::stringFromGVariantByteArray(pAyBuffer).c_str());

                                              // Since all of these methods (onReadValue, onWriteValue, onUpdateValue) are all part of the same
                                              // Characteristic interface (which just so happens to be the same interface passed into our self
                                              // parameter) we can that parameter to call our own onUpdatedValue method
                                              self.callOnUpdatedValue(pConnection, pUserData);
                                      })
                        // Here we use the onUpdatedValue to set a callback that isn't exposed to BlueZ, but rather allows us to manage
                        // updates to our value. These updates may have come from our own server or some other source.
                        //
                        // We can handle updates in any way we wish, but the most common use is to send a change notification.
                .onUpdatedValue(CHARACTERISTIC_UPDATED_VALUE_CALLBACK_LAMBDA
                                        {
                                                const char *pTextString = self.getDataPointer<const char *>("text/string", "1234");
                                                self.sendChangeNotificationValue(pConnection, pTextString);
                                                return true;
                                        })
                .gattCharacteristicEnd()
                .gattCharacteristicBegin("sendMAC", "00000004-1E3D-FAD4-74E2-97A033F1BFEE", {"write", "notify"})

                        // Standard characteristic "ReadValue" method call
                        // Standard characteristic "WriteValue" method call
                .onWriteValue(CHARACTERISTIC_METHOD_CALLBACK_LAMBDA
                                      {
                                              // Update the text string value
                                              GVariant * pAyBuffer = g_variant_get_child_value(pParameters, 0);
                                              self.setDataPointer("text/string", Utils::stringFromGVariantByteArray(pAyBuffer).c_str());

                                              // Since all of these methods (onReadValue, onWriteValue, onUpdateValue) are all part of the same
                                              // Characteristic interface (which just so happens to be the same interface passed into our self
                                              // parameter) we can that parameter to call our own onUpdatedValue method
                                              self.callOnUpdatedValue(pConnection, pUserData);
                                      })
                        // Here we use the onUpdatedValue to set a callback that isn't exposed to BlueZ, but rather allows us to manage
                        // updates to our value. These updates may have come from our own server or some other source.
                        //
                        // We can handle updates in any way we wish, but the most common use is to send a change notification.
                .onUpdatedValue(CHARACTERISTIC_UPDATED_VALUE_CALLBACK_LAMBDA
                                        {
                                                const char *pTextString = self.getDataPointer<const char *>("text/string", "123");
                                                self.sendChangeNotificationValue(pConnection, pTextString);
                                                return true;
                                        })
                .gattCharacteristicEnd()
                .gattCharacteristicBegin("sendGPSData", "00000005-1E3D-FAD4-74E2-97A033F1BFEE", {"write", "notify"})

                        // Standard characteristic "ReadValue" method call
                        // Standard characteristic "WriteValue" method call
                .onWriteValue(CHARACTERISTIC_METHOD_CALLBACK_LAMBDA
                                      {
                                              // Update the text string value
                                              GVariant * pAyBuffer = g_variant_get_child_value(pParameters, 0);
                                              self.setDataPointer("text/string", Utils::stringFromGVariantByteArray(pAyBuffer).c_str());

                                              // Since all of these methods (onReadValue, onWriteValue, onUpdateValue) are all part of the same
                                              // Characteristic interface (which just so happens to be the same interface passed into our self
                                              // parameter) we can that parameter to call our own onUpdatedValue method
                                              self.callOnUpdatedValue(pConnection, pUserData);
                                      })
                        // Here we use the onUpdatedValue to set a callback that isn't exposed to BlueZ, but rather allows us to manage
                        // updates to our value. These updates may have come from our own server or some other source.
                        //
                        // We can handle updates in any way we wish, but the most common use is to send a change notification.
                .onUpdatedValue(CHARACTERISTIC_UPDATED_VALUE_CALLBACK_LAMBDA
                                        {
                                                const char *pTextString = self.getDataPointer<const char *>("text/string", "123123");
                                                self.sendChangeNotificationValue(pConnection, pTextString);
                                                return true;
                                        })
                .gattCharacteristicEnd()
                .gattCharacteristicBegin("getUserInformation", "00000006-1E3C-FAD4-74E2-97A033F1BFAA", {"read"})

                        // Standard characteristic "ReadValue" method call
                .onReadValue(CHARACTERISTIC_METHOD_CALLBACK_LAMBDA
                                     {
                                             const char *pTextString = self.getDataPointer<const char *>("text/string", "Hello Mario");
                                             self.methodReturnValue(pInvocation, pTextString, true);
                                     })
                .gattCharacteristicEnd()
                .gattServiceEnd()
#endif
                ; // << -- NOTE THE SEMICOLON

        //  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
        //                                                ____ _____ ___  _____
        //                                               / ___|_   _/ _ \|  _  |
        //                                               \___ \ | || | | | |_) |
        //                                                ___) || || |_| |  __/
        //                                               |____/ |_| \___/|_|
        //
        // You probably shouldn't mess with stuff beyond this point. It is required to meet BlueZ's requirements for a GATT Service.
        //
        // >>
        // >>  WHAT IT IS
        // >>
        //
        // From the BlueZ D-Bus GATT API description (https://git.kernel.org/pub/scm/bluetooth/bluez.git/plain/doc/gatt-api.txt):
        //
        //     "To make service registration simple, BlueZ requires that all objects that belong to a GATT service be grouped under a
        //     D-Bus Object Manager that solely manages the objects of that service. Hence, the standard DBus.ObjectManager interface
        //     must be available on the root service path."
        //
        // The code below does exactly that. Notice that we're doing much of the same work that our Server description does except that
        // instead of defining our own interfaces, we're following a pre-defined standard.
        //
        // The object types and method names used in the code below may look unfamiliar compared to what you're used to seeing in the
        // Server desecription. That's because the server description uses higher level types that define a more GATT-oriented framework
        // to build your GATT services. That higher level functionality was built using a set of lower-level D-Bus-oriented framework,
        // which is used in the code below.
        //  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -

        // Create the root object and push it into the list. We're going to build off of this object, so we need to get a reference
        // to the instance of the object as it resides in the list (and not the object that would be added to the list.)
        //
        // This is a non-published object (as specified by the 'false' parameter in the DBusObject constructor.) This way, we can
        // include this within our server hieararchy (i.e., within the `objects` list) but it won't be exposed by BlueZ as a Bluetooth
        // service to clietns.
        objects.push_back(DBusObject(DBusObjectPath(), false));

        // Get a reference to the new object as it resides in the list
        DBusObject &objectManager = objects.back();

        // Create an interface of the standard type 'org.freedesktop.DBus.ObjectManager'
        //
        // See: https://dbus.freedesktop.org/doc/dbus-specification.html#standard-interfaces-objectmanager
        auto omInterface = std::make_shared<DBusInterface>(objectManager, "org.freedesktop.DBus.ObjectManager");

        // Add the interface to the object manager
        objectManager.addInterface(omInterface);

        // Finally, we setup the interface. We do this by adding the `GetManagedObjects` method as specified by D-Bus for the
        // 'org.freedesktop.DBus.ObjectManager' interface.
        const char *pInArgs[] = {nullptr};
        const char *pOutArgs = "a{oa{sa{sv}}}";
        omInterface->addMethod("GetManagedObjects", pInArgs, pOutArgs, INTERFACE_METHOD_CALLBACK_LAMBDA
                {
                        ServerUtils::getManagedObjects(pInvocation);
                });
    }

// ---------------------------------------------------------------------------------------------------------------------------------
// Utilitarian
// ---------------------------------------------------------------------------------------------------------------------------------

// Find a D-Bus interface within the given D-Bus object
//
// If the interface was found, it is returned, otherwise nullptr is returned
    std::shared_ptr<const DBusInterface>
    Server::findInterface(const DBusObjectPath &objectPath, const std::string &interfaceName) const {
        for (const DBusObject &object : objects) {
            std::shared_ptr<const DBusInterface> pInterface = object.findInterface(objectPath, interfaceName);
            if (pInterface != nullptr) {
                return pInterface;
            }
        }

        return nullptr;
    }

// Find and call a D-Bus method within the given D-Bus object on the given D-Bus interface
//
// If the method was called, this method returns true, otherwise false. There is no result from the method call itself.
    bool Server::callMethod(const DBusObjectPath &objectPath, const std::string &interfaceName,
                            const std::string &methodName, GDBusConnection *pConnection, GVariant *pParameters,
                            GDBusMethodInvocation *pInvocation, gpointer pUserData) const {
        for (const DBusObject &object : objects) {
            if (object.callMethod(objectPath, interfaceName, methodName, pConnection, pParameters, pInvocation,
                                  pUserData)) {
                return true;
            }
        }

        return false;
    }

// Find a GATT Property within the given D-Bus object on the given D-Bus interface
//
// If the property was found, it is returned, otherwise nullptr is returned
    const GattProperty *Server::findProperty(const DBusObjectPath &objectPath, const std::string &interfaceName,
                                             const std::string &propertyName) const {
        std::shared_ptr<const DBusInterface> pInterface = findInterface(objectPath, interfaceName);

        // Try each of the GattInterface types that support properties?
        if (std::shared_ptr<const GattInterface> pGattInterface = TRY_GET_CONST_INTERFACE_OF_TYPE(pInterface,
                                                                                                  GattInterface)) {
            return pGattInterface->findProperty(propertyName);
        } else if (std::shared_ptr<const GattService> pGattInterface = TRY_GET_CONST_INTERFACE_OF_TYPE(pInterface,
                                                                                                       GattService)) {
            return pGattInterface->findProperty(propertyName);
        } else if (std::shared_ptr<const GattCharacteristic> pGattInterface = TRY_GET_CONST_INTERFACE_OF_TYPE(
                pInterface, GattCharacteristic)) {
            return pGattInterface->findProperty(propertyName);
        }

        return nullptr;
    }

}; // namespace ggk
