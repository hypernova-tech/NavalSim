#pragma once
#include "IConnection.h"
#include <winsock2.h>
class CWinUDPSocket : public IConnection
{

private:
    SOCKET m_socket;
    struct sockaddr_in m_address;
    WSADATA m_wsaData;

public:
    CWinUDPSocket();
    virtual bool Create(SConnectionArgs* p_args) override;
    virtual bool SendData(INT8U* p_data, INT32U count, INT16U remote_port) override;
    virtual bool ReceivedData(INT8U* p_dest, INT32U dest_size, INT32U& read_count) override;
    ~CWinUDPSocket();
};

