#include "encryption.h"

namespace encryption
{

    void encryptBlock(const char *plainText, unsigned char *key, unsigned char *outputBuffer)
    {

        mbedtls_aes_context aes;

        mbedtls_aes_init(&aes);
        mbedtls_aes_setkey_enc(&aes, key, AES_KEY_SIZE * 8);
        mbedtls_aes_crypt_ecb(&aes, MBEDTLS_AES_ENCRYPT, (const unsigned char *)plainText, outputBuffer);
        mbedtls_aes_free(&aes);
    }

    void decryptBlock(unsigned char *cipherText, unsigned char *key, unsigned char *outputBuffer)
    {

        mbedtls_aes_context aes;

        mbedtls_aes_init(&aes);
        mbedtls_aes_setkey_dec(&aes, (const unsigned char *)key, AES_KEY_SIZE * 8);
        mbedtls_aes_crypt_ecb(&aes, MBEDTLS_AES_DECRYPT, (const unsigned char *)cipherText, outputBuffer);
        mbedtls_aes_free(&aes);
    }
    std::string encryptMsg(std::string plainText, unsigned char *key)
    {
        std::string encryptedText;
        plainText = stringUtils::padToBlock(plainText);
        unsigned char block[BLOCK_SIZE];
        char blockHex[BLOCK_SIZE * 2 + 1];
        for (size_t i = 0; i < plainText.length() / BLOCK_SIZE; i++)
        {
            encryptBlock(plainText.substr(i * BLOCK_SIZE, BLOCK_SIZE).c_str(), key, block);
            hexUtils::uint8ToHex(blockHex, sizeof(blockHex) / sizeof(char), block, sizeof(block) / sizeof(unsigned char));
            encryptedText.append(blockHex);
        }
        return encryptedText;
    }
    std::string decryptMsg(std::string cipherText, unsigned char *key)
    {
        std::string decipheredText;
        size_t cipherTextBytesLen = cipherText.length() / 2;
        unsigned char cipherTextBytes[cipherTextBytesLen];
        unsigned char cipheredBlock[BLOCK_SIZE];
        unsigned char decipheredBlock[BLOCK_SIZE];

        hexUtils::hexToUint8(cipherTextBytes, cipherTextBytesLen, cipherText.c_str());
        for (size_t i = 0; i < cipherTextBytesLen / BLOCK_SIZE; i++)
        {
            memcpy(cipheredBlock, &cipherTextBytes[i * BLOCK_SIZE], BLOCK_SIZE);
            decryptBlock(cipheredBlock, key, decipheredBlock);
            for (size_t j = 0; j < BLOCK_SIZE; j++)
            {
                decipheredText += (char)decipheredBlock[j];
            }
        }
        return decipheredText;
    }
    std::string hmac(std::string payload, unsigned char *key)
    {
        unsigned char hmacResult[HASH_SIZE];
        mbedtls_md_context_t ctx;

        const size_t payloadLength = payload.length();

        mbedtls_md_init(&ctx);
        mbedtls_md_setup(&ctx, mbedtls_md_info_from_type(MD_TYPE), 1);
        mbedtls_md_hmac_starts(&ctx, (const unsigned char *)key, HMAC_KEY_SIZE);
        mbedtls_md_hmac_update(&ctx, (const unsigned char *)payload.c_str(), payloadLength);
        mbedtls_md_hmac_finish(&ctx, hmacResult);
        mbedtls_md_free(&ctx);

        char hmacHex[HASH_SIZE * 2 + 1];
        hexUtils::uint8ToHex(hmacHex, sizeof(hmacHex) / sizeof(char), hmacResult, sizeof(hmacResult) / sizeof(unsigned char));
        return std::string(hmacHex);
    }
    std::string hash(std::string payload)
    {
        unsigned char hashResult[HASH_SIZE];
        char hashResultHex[HASH_SIZE * 2 + 1];
        mbedtls_md_context_t ctx;
        mbedtls_md_init(&ctx);
        mbedtls_md_setup(&ctx, mbedtls_md_info_from_type(MD_TYPE), 0);
        mbedtls_md_starts(&ctx);
        mbedtls_md_update(&ctx, (const unsigned char *)payload.c_str(), payload.length());
        mbedtls_md_finish(&ctx, hashResult);
        mbedtls_md_free(&ctx);
        hexUtils::uint8ToHex(hashResultHex, HASH_SIZE * 2 + 1, hashResult, HASH_SIZE);

        return std::string(hashResultHex);
    }
    std::string hashAndSalt(std::string password)
    {
        std::string salt = std::to_string(esp_random());
        password += salt;
        return hash(password) + salt;
    }
}