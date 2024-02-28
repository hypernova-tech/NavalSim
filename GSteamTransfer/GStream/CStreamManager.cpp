#include "CStreamManager.h"
#include <iostream>

void CStreamManager::Init(const SAppArgs& args)
{
    Args = args;
    pThread = new thread(&CStreamManager::ThreadFunc, this);
    //pThread->join();

}

void CStreamManager::ThreadFunc()
{
    INT32U read_len = 0;
    INT32U raw_len = Args.Size;
    INT8U* p_raw = new INT8U[raw_len];
    pSharedMem = new CSharedMem();
    pSharedMem->Init(Args.Size, Args.ShareMemName.c_str());
    pSender = new GStreamSender();
    pSender->Init(Args.ShareMemName, Args.IpAddr, Args.PortNo, Args.Width, Args.Height, Args.FrameRate);

    while (true) {
        if (pSharedMem->ReadData(p_raw, Args.Size, read_len)) {
            // process stream;
            auto hdr = pSharedMem->GetHeader();
            auto start = std::chrono::high_resolution_clock::now();
            pSender->OnReceivedFrame(hdr, p_raw, hdr->Width, hdr->Height);
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

            /////std::cout << "Time taken by function: " << duration.count()/1000.0 << " ms" << std::endl;

        }
    }
}
