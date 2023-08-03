#pragma once
#include "Lib/Types/Primitives.h"
class IDataIF
{

public:
	virtual void OnReceivedData(INT8U* p_data, INT32U len);
};

