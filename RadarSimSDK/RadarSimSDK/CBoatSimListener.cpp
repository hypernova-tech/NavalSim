#include "CBoatSimListener.h"
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
    DWORD bytesRead;

    // Named Pipe oluşturma
    std::cout << "Waiting For Pipe Connection" << std::endl;
    while (true) {
        hPipe = CreateFile(
            TEXT("\\\\.\\pipe\\RadarSimPipe"),
            GENERIC_READ | GENERIC_WRITE,
            0,
            NULL,
            OPEN_EXISTING,
            0,
            NULL
        );

        if (hPipe == INVALID_HANDLE_VALUE) {
            
           
        }
        else {
            std::cout << "Uygulama A: Named Pipe olusturuldu. Bekleniyor..." << std::endl;
            break;
        }
    }


    

    // Uygulama B'den veri gelene kadar bekleyelim
    ConnectNamedPipe(hPipe, NULL);

    while (true){
        // Veriyi okuyalım
        ReadFile(hPipe, p_buffer, size, &bytesRead, NULL);

        if (bytesRead > 0) {
            ProcessData(p_buffer, size);
            std::cout << "Data Received" <<size<< std::endl;

        }
       

    
    }


    // Named Pipe kapatma
    CloseHandle(hPipe);
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
