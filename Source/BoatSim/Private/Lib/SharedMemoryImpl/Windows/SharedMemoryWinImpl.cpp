// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/SharedMemoryImpl/Windows/SharedMemoryWinImpl.h"

HANDLE USharedMemoryWinImpl::CreateOrAccessSharedMemory(const FString sharedMemoryName, size_t size, size_t header_size)
{
    size += header_size;//sizeof(SSharedMemBufferHdr);
    // Create or open a file mapping object for the shared memory
    HANDLE hMapFile = CreateFileMapping(
        INVALID_HANDLE_VALUE,    // Use paging file - shared memory
        NULL,                    // Default security attributes
        PAGE_READWRITE,          // Read/write access to the shared memory
        0,                       // Maximum object size (high-order DWORD)
        size,                    // Maximum object size (low-order DWORD)
        *sharedMemoryName);       // Name of the mapping object

    if (hMapFile == NULL) {
        // Handle error (e.g., log it)
        return nullptr;
    }

    pMemPtr = MapViewOfFile(
        hMapFile,         // Handle to map object
        FILE_MAP_ALL_ACCESS,     // Read/write permission
        0,
        0,
        size);

    if (pMemPtr == NULL) {
        // Handle error
        CloseHandle(hMapFile); // Cleanup
        return nullptr;
    }

    return hMapFile;
}

void USharedMemoryWinImpl::InitConnection(void* p_args)
{
    Super::InitConnection(p_args);
    SSharedMemInitArgs* p_sm_arg = (SSharedMemInitArgs*)p_args;
    Handle = CreateOrAccessSharedMemory(p_sm_arg->Name, p_sm_arg->size, p_sm_arg->HeaderSize);

}

bool USharedMemoryWinImpl::SendData(const INT8U* p_bytes, INT32U count)
{
    Super::SendData(p_bytes, count);
#if 0
    SSharedMemBufferHdr* p_hdr = (SSharedMemBufferHdr*)pMemPtr;
    p_hdr->IsUpdated = 1;
    p_hdr->DataSize                 = count;
    p_hdr->Width                    = DataWidth;
    p_hdr->Height                   = DataHeight;
    p_hdr->ImageInfo.IsIr           = IsIR;
    p_hdr->ImageInfo.IsWhiteHot     = IsWhiteHot;
    p_hdr->ImageInfo.EnableDefog    = IsDefogEnabled;
    p_hdr->ImageInfo.DefogLevel     = DefogLevel;
    p_hdr->ImageInfo.IsICREnabled   = IsICREnabled;
    INT8U* p_data = (INT8U*)pMemPtr;
    memcpy(&p_data[sizeof(SSharedMemBufferHdr)], p_bytes, count);
#endif
    return true;
}
bool USharedMemoryWinImpl::SendData(const INT8U* p_hdr, INT32U header_size, const INT8U* p_bytes, INT32U count)
{
    Super::SendData(p_hdr, header_size, p_bytes, count);
    memcpy(pMemPtr, p_hdr, header_size);
    INT8U* p_data = (INT8U*)pMemPtr;
    memcpy(&p_data[header_size], p_bytes, count);
    return true;
}

void* USharedMemoryWinImpl::GetHeader()
{
    return pMemPtr;
}
