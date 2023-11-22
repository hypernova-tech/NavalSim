#if __linux__
#pragma once
#include "IConnection.h"

#include "../src/Network/NetworkComm.h"
#include "../src/Network/NetworkCommFactory.hpp"
#include "../src/Network/AbstractComm.hpp"

class CLinuxUDPSocket : public IConnection,
    public INetworkCommManager
{

private:
    //SOCKET m_socket;
    //struct sockaddr_in m_address;
    //WSADATA m_wsaData

    bool IsDataReceived;
    INT8U ReceivedDataBuff[2048];
    INT32S ReceivedDataLen;

public:

    CLinuxUDPSocket();

    NetworkComm* simComm_;
    void					receivePackage(NetworkComm* networkComm, uint8_t data[], uint16_t len, const portIP& senderPortIP);


    virtual bool Create(SConnectionArgs* p_args) override;
    virtual bool SendData(INT8U* p_data, INT32U count, INT16U remote_port) override;
    virtual bool ReceivedData(INT8U* p_dest, INT32U dest_size, INT32U& read_count);
    ~CLinuxUDPSocket();
};
#endif
