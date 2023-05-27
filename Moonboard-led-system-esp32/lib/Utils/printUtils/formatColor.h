#include <string>
namespace formatColor
{
    /// @brief takes a string and formats it with escape characters to print it in red
    /// @param str the string to format
    /// @return a string in red
    std::string formatRed(std::string str);
    /// @brief takes a string and formats it with escape characters to print it in blue
    /// @param str the string to format
    /// @return a string in blue
    std::string formatBlue(std::string str);
    /// @brief takes a string and formats it with escape characters to print it in green
    /// @param str the string to format
    /// @return a string in green
    std::string formatGreen(std::string str);
    /// @brief takes a string and formats it with escape characters to print it in purple
    /// @param str the string to format
    /// @return a string in purple
    std::string formatPurple(std::string str);
}