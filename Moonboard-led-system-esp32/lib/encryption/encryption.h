#pragma once
#include <string>

#include "encryptionConfig.h"
#include "mbedtls/aes.h"
#include "mbedtls/md.h"

#include <hexUtils/hexUtils.h>
#include <stringUtils/stringUtils.h>

namespace encryption{
    /// @brief encrypts plain text that is in the width of one block, saves the encrypted data in a buffer
    /// @param plainText the data to encrypt (should be in the width of one block)
    /// @param key the key to encrypt the block with
    /// @param outputBuffer the buffer to store the encrypted data in
    void encryptBlock(const char * plainText, unsigned char * key, unsigned char * outputBuffer);
    
    /// @brief decrypts a block of encrypted text, saves decrypted text in a buffer
    /// @param cipherText a block of encrypted data
    /// @param key the key to decrypt the block with
    /// @param outputBuffer the buffer to store deciphered data in
    void decryptBlock(unsigned char * cipherText, unsigned char * key, unsigned char * outputBuffer);

    /// @brief takes text without specific length and encrypts it (saves the data in hex string)
    /// @param plainText the text to encrypt
    /// @param key the key to encrypt the text with
    /// @return a hex string representing the encrypted data
    std::string encryptMsg(std::string plainText, unsigned char *key);

    /// @brief takes a hex presentation of encrypted data and returns a deciphered string
    /// @param cipherText the text to decrypt
    /// @param key the key to decrypt the string with
    /// @return a plain text string with the deciphered data
    std::string decryptMsg(std::string cipherText, unsigned char *key);

    /// @brief calculates an hmac of the message with given key and creates a hex string from it 
    /// @param msg the message to create hmac from
    /// @param key the key to calculate hmac with
    /// @return a hex string representing the hmac
    std::string hmac(std::string msg, unsigned char* key);

    /// @brief calculates a hash of the payload and creates a hex string from it
    /// @param payload the content to calculate it's hash
    /// @return a hex string of the calculated hash
    std::string hash(std::string payload);
    
    /// @brief generates a random salt which will be added to the password to calculate hash with, returns the hash + the salt
    /// @param password the password to hash
    /// @return a string with the hashed password and afterwards the salt. (hash+salt)
    std::string hashAndSalt(std::string password);
}