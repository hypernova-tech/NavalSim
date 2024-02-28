#pragma once
#include "types.h"
class CMath
{

public:
	inline static INT8U ClampHighResPixel(INT16S val, INT16S min_val, INT16S max_val)
	{
        
        if (val <= min_val) {
            return val;
        }
        else if (val >= max_val) {
            return max_val;
        }

        return val;
        
	}

    inline static INT16S Lerp(float  tf, INT16S min_val, INT16S max_val)
    {
        if (tf < 0) {
            tf = 0;
        } if (tf > 1) {
            tf = 1;
        }

        INT16S val = min_val + (max_val - min_val) * tf;
        return val;

    }

    template<typename T>
    inline static T Clamp(T value, T min, T max) {
        return (value < min) ? min : (value > max) ? max : value;
    }
};

