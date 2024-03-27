#pragma once
#include "types.h"
using namespace std;


struct SSharedMemBufferHdr {
	unsigned int IsUpdated;
	unsigned int DataSize;
	unsigned int SonarSimIsUpdateData;
	float SonarSimVerticalFovDeg; // from SonarSim to UnrealEngine
	float SonarSimHorizontalFovDeg; // from SonarSim to UnrealEngine
	float SonarSimIsSquelchEnabled; // from SonarSim to UnrealEngine
	double PosXYZ[3];
	double PosLLH[3];
	double LookDir[3];

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