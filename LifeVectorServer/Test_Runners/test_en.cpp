#include "../EncryptionModule.h"

// g++ -Wall -std=c++11 evp-encrypt.cxx -o evp-encrypt.exe -lcrypto

typedef unsigned char byte;
typedef std::basic_string<char, std::char_traits<char>, zallocator<char> > secure_string;
using EVP_CIPHER_CTX_free_ptr = std::unique_ptr<EVP_CIPHER_CTX, decltype(&::EVP_CIPHER_CTX_free)>;




int main(int argc, char* argv[])
{
    EncryptionModule em;
    // Load the necessary cipher
    EVP_add_cipher(EVP_aes_256_cbc());

    // plaintext, ciphertext, recovered text
    std::string hello = "helalsdasd";
    secure_string ptext = hello.c_str();
    secure_string ctext, rtext;

    byte key[KEY_SIZE], iv[BLOCK_SIZE];
    em.gen_params(key, iv);

    em.aes_encrypt(key, iv, ptext, ctext);
    em.aes_decrypt(key, iv, ctext, rtext);

    OPENSSL_cleanse(key, KEY_SIZE);
    OPENSSL_cleanse(iv, BLOCK_SIZE);

    std::cout << "Original message:\n"
    "" << ptext << std::endl;
    std::cout << "Cipher message:\n"
    "" << ctext << std::endl;
    std::cout << "Recovered message:\n"
    "" << rtext << std::endl;
//
//    char message[KEY_LENGTH / 8] = "Batuhan AVLAYAN - OpenSSL_RSA demo";
//    char *encrypt = NULL;
//    char *decrypt = NULL;
//
//    char private_key_pem[12] = "private_key";
//    char public_key_pem[11]  = "public_key";
//    static const char key1[] = "-----BEGIN PRIVATE KEY-----\n"
//    "MIIEvQIBADANBgkqhkiG9w0BAQEFAASCBKcwggSjAgEAAoIBAQDKcv436G4lRKh3\n"
//    "ZXuK0QKusB9flucDIVvNbu+iaofwjun8g8DkspU8tlgEAgvR5WX4IZoJ3rVE3HsI\n"
//    "IPYkrrl46Ud3MMqca9Nl8siZtSabQRDGWPJUmTFaIOzreLlcjWfC8LiGTbcPEdWR\n"
//    "JBbc7rgvikaZF/paRoOtE3STRdKrdccItTeJU3JHjHYHImSJUqXRKp3qjAEqyVKp\n"
//    "jJTzn89Svs6nEpdmp+a/Ef+e1sTH2KLtosYtSkb9nQxnH/hs8zYQgJ/cs9GMCeca\n"
//    "Xqt8ezIyEdDtjuXUu04sZ4sVx0NFsPjDe3B7iUekCglIw2L7faV0SEm2WbFCPAeC\n"
//    "vdLI33UHAgMBAAECggEAcFWghEs/mwRe6mlwRNW3RFFMB1GHe/AtrW9KHvg32B6k\n"
//    "7U36YaxKLPcNHaQ5BM3iZ+3VOHbQZJZm2lrX2gDtv47J7fQv6N3X3nD6KHghJPLU\n"
//    "qQq/5HtgrB8q93n0eMup8Lx712IWGpGNzMyWtx7rKs3yYI6yhxcrh4UNIY8T17kz\n"
//    "dF1JJV6OQQfg6IvP68WoGuzW+JVkUw+6ROKqnOmnmIO0/X1uTx2s6/aNBM/NwO+S\n"
//    "6NjlVxYZJJ/EkvAeABP1lYVT6P942WSg9/+KryzKErxL1mJgCmYtBW6zW33bpMwI\n"
//    "lBt1RWSlZCxENR1X6Vb+2Y9N0TJfQhtQLCbd3FgkCQKBgQD+Y9uZtOm5fJx0gehz\n"
//    "iAFqkJu2+P9xqyo2bWKH+en5LaOwogzxCfY5ZOX4Sy+4s/b3IK1JqgP15Y7L0wpj\n"
//    "hpMUQddpCmh2KzLqNjgWLvEb3vKWHk0GCDBHmGQ4OidY9xbxG1wHuFohox11MS0G\n"
//    "ua190Y7Df8a5G838S9ifwwp0JQKBgQDLuvwdW2NPsT+jzfEVex8uBwel/fGIOSkS\n"
//    "nUq9XbNpSAG+FKlRbkzwMnNR9aGNXT9SYQGF48zqN5u/4xm4sES1JxEN4+B+rSzi\n"
//    "WNSry/2oJGAjm99vQZ6Qprn/zwzY4Qtm93s7nh7cKkONPzBjvhff05C4JHsLjFqr\n"
//    "IgIVUEjGuwKBgQC60Jp0Rmdd0CUValFKhpaDq66usCznTMuzLKEvOliYpOuQy3C1\n"
//    "kps/itlXfd+NxJuY4GZhsnk2G17CUDlQ3YMlXTTT7tDH9vFtDmQh5lcqIJAFrjri\n"
//    "MyFBcqgmoo5CxzIR3gW3paqLCT6Y0YQ/bBv8SeiNqUhGqCSdIxXyPUlwAQKBgDY8\n"
//    "fje2h5bEoiOShNbqIx/WwUOAeleAmB2bgDUvh77Ea6pVx8mk6pibZbXxBWQj6eKd\n"
//    "bpFdVsUXrsADzmoriu8LCnA/NRmkMqnDakKeGNY1cN6p9W6h8nuh1yxxy4LFTorH\n"
//    "4ae6bY7BNaCZrOEwzxxIL2PGWnd1kwGyGQnhiK5XAoGAGGOgoYeF9FX8xDE+9wCP\n"
//    "rofhUjR/RTU+LOb9AYSzC4Bs/negwIse2Z+Spd0+QCwQQXjwed/ggcogjjuzHvix\n"
//    "PAjIbzHwtOOR3uuZLZUzbbGFK0bE8pUkzumSUsbMGsCEkItCW4Sjd/n0hGsE93Hs\n"
//    "RkduM6euSfwBIaSn84x3Kzg=\n"
//    "-----END PRIVATE KEY-----\n";
//    static const char key2[] = "-----BEGIN PUBLIC KEY-----\n"
//    "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAynL+N+huJUSod2V7itEC\n"
//    "rrAfX5bnAyFbzW7vomqH8I7p/IPA5LKVPLZYBAIL0eVl+CGaCd61RNx7CCD2JK65\n"
//    "eOlHdzDKnGvTZfLImbUmm0EQxljyVJkxWiDs63i5XI1nwvC4hk23DxHVkSQW3O64\n"
//    "L4pGmRf6WkaDrRN0k0XSq3XHCLU3iVNyR4x2ByJkiVKl0Sqd6owBKslSqYyU85/P\n"
//    "Ur7OpxKXZqfmvxH/ntbEx9ii7aLGLUpG/Z0MZx/4bPM2EICf3LPRjAnnGl6rfHsy\n"
//    "MhHQ7Y7l1LtOLGeLFcdDRbD4w3twe4lHpAoJSMNi+32ldEhJtlmxQjwHgr3SyN91\n"
//    "BwIDAQAB\n"
//    "-----END PUBLIC KEY-----\n";
//    BIO* mem = BIO_new_mem_buf(key1, (int)sizeof(key1));
//    BIO* mem1 = BIO_new_mem_buf(key2, (int)sizeof(key2));
//    EVP_PKEY* pkey = PEM_read_bio_PrivateKey(mem, NULL, NULL, NULL);
//    EVP_PKEY* pkey2 = PEM_read_bio_PUBKEY(mem1, NULL, NULL, NULL);
//    RSA * private_key = EVP_PKEY_get1_RSA(pkey);
//    RSA * public_key = EVP_PKEY_get1_RSA(pkey2);
//    BIO * pubkeybio = BIO_new(BIO_s_mem());
//    BIO * prikeybio = BIO_new(BIO_s_mem());
//    RSA_print(pubkeybio, public_key, 0);
//    char buffer [2048];
//    std::string res = "";
//    if(BIO_read (pubkeybio, buffer, 2048) > 0)
//    {
//        std::cout << "as" << std::endl;
//	    std::cout << buffer;
//    }
//    BIO_free(pubkeybio);
//    RSA_print(prikeybio, private_key, 0);
//    char buffer1 [2048];
//    while (BIO_read (prikeybio, buffer1, 2048) > 0)
//    {
//	    std::cout << buffer1;
//    }
//    BIO_free(prikeybio);
//    std::cout << "asdsdasdas" << std::endl;
//    encrypt = (char*)malloc(RSA_size(private_key));
//    std::cout << "asda" << std::endl;
//    int encrypt_length = em.public_encrypt(strlen(message) + 1, (unsigned char*)message, (unsigned char*)encrypt, public_key, RSA_PKCS1_OAEP_PADDING);
//    if(encrypt_length == -1) {
//        printf("An error occurred in public_encrypt() method");
//    }
//    printf("Data has been encrypted.");
//
//    em.create_encrypted_file(encrypt, public_key);
//    printf("Encrypted file has been created.");
//
//    decrypt = (char *)malloc(encrypt_length);
//    int decrypt_length = em.private_decrypt(encrypt_length, (unsigned char*)encrypt, (unsigned char*)decrypt, private_key, RSA_PKCS1_OAEP_PADDING);
//    if(decrypt_length == -1) {
//        printf("An error occurred in private_decrypt() method");
//    }
//    printf("Data has been decrypted.");
//
//    FILE *decrypted_file = fopen("decrypted_file.txt", "w");
//    fwrite(decrypt, sizeof(*decrypt), decrypt_length - 1, decrypted_file);
//    fclose(decrypted_file);
//    printf("Decrypted file has been created.");
//
//    free(private_key);
//    free(public_key);
//    free(encrypt);
//    free(decrypt);
//    printf("OpenSSL_RSA has been finished.");

    return 0;
}


