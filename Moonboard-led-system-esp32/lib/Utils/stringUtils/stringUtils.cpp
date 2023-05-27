#include "stringUtils.h"
namespace stringUtils
{
    std::string padToBlock(std::string data, size_t blockLen, const char paddingChar)
    {
        int paddingSize = blockLen - (data.length() % blockLen); // Calculate padding size
        if (paddingSize == 0)
        {
            paddingSize = blockLen; // If data size is a multiple of block size, pad with a full block of whitespace
        }
        for (int i = 0; i < paddingSize; i++)
        {
            data += paddingChar;
        }
        return data;
    }

    std::string rtrim(std::string str, const char *regex)
    {
        str.erase(str.find_last_not_of(regex) + 1);
        return str;
    }

    void padStringTo(std::string &str, const size_t width, const char paddingChar)
    {
        if (width > str.size())
            str.insert(0, width - str.size(), paddingChar);
    }

    int GetNthIndex(std::string str, char chr, int n)
    {
        int count = 0;
        for (int i = 0; i < str.length(); i++)
        {
            if (str[i] == chr)
            {
                count++;
                if (count == n)
                {
                    return i;
                }
            }
        }
        return -1;
    }
    std::string increaseAtIndex(std::string data,int index,char delimiter){
    int pos = GetNthIndex(data,delimiter,index);
    int posNext = GetNthIndex(data,delimiter,index+1);
    pos +=1;

    int count = stoi(data.substr(pos,posNext-pos));
    count++;
    data.replace(pos,posNext-pos,std::to_string(count));
    return data;
}
}