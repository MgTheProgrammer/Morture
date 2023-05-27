#include "formatColor.h"
namespace formatColor
{
    std::string formatRed(std::string value)
    {
        return "\033[91m" + value + "\033[00m";
    }
    std::string formatBlue(std::string value)
    {
        return "\033[96m" + value + "\033[00m";
    }
    std::string formatGreen(std::string value)
    {
        return "\033[92m" + value + "\033[00m";
    }
    std::string formatPurple(std::string value)
    {
        return "\033[35m" + value + "\033[00m";
    }
}