#if _WIN32
#include <stdio.h>
#include <iostream>
#include "CWinUDPSocket.h"
#include <ws2tcpip.h>
#include <tchar.h>
#pragma comment(lib, "ws2_32.lib")

CWinUDPSocket::CWinUDPSocket() : m_socket(INVALID_SOCKET) {}

bool CWinUDPSocket::Create(SConnectionArgs* p_args) {

    IConnection::Create(p_args);
    SConnectionArgs* connArgs = p_args;

  
 
    const char* hello = "Hello from server";

    int iResult;
    WSADATA wsaData;
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        std::cout << "WSAStartup failed: " << iResult << std::endl;
        return false;
    }

 
    m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    // Creating socket file descriptor
    if ((m_socket) == INVALID_SOCKET) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

 
    memset(&m_address, 0, sizeof(m_address));

    // Filling server information
    m_address.sin_family = AF_INET; // IPv4
    m_address.sin_addr.s_addr = INADDR_ANY;
    m_address.sin_port = htons(p_args->ReceivePort);
   

    // Bind the socket with the server address
    if (bind(m_socket, (struct sockaddr*)&m_address, sizeof(m_address)) == SOCKET_ERROR)
    {
        printf("bind failed: %d", WSAGetLastError());
        exit(EXIT_FAILURE);
    }

  
    return true;
}

bool CWinUDPSocket::SendData(INT8U* p_data, INT32U count, INT16U remote_port) {
    struct sockaddr_in dest_address;
    dest_address.sin_family = AF_INET;
    dest_address.sin_port = htons(remote_port);
    dest_address.sin_addr = m_address.sin_addr; // Use the same IP as before.

#if false
    int sendResult = sendto(m_socket, reinterpret_cast<const char*>(p_data), count, 0,
        (struct sockaddr*)&dest_address, sizeof(dest_address));
    if (sendResult == SOCKET_ERROR) {
        std::cerr << "Data send failed: " << WSAGetLastError() << std::endl;
        return false;
    }
#endif


    // send the message
    if (sendto(m_socket, (const char*)p_data, count, 0, (sockaddr*)&dest_address, sizeof(sockaddr_in)) == SOCKET_ERROR)
    {
        printf("sendto() failed with error code: %d", WSAGetLastError());
        return 3;
    }


    return true;
}

bool CWinUDPSocket::ReceivedData(INT8U* p_dest, INT32U dest_size, INT32U& read_count) {
    int addrLength = sizeof(m_address);



    int clientAddressSize = sizeof(m_address);
    int recv_len = recvfrom(m_socket, (char*)p_dest, dest_size, 0, (struct sockaddr*)&m_address, &clientAddressSize);

    if (read_count == SOCKET_ERROR) {
        std::cerr << "Data receive failed: " << WSAGetLastError() << std::endl;
        return false;
    }
    else {
        read_count = recv_len;
        std::cout << (char*)p_dest;
    }
    return true;
}

CWinUDPSocket::~CWinUDPSocket() {
    closesocket(m_socket);
    WSACleanup();
}

#endif