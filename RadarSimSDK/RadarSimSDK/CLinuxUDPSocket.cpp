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

	ReceivedDataLen = 0;
}

bool CLinuxUDPSocket::Create(SConnectionArgs* p_args) {

	return true;
}

bool CLinuxUDPSocket::SendData(INT8U* p_data, INT32U count, INT16U remote_port, bool broadcast) {

	if (broadcast) {
		//////
		
	}
	if (!simComm_->transmitPackage(p_data, count))
		std::cout << "CWinUDPSocket failed." << std::endl;
	else
		std::cout << "CWinUDPSocket sent." << std::endl;

	return true;
}

bool CLinuxUDPSocket::ReceivedData(INT8U* p_dest, INT32U dest_size, INT32U& read_count)
{
	bool ret = IsDataReceived;

	if (ret) {
		memcpy(p_dest, ReceivedDataBuff, ReceivedDataLen);
		read_count = ReceivedDataLen;
		IsDataReceived = false;
	}

	return ret;

}
void CLinuxUDPSocket::receivePackage(NetworkComm* networkComm, uint8_t data[], uint16_t len, const portIP& senderPortIP) {
	std::cout << "CWinUDPSocket mssg received" << std::endl;
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
