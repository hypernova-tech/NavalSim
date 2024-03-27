#pragma once
#include "SharedMemTypes.h"
class CFlsIF
{

public:

	void UpdateFlsData(SSharedMemBufferHdr hdr, SFLSDataEntry* p_entries, INT32U count);
};

