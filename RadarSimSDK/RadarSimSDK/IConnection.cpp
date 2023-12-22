#include "IConnection.h"
#include <string.h>


bool IConnection::Create(SConnectionArgs* p_args)
{
	Args = *p_args;
	pPacketBuff = new SUDPPack[BufferCount];



	return false;
}

bool IConnection::SendData(INT8U* p_data, INT32U count, INT16U remote_port, bool broadcast)
{
	return false;
}

bool IConnection::ReceivedData(INT8U* p_dest, INT32U dest_size, INT32U& read_count)
{
	return false;
}
void IConnection::AddPacket(void *p_data, INT32U len)
{
	mtx.lock();
	
	auto p_curr = &pPacketBuff[WriteInd];
	memcpy((void*)p_curr->Data, p_data,len);
	p_curr->DataLen = len;
	WriteInd++;
	WriteInd %= BufferCount;
	mtx.unlock();
}
bool IConnection::ReadPacket(INT8U* p_dest, INT32U dest_size, INT32U& read_count)
{
	mtx.lock();
	if(ReadInd == WriteInd){
		mtx.unlock();
		return false;
	}

	auto p_curr = &pPacketBuff[ReadInd];
	memcpy((void*)p_dest, p_curr->Data, p_curr->DataLen);
	read_count = p_curr->DataLen;
	ReadInd++;
	ReadInd %= BufferCount;
	mtx.unlock();
	return true;
}
