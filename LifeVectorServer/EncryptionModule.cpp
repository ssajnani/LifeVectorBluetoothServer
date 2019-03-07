#include "EncryptionModule.h"


void EncryptionModule::gen_params(byte key[KEY_SIZE], byte iv[BLOCK_SIZE])
{
    int rc = RAND_bytes(key, KEY_SIZE);
    if (rc != 1)
        throw std::runtime_error("RAND_bytes key failed");

    rc = RAND_bytes(iv, BLOCK_SIZE);
    if (rc != 1)
        throw std::runtime_error("RAND_bytes for iv failed");
}

void EncryptionModule::aes_encrypt(const byte key[KEY_SIZE], const byte iv[BLOCK_SIZE], const secure_string& ptext, secure_string& ctext)
{
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    int rc = EVP_EncryptInit(ctx, EVP_aes_256_ctr(), key, iv);
    if (rc != 1)
        throw std::runtime_error("EVP_EncryptInit_ex failed");

    // Recovered text expands upto BLOCK_SIZE
    ctext.resize(ptext.size()+BLOCK_SIZE);
    int out_len1 = (int)ctext.size();

    rc = EVP_EncryptUpdate(ctx, (byte*)&ctext[0], &out_len1, (const byte*)&ptext[0], (int)ptext.size());
    if (rc != 1)
        throw std::runtime_error("EVP_EncryptUpdate failed");

    int out_len2 = (int)ctext.size() - out_len1;
    rc = EVP_EncryptFinal(ctx, (byte*)&ctext[0]+out_len1, &out_len2);
    if (rc != 1)
        throw std::runtime_error("EVP_EncryptFinal_ex failed");

    // Set cipher text size now that we know it
    ctext.resize(out_len1 + out_len2);


    free(ctx);

}

void EncryptionModule::handleErrors(void)
{
    perror("Error: ");
    ERR_print_errors_fp(stderr);
    abort();
}

void EncryptionModule::aes_decrypt(const byte key[KEY_SIZE], const byte iv[BLOCK_SIZE], int length, const secure_string& ctext, secure_string& rtext)
{
    std::cout << "inside1" << std::endl;

    EVP_CIPHER_CTX *ctx2 = EVP_CIPHER_CTX_new();
    int declen1, declen2;
    int rc = EVP_DecryptInit(ctx2, EVP_aes_256_ctr(), key, iv);
    if (rc != 1)
        throw std::runtime_error("EVP_DecryptInit failed");
    std::cout << "inside2" << std::endl;

    // Recovered text contracts upto BLOCK_SIZE
    rtext.resize(length);
    int out_len1;

    rc = EVP_DecryptUpdate(ctx2, (byte*)&rtext[0], &out_len1, (const byte*)&ctext[0], (int)ctext.size());
    if (rc != 1)
        throw std::runtime_error("EVP_DecryptUpdate failed");

    std::cout << "inside3" << std::endl;

    int out_len2;
    rc = EVP_DecryptFinal_ex(ctx2, (byte*)&rtext[0]+out_len1, &out_len2);
    if (rc != 1)
        throw std::runtime_error("EVP_DecryptFinal_ex failed");
    std::cout << "inside4" << std::endl;
    std::cout << out_len1 << std::endl;
    std::cout << out_len2 << std::endl;

//    rtext.resize(out_len1 + out_len2);
    std::cout << "inside5" << std::endl;


    free(ctx2);
}

int EncryptionModule::envelope_seal(EVP_PKEY **pub_key, unsigned char *plaintext, int plaintext_len, unsigned char **encrypted_key, int *encrypted_key_len, unsigned char *iv, unsigned char *ciphertext)
{
    EVP_CIPHER_CTX *ctx;

    int ciphertext_len;

    int len;

    if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();
    std::cout << "assdas" << std::endl;

    if(1 != EVP_SealInit(ctx, EVP_aes_128_cbc(),
                         encrypted_key,
                         encrypted_key_len,
                         iv,
                         pub_key, 1))
    {
        handleErrors();
    }
    std::cout << "ass" << std::endl;

    if(1 != EVP_SealUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
    {
        handleErrors();
    }
    ciphertext_len = len;

    if(1 != EVP_SealFinal(ctx, ciphertext + len, &len))
    {
        handleErrors();
    }
    ciphertext_len += len;

    EVP_CIPHER_CTX_free(ctx);

    return ciphertext_len;
}



int EncryptionModule::envelope_open(EVP_PKEY *priv_key, unsigned char *ciphertext, int ciphertext_len, unsigned char *encrypted_key, int encrypted_key_len, unsigned char *iv, unsigned char *plaintext)
{
    EVP_CIPHER_CTX *ctx;

    int len;

    int plaintext_len;

    if(!(ctx = EVP_CIPHER_CTX_new()))
    {
        handleErrors();
    }

    if(1 != EVP_OpenInit(ctx, EVP_aes_128_cbc(), encrypted_key,
                         encrypted_key_len, iv, priv_key))
    {
        handleErrors();
    }

    if(1 != EVP_OpenUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
    {
        handleErrors();
    }
    plaintext_len = len;

    if(1 != EVP_OpenFinal(ctx, plaintext + len, &len))
    {
        handleErrors();
    }
    plaintext_len += len;

    EVP_CIPHER_CTX_free(ctx);
    plaintext[plaintext_len] = '\0';

    return plaintext_len;
}

void EncryptionModule::getKeyFromString(std::string key, int pem_type, RSA * cipher) {
    BIO *mem= BIO_new(BIO_s_mem());
    BIO_write(mem, key.c_str(), key.length());
    if (pem_type == PUBLIC_KEY_PEM){
	PEM_read_bio_RSA_PUBKEY(mem, &cipher, 0, 0);
    } else if (pem_type == PRIVATE_KEY_PEM){
	PEM_read_bio_RSAPrivateKey(mem, &cipher, 0, 0);
    }
    BIO_free(mem);
}

int EncryptionModule::public_encrypt(int flen, unsigned char* from, unsigned char* to, RSA* key, int padding) {

    int result = RSA_public_encrypt(flen, from, to, key, padding);
    return result;
}

char * EncryptionModule::base64(const unsigned char* input, int length)
{
    BIO *bmem, *b64;
    BUF_MEM *bptr;
    char *buff = NULL;

    b64 = BIO_new(BIO_f_base64());
    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
    bmem = BIO_new(BIO_s_mem());
    b64 = BIO_push(b64, bmem);
    BIO_write(b64, input, length);
    BIO_flush(b64);
    BIO_get_mem_ptr(b64, &bptr);

    buff = (char *)malloc(bptr->length+1);
    memcpy(buff, bptr->data, bptr->length);
    buff[bptr->length] = '\0';

//    BIO_free_all(b64);

    return buff;
}



static const std::string base64_chars =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789+/";


static inline bool is_base64(unsigned char c) {
    return (isalnum(c) || (c == '+') || (c == '/'));
}


std::string EncryptionModule::base64_decode(std::string const& encoded_string) {
    int in_len = encoded_string.size();
    int i = 0;
    int j = 0;
    int in_ = 0;
    unsigned char char_array_4[4], char_array_3[3];
    std::string ret;

    while (in_len-- && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
        char_array_4[i++] = encoded_string[in_]; in_++;
        if (i ==4) {
            for (i = 0; i <4; i++)
                char_array_4[i] = base64_chars.find(char_array_4[i]);

            char_array_3[0] = ( char_array_4[0] << 2       ) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) +   char_array_4[3];

            for (i = 0; (i < 3); i++)
                ret += char_array_3[i];
            i = 0;
        }
    }

    if (i) {
        for (j = 0; j < i; j++)
            char_array_4[j] = base64_chars.find(char_array_4[j]);

        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);

        for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
    }

    return ret;
}

int EncryptionModule::private_decrypt(int flen, unsigned char* from, unsigned char* to, RSA* key, int padding) {

    int result = RSA_private_decrypt(flen, from, to, key, padding);
    return result;
}

void EncryptionModule::create_encrypted_file(char* encrypted, RSA* key_pair) {

    std::cout << "ENCRYPTEDDDDD: " << encrypted << std::endl;
    FILE* encrypted_file = fopen("encrypted_file.bin", "w");
    fwrite(encrypted, sizeof(*encrypted), RSA_size(key_pair), encrypted_file);
    fclose(encrypted_file);
}



