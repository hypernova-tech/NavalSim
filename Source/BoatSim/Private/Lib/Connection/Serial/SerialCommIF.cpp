// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/Connection/Serial/SerialCommIF.h"
#define EN_WIN 1

#if EN_WIN > 0
#include "windows.h"
#endif
void USerialCommIF::BeginPlay()
{

	Super::BeginPlay();
#if EN_WIN > 0
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

#endif
}




bool USerialCommIF::SendData(const INT8U* p_bytes, INT32U count)
{
#if EN_WIN > 0
    DWORD bytesSent = 0;
     

    if (!WriteFile(hSerialHandle, p_bytes, count, &bytesSent, NULL)) {
        // Error writing
        //CloseHandle(hSerialHandle);
        return false;
    }

#endif

    return true;
}
