// GStream.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <thread>
#include "CSharedMem.h"
#include "GStreamSender.h"
#include "CStreamManager.h"
#include <list>

#define STANDALONE_EN 1


void ThreadFuncInit()
{

}

#if STANDALONE_EN > 0
CStreamManager* pStreamManager;
int main() {

   
    SAppArgs args;
    args.ShareMemName = "eo1sm";
    
    args.PortNo = 1045;
    args.IpAddr = "15.0.91.65"; // FPlatformTime::Seconds()
    args.Width = 720;
    args.Height = 576;
    args.Size = args.Width * args.Height * 4;
    args.FrameRate = 60;

    pStreamManager = new CStreamManager();
    pStreamManager->Init(args);

    while (true) {
        
    }
   
    return 0;
}

#else

struct SInstanceParams
{
    SAppArgs Args;
    CStreamManager* pStreamManager;
};

list< SInstanceParams*> InstancesParams;


// Declare the function with the export/import macro
extern "C" {  // Use extern "C" if you want to ensure C linkage (name mangling avoidance)
    __declspec(dllexport) int CreateInstance(char* sm_sname, char* listner_ip_addr, int listener_port, int width, int height, int frame_rate);
}



// Function definition
int CreateInstance(char* sm_sname_s, char* listner_ip_addr_s, int listener_port, int width, int height, int frame_rate) {
    // Implement your initialization logic here
    SInstanceParams* p_ins = new SInstanceParams();
    SAppArgs args;
    args.ShareMemName = string(sm_sname_s);

    args.PortNo = listener_port;
    args.IpAddr = string(listner_ip_addr_s);// "15.0.91.65";
    args.Width = width;
    args.Height = height;
    args.Size = args.Width * args.Height * 4;
    args.FrameRate = frame_rate;

    p_ins->pStreamManager = new CStreamManager();
    p_ins->pStreamManager->Init(args);

    InstancesParams.push_back(p_ins);
    return InstancesParams.size();

    //while (true);
}





#endif