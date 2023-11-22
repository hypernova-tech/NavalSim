#pragma once
#include "Lib/Types/Primitives.h"


class IConnectionDataReceiver
{
public:
	IConnectionDataReceiver()
	{

	}
	virtual void OnReceivedConnectionData(void* connection, INT8U* p_data, INT32U count) = 0;

};