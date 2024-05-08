// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/Connection/Serial/SerialCommIF.h"
#include "Windows.h"
void USerialCommIF::BeginPlay()
{
	Super::BeginPlay();

	//RegisterConnectionInfo(0, pUDPConnection->GetConnectionInfo(), pUDPConnection);

    hSerialHandle = CreateFile(L"COM3",
        GENERIC_READ | GENERIC_WRITE,
        0,
        0,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        0);

    if (hSerialHandle == INVALID_HANDLE_VALUE) {
        // Handle errors
        return;
    }


    DCB dcbSerialParams = { 0 };
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    if (!GetCommState(hSerialHandle, &dcbSerialParams)) {
        // Error getting current serial port state
        CloseHandle(hSerialHandle);
        return;
    }

    dcbSerialParams.BaudRate = 2000000;  // Set the baud rate to 1,228,800 bps
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;

    if (!SetCommState(hSerialHandle, &dcbSerialParams)) {
        // Error setting serial port state
        CloseHandle(hSerialHandle);
        return;
    }
}
#pragma pack(push, 1)
struct SCanControllerPacket
{
    INT8U Header1;
    INT8U Header2;
    INT8U Type;
    INT8U FrameType;
    INT8U FrameFormat;
    
    INT32U FrameId;
       
 
    INT8U Length;
    INT8U Data[8];
    INT8U Reserved;
    INT8U Checksum;

public:
    void SetData(INT8U frame_id, INT8U* p_data, INT8U len) {
        memset(this, 0, sizeof(SCanControllerPacket));
        Header1 = 0xaa;
        Header2 = 0x55;
        FrameType = 0x1;
        Type = 0x1;
        FrameFormat = 0x1;
        FrameId = frame_id;
        Length = len;
        memcpy(Data, p_data, len);
        Reserved = 0;


        Checksum = Type + FrameType + FrameFormat + FrameId + Length;

        for (int i = 0; i < 8; i++) {
            Checksum += Data[i];
        }
        

    }
};
#pragma pack(pop)



bool USerialCommIF::SendData(const INT8U* p_bytes, INT32U count)
{
    DWORD bytesSent = 0;
    char data[] = { '1','2','3' ,'4' ,'5' ,'6' ,'7' ,'8' };
    SCanControllerPacket pack;
    pack.SetData(1, (INT8U*)data, sizeof(data));

    

    if (!WriteFile(hSerialHandle, &pack, sizeof(SCanControllerPacket), &bytesSent, NULL)) {
        // Error writing
        //CloseHandle(hSerialHandle);
        return false;
    }

    return true;
}
