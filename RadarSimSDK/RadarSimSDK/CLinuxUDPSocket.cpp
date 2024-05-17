#if __linux__
#include <stdio.h>
#include <iostream>
#include <string.h>
#include "CLinuxUDPSocket.h"

//#include <ws2tcpip.h>
////#include <tchar.h>
////#pragma comment(lib, "ws2_32.lib")

CLinuxUDPSocket::CLinuxUDPSocket() {
	IsDataReceived = false;
	simComm_ = NetworkCommFactory::getInstance(std::string("SIM-COM"), enumNetworkComm::UDP);
	simComm_->initialize(stNetCommParam{ enumCommMode::TXRX, 1500,
										  portIP{13081, "15.0.83.83"},
										  portIP{13080, "15.0.83.84"} },
		this);
	simComm_->start();


	/*bcastComm_ = NetworkCommFactory::getInstance(std::string("BCAST-COM"), enumNetworkComm::UDP);
	bcastComm_->initialize(stNetCommParam{ enumCommMode::TXRX, 1500,
										  portIP{13081, "15.0.83.83"},
										  portIP{13080, "15.0.255.255"} },
		this);
	bcastComm_->start();*/

	ReceivedDataLen = 0;
}

bool CLinuxUDPSocket::Create(SConnectionArgs* p_args) {

	IConnection::Create(p_args);
	return true;
}

bool CLinuxUDPSocket::SendData(INT8U* p_data, INT32U count, INT16U remote_port, bool broadcast) {

	if (broadcast) {
		simComm_->transmitPackage(p_data, count, portIP{13080, "15.0.255.255"});
		return true;
	}else{
		if (!simComm_->transmitPackage(p_data, count))
			std::cout << "CWinUDPSocket failed." << std::endl;
		//else
			//std::cout << "CWinUDPSocket sent." << std::endl;

		return true;
	}

}

bool CLinuxUDPSocket::ReceivedData(INT8U* p_dest, INT32U dest_size, INT32U& read_count)
{

	return ReadPacket(p_dest, dest_size, read_count);

	bool ret = IsDataReceived;

	if (ret) {
		memcpy(p_dest, ReceivedDataBuff, ReceivedDataLen);
		read_count = ReceivedDataLen;
		IsDataReceived = false;
	}

	return ret;

}
void CLinuxUDPSocket::receivePackage(NetworkComm* networkComm, uint8_t data[], uint16_t len, const portIP& senderPortIP) {

	AddPacket(data, len);
	return;

	//std::cout << "CWinUDPSocket mssg received" << std::endl;
	IsDataReceived = true;
	memcpy((void*)ReceivedDataBuff, (void*)data, len);
	ReceivedDataLen = len;

}

CLinuxUDPSocket::~CLinuxUDPSocket()
{
#if false
	closesocket(m_socket);
	WSACleanup();
#endif
}
#endif
