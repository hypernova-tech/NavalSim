#pragma once
#include "Lib/Types/Primitives.h"
#include <string>

struct SConnectionArgs {
	std::string ipAddress;
	unsigned short ReceivePort;
	unsigned short RemotePort;
};

class IConnection
{

protected:
	SConnectionArgs Args;
public:
	virtual bool Create(SConnectionArgs* p_args);
	virtual bool SendData(INT8U* p_data, INT32U count, INT16U remote_port, bool broadcast = false);
	virtual bool ReceivedData(INT8U* p_dest, INT32U dest_size,  INT32U& read_count);

	INT16U GetRemotePort()
	{
		return Args.RemotePort;
	}
};

