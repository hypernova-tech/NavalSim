#pragma once
#include <Windows.h>
#include <string>
#include "types.h"
#include "SharedMemTypes.h"

using namespace std;


class CSharedMem
{

public:
	void Init(int size, string name);
	wstring StringToWString(const std::string& s);
	bool ReadData(INT8U*p_data, INT32U dest_len, INT32U &read_len);
	INT8U* GetPayload();
	SFLSSharedMemBufferHdr* GetHeader();
protected:
	PVOID pMemPtr;

};

