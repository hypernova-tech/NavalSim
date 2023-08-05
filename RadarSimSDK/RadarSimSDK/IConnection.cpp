#include "IConnection.h"

bool IConnection::Create(SConnectionArgs* p_args)
{
	return false;
}

bool IConnection::SendData(INT8U* p_data, INT32U count, INT16U remote_port)
{
	return false;
}

bool IConnection::ReceivedData(INT8U* p_dest, INT32U dest_size, INT32U& read_count)
{
	return false;
}
