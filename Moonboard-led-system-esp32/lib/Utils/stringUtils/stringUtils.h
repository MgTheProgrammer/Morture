#pragma once
#include <string>

#include "stringUtilsConfig.h"

namespace stringUtils{
    /// @brief pads string to given width
    /// @param str the string to pad
    /// @param width the wanted width for the output
    /// @param paddingChar the character to pad extra space with, defaults to '0'
    void padStringTo(std::string &str, const size_t width, const char paddingChar = '0');

    /// @brief pads string to a multiple of block length, if the string length is already a multiple of the block will add a full padded block
    /// @param data string to pad
    /// @param blockLen the width of one block
    /// @param paddingChar the characters to pad with
    /// @return padded string
    std::string padToBlock(std::string data,size_t blockLen = BLOCK_SIZE,const char paddingChar = BLOCK_PADDING_CHAR);
    
    /// @brief trims from right characters matching the regex
    /// @param str string to trim
    /// @param regex the regex to match characters to
    /// @return the trimmed string
    std::string rtrim(std::string str, const char *regex = REMOVE_REGEX);

    /// @brief finds the index of the nth occurrence of a char in string
    /// @param str the string to search in
    /// @param chr the char that is being searched for
    /// @param n the occurrences which we want to find
    /// @return the index of the Nth occurrence of a given char, if no such occurrence exists returns -1
    int GetNthIndex(std::string str, char chr, int n);

    /// @brief will increase by one a int value in a string containing values separated by a delimeter, the value to be increased is specified by the index.
    /// \n For example: given the string "0;1;2;3;4;" and the index 1 will return "0;2;2;3;4;"
    /// @param data a string containing numbers separated by a constant delimeter, string must end with delimeter and start with value e.g. "0;1;2;10;6;"
    /// @param index the index of the value in the string to increase
    /// @param delimiter the char that separates the values
    /// @return a modified string in which the value in pos index is increased by 1.
    std::string increaseAtIndex(std::string data,int index,char delimiter);
}