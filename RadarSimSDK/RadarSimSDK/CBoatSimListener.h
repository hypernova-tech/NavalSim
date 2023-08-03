#pragma once
#include <thread>
#include "Lib/Types/Primitives.h"
#include "IDataIF.h"
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

	void ProcessData(INT8U* p_data, INT32U len);
};

