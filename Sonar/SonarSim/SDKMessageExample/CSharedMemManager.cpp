#include "CSharedMemManager.h"
#include <iostream>

void CSharedMemManager::Init(const SAppArgs& args)
{
    Args = args;
    pThread = new thread(&CSharedMemManager::ThreadFunc, this);
    //pThread->join();

}

void CSharedMemManager::SetVerticalFOVDeg(float ang)
{
    auto p_hdr = pSharedMem->GetHeader();
    p_hdr->SonarSimVerticalFovDeg = ang;
    p_hdr->SonarSimIsUpdateData = true;
}

void CSharedMemManager::SetHorizontalFOVDeg(float ang)
{
    auto p_hdr = pSharedMem->GetHeader();
    p_hdr->SonarSimHorizontalFovDeg = ang;
    p_hdr->SonarSimIsUpdateData = true;
}

void CSharedMemManager::ThreadFunc()
{
    INT32U read_len = 0;
    INT32U raw_len = Args.Size;
    INT8U* p_raw = new INT8U[raw_len];
    pSharedMem = new CSharedMem();
    pSharedMem->Init(Args.Size, Args.ShareMemName.c_str());
  
    while (true) {
        if (pSharedMem->ReadData(p_raw, Args.Size, read_len)) {
            // process stream;
            auto hdr = pSharedMem->GetHeader();
            SFLSDataEntry* p_entries = (SFLSDataEntry*)pSharedMem->GetPayload();
            int cnt = hdr->DataSize / sizeof(SFLSDataEntry);

            auto start = std::chrono::high_resolution_clock::now();
            // HANDLE FRAME

            Args.pFlsIf->UpdateFlsData(*hdr, p_entries, cnt);
       
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

            /////std::cout << "Time taken by function: " << duration.count()/1000.0 << " ms" << std::endl;

        }
    }
}
