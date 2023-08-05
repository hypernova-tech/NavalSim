#include <stdio.h>
#include <iostream>
#include "CWinUDPSocket.h"
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

CWinUDPSocket::CWinUDPSocket() : m_socket(INVALID_SOCKET) {}

bool CWinUDPSocket::Create(SConnectionArgs* p_args) {
    SConnectionArgs* connArgs = p_args;

    if (WSAStartup(MAKEWORD(2, 2), &m_wsaData) != 0) {
        std::cerr << "WSAStartup failed: " << WSAGetLastError() << std::endl;
        return false;
    }

    m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (m_socket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return false;
    }

    m_address.sin_family = AF_INET;
    m_address.sin_port = htons(connArgs->ReceivePort);
    if (inet_pton(AF_INET, connArgs->ipAddress.c_str(), &m_address.sin_addr) <= 0) {
        std::cerr << "Invalid address/ Address not supported \n";
        return false;
    }

    return true;
}

bool CWinUDPSocket::SendData(INT8U* p_data, INT32U count, INT16U remote_port) {
    struct sockaddr_in dest_address;
    dest_address.sin_family = AF_INET;
    dest_address.sin_port = htons(remote_port);
    dest_address.sin_addr = m_address.sin_addr; // Use the same IP as before.

    int sendResult = sendto(m_socket, reinterpret_cast<const char*>(p_data), count, 0,
        (struct sockaddr*)&dest_address, sizeof(dest_address));
    if (sendResult == SOCKET_ERROR) {
        std::cerr << "Data send failed: " << WSAGetLastError() << std::endl;
        return false;
    }
    return true;
}

bool CWinUDPSocket::ReceivedData(INT8U* p_dest, INT32U dest_size, INT32U& read_count) {
    int addrLength = sizeof(m_address);
    read_count = recvfrom(m_socket, reinterpret_cast<char*>(p_dest), dest_size, 0,
        (struct sockaddr*)&m_address, &addrLength);
    if (read_count == SOCKET_ERROR) {
        std::cerr << "Data receive failed: " << WSAGetLastError() << std::endl;
        return false;
    }

    return true;
}

CWinUDPSocket::~CWinUDPSocket() {
    closesocket(m_socket);
    WSACleanup();
}