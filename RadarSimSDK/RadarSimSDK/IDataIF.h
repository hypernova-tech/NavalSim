#pragma once
#include "Lib/Types/Primitives.h"
class IDataIF
{

public:
	virtual void OnReceivedData(void *p_if, INT8U* p_data, INT32U len);
};

