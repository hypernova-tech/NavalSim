#pragma once
#include <Windows.h>
#include <string>
#include "types.h"
using namespace std;


struct SSharedMemBufferHdr {
	unsigned int IsUpdated;
	unsigned int DataSize;
	unsigned int Height;
	unsigned int Width;
	float ContrastLevel;
	float BrightnessLevel;
	struct {
		unsigned int IsIr : 1;
		unsigned int IsWhiteHot : 1;
		unsigned int EnableDefog : 1;
		unsigned int DefogLevel : 2;
		unsigned int IsICREnabled : 1;
		unsigned int Reserved : 26;
	}ImageInfo;

};

class CSharedMem
{

public:
	void Init(int size, string name);
	wstring StringToWString(const std::string& s);
	bool ReadData(INT8U*p_data, INT32U dest_len, INT32U &read_len);
	INT8U* GetPayload();
	SSharedMemBufferHdr* GetHeader();
protected:
	PVOID pMemPtr;

};

