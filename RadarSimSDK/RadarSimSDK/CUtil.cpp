#include "CUtil.h"
#include <string>
#include <iostream>

std::vector<INT8U> CUtil::HexStrToByteArray(const char* p_hex_str)
{
    std::vector<INT8U> byteArray;
    std::string hexStr(p_hex_str);

    // Check if the string has an even number of characters
    if (hexStr.length() % 2 != 0)
    {
        std::cerr << "Invalid hex string length." << std::endl;
        return byteArray;
    }

    for (size_t i = 0; i < hexStr.length(); i += 2)
    {
        std::string byteString = hexStr.substr(i, 2);
        INT8U byte = static_cast<INT8U>(std::strtoul(byteString.c_str(), nullptr, 16));
        byteArray.push_back(byte);
    }

    return byteArray;
}
