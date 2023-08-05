#pragma once
#include <thread>
#include "Lib/Types/Primitives.h"
#include "IDataIF.h"
#include "IConnection.h"
class CBoatSimListener
{
public:
	void Init();
	void ThreadFunction();

	void SetDataIF(IDataIF* p_val);
	IDataIF* GetDataIF();

protected:

	std::thread *pPipeReadThread;
	IDataIF* pDataIF;
	IConnection* RadarChannelDataProvider[1];



	void ProcessData(INT8U* p_data, INT32U len);
	void SetConnectionIF(INT32U ind, IConnection* p_val);
};

