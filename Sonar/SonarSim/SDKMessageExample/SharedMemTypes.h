#pragma once
#include "types.h"
using namespace std;

struct SFLSProcessorSettings
{
	unsigned int IsFlsOn;
	double RangeMeter;
	unsigned int BottomDetectionEnabled;
	unsigned int IsAutoSquelchEnabled;
	double SquelchSensitivity;
};

struct SFLSSharedMemBufferHdr {
	unsigned int IsUpdated;
	unsigned int DataSize;
	unsigned int SonarSimIsUpdateData;//host to sim update
	double PosXYZ[3];
	double PosLLH[3];
	double LookDir[3];
	SFLSProcessorSettings FromHostToSim;
	SFLSProcessorSettings FromSimToHost;

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