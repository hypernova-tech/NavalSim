#include "CBoatSimListener.h"
#include "Lib/Types/Primitives.h"
#include <iostream>
#if IS_WIN()
#include <Windows.h>
#endif
#include <thread>



void CBoatSimListener::Init()
{

    pPipeReadThread =  new std::thread(&CBoatSimListener::ThreadFunction, this);

    // Wait for the thread to finish its execution
    //pPipeReadThread->join();

   
}

void CBoatSimListener::ThreadFunction() {
   
  
    INT32U size = 1024 * 1024 * 4;
    INT8U *p_buffer = new INT8U[size];
    INT32U bytes_read = 0;

    while (true){
        // Veriyi okuyalım
        if (RadarChannelDataProvider[0] == nullptr) {
            continue;
        }
        bytes_read = 0;
        bool ret = RadarChannelDataProvider[0]->ReceivedData(p_buffer, size, bytes_read);
      
        if (ret) {
            if (bytes_read > 0) {
                ProcessData(RadarChannelDataProvider[0], p_buffer, bytes_read);
                std::cout << "Data Received " << bytes_read << std::endl;
            }
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

void CBoatSimListener::ProcessData(IConnection *p_if, INT8U* p_data, INT32U len)
{
    if (pDataIF != nullptr) {
        pDataIF->OnReceivedData(p_if, p_data, len);
    }

}

void CBoatSimListener::SetConnectionIF(INT32U ind, IConnection* p_val)
{
    RadarChannelDataProvider[ind] = p_val;
}
