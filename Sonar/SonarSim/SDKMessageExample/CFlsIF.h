#pragma once
#include "SharedMemTypes.h"
#include <string>
#include "zeromq/include/zmq.h"
#include "zeromq/include/zmq.hpp"


class CFlsIF
{

public:

	CFlsIF();
	static const std::string TargetDataPort;
	static const std::string FovPort;
	static const std::string SquelchPort;
	static const std::string AutoSquelchPort;
	static const std::string BottomDetectionPort;
	static const std::string GetProcessingSettingPort;
	static const std::string PublisProcessingSettinghPort;

	void  SetHeader(SFLSSharedMemBufferHdr *p_val);
	SFLSSharedMemBufferHdr* GetHeader();

	void UpdateFlsTargetData(SFLSSharedMemBufferHdr hdr, SFLSDataEntry* p_entries, INT32U count);
	void PublishProcessorSettingData();
	void SetRangeMeter(double val);
	double GetRangeMeter();


	void SetFLSOn(bool val);
	bool GetFLSOn();

	void SetAutoSquelch(bool val);
	bool GetAutoSquelch();

	void SetBottomDetection(bool val);
	bool GetBottomDetection();

	void SetInWaterSquelch(double val);
	double GetInWaterSquelch();




protected:
	SFLSSharedMemBufferHdr* pHdr;
	zmq::socket_t *pTargetDataPublisherSoc = nullptr;
	zmq::socket_t* pProcessorSettingsSoc = nullptr;
	bool IsHostDataDirty = true;
};

