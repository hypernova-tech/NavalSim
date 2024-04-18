
#include "CSharedMem.h"



wstring CSharedMem::StringToWString(const std::string& s)
{
    int len;
    int slength = (int)s.length() + 1;
    len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
    wchar_t* buf = new wchar_t[len];
    MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
    std::wstring r(buf);
    delete[] buf;
    return r;
}

bool CSharedMem::ReadData(INT8U* p_data, INT32U dest_len, INT32U& read_len)
{
    SFLSSharedMemBufferHdr* p_hdr = (SFLSSharedMemBufferHdr*)pMemPtr;
    
    if (p_hdr->IsUpdated) {
        if (dest_len >= p_hdr->DataSize) {
            memcpy(p_data, GetPayload(), p_hdr->DataSize);
            p_hdr->IsUpdated = 0;
            return true;
        }
    }
    return false;
}

INT8U* CSharedMem::GetPayload()
{
    INT8U* p_data = (INT8U*)pMemPtr;
    return &p_data[sizeof(SFLSSharedMemBufferHdr)];
}

SFLSSharedMemBufferHdr* CSharedMem::GetHeader()
{
    return (SFLSSharedMemBufferHdr*)pMemPtr;;
}

void CSharedMem::Init(int size, string name)
{
    wstring text = StringToWString(name);
    size += sizeof(SFLSSharedMemBufferHdr);

    HANDLE sharedMemory = CreateFileMapping(
        INVALID_HANDLE_VALUE,    // use paging file
        NULL,                    // default security
        PAGE_READWRITE,          // read/write access
        0,                       // maximum object size (high-order DWORD)
        size ,                  // maximum object size (low-order DWORD)
        text.c_str());

    pMemPtr = MapViewOfFile(
        sharedMemory,         // Handle to map object
        FILE_MAP_ALL_ACCESS,     // Read/write permission
        0,
        0,
        0);

    if (pMemPtr == NULL) {
        // Handle error
        CloseHandle(sharedMemory); // Cleanup
    }

}
