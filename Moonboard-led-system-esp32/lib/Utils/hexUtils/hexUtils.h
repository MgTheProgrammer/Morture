#pragma once
#include <Arduino.h>

namespace hexUtils{
    /// @brief takes uint8 array and converts it into hex string
    /// @param dest a char array to save hex string to
    /// @param dest_len length of char array
    /// @param src uint8 array
    /// @param src_len the length of the uint8 array
    /// @return false if dest len isn't big enough, true if preformed conversion
    bool uint8ToHex(char *dest, size_t dest_len, const uint8_t *src, size_t src_len);
    
    /// @brief takes a hex string and converts it to uint8 values, the values are stored into uint8t array
    /// @param dest an array to store the results
    /// @param dest_len length of destination array
    /// @param src the hex string to convert
    /// @return the number of characters converted
    size_t hexToUint8(uint8_t *dest, size_t dest_len, const char *src);
    
    /// @brief prints to serial monitor uint8 array as hex characters, also will print a label before the hex
    /// @param label a text to print to describe the data
    /// @param data the uint8 data to print
    /// @param len length of data
    void printHex(const char *label, uint8_t *data, int len);
}