#pragma once
#include <Windows.h>
#include <string>
#include "types.h"
using namespace std;


struct SSharedMemBufferHdr {
	unsigned int IsUpdated;
	unsigned int DataSize;
	unsigned int SonarSimIsUpdateData;
	float SonarSimVerticalFovDeg; // from SonarSim to UnrealEngine
	float SonarSimHorizontalFovDeg; // from SonarSim to UnrealEngine
	float SonarSimIsSquelchEnabled; // from SonarSim to UnrealEngine
};

struct SFLSDataEntry
{
public:
	struct {
		INT32U IsGround : 1;
		INT32U Reserved : 31;
	}Info;
	FLOAT32 X;
	FLOAT32 Y;
	FLOAT32 Z;
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

