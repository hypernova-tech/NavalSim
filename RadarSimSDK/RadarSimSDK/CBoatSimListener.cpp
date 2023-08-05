﻿#include "CBoatSimListener.h"
#include <iostream>
#include <Windows.h>

#include <thread>
#include "Lib/Types/Primitives.h"


void CBoatSimListener::Init()
{

    pPipeReadThread =  new std::thread(&CBoatSimListener::ThreadFunction, this);

    // Wait for the thread to finish its execution
    pPipeReadThread->join();

   
}

void CBoatSimListener::ThreadFunction() {
   
    HANDLE hPipe;
    INT32U size = 1024 * 1024 * 4;
    INT8U *p_buffer = new INT8U[size];
    INT32U bytes_read = 0;

    while (true){
        // Veriyi okuyalım
        RadarChannelDataProvider[0]->ReceivedData(p_buffer, size, bytes_read);
      

        if (bytes_read > 0) {
            ProcessData(p_buffer, bytes_read);
            std::cout << "Data Received " << bytes_read << std::endl;
        }
    }
}

void CBoatSimListener::SetDataIF(IDataIF* p_val)
{
    pDataIF = p_val;
}

IDataIF* CBoatSimListener::GetDataIF()
{
    return pDataIF;
}

void CBoatSimListener::ProcessData(INT8U* p_data, INT32U len)
{
    if (pDataIF != nullptr) {
        pDataIF->OnReceivedData(p_data, len);
    }

}

void CBoatSimListener::SetConnectionIF(INT32U ind, IConnection* p_val)
{
    RadarChannelDataProvider[ind] = p_val;
}
