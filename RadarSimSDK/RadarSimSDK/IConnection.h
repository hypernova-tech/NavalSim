#pragma once
#include "Lib/Types/Primitives.h"
#include <string>
#include <list>
#include <mutex>
#include <thread>
using namespace std;

struct SConnectionArgs {
	std::string ipAddress;
	unsigned short ReceivePort;
	unsigned short RemotePort;
};

struct SUDPPack{
	INT32U DataLen;
	INT8U Data[2048];

};

class IConnection
{

protected:
	SConnectionArgs Args;
	SUDPPack *pPacketBuff;
	std::mutex mtx;
public:


	virtual bool Create(SConnectionArgs* p_args);
	virtual bool SendData(INT8U* p_data, INT32U count, INT16U remote_port, bool broadcast = false);
	virtual bool ReceivedData(INT8U* p_dest, INT32U dest_size,  INT32U& read_count);

	INT16U GetRemotePort()
	{
		return Args.RemotePort;
	}
	void AddPacket(void *p_data, INT32U len);
	bool ReadPacket(INT8U* p_dest, INT32U dest_size, INT32U& read_count);

	INT32S WriteInd = 0;
	INT32S ReadInd = 0;
	INT32S BufferCount = 512;
};

