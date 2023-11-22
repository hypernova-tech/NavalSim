#pragma once
using namespace std;
#include <vector>
#include "Lib/Types/Primitives.h"

class CUtil
{
public:
    static vector<INT8U> HexStrToByteArray(const char* p_hex_str);
};

