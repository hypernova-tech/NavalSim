#pragma once
#include "SharedMemTypes.h"
#include <string>
class CFlsIF
{

public:
	static const std::string TargetDataPort;
	static const std::string FovPort;
	static const std::string SquelchPort;
	static const std::string AutoSquelchPort;
	static const std::string BottomDetectionPort;
	

	void UpdateFlsTargetData(SSharedMemBufferHdr hdr, SFLSDataEntry* p_entries, INT32U count);
};

