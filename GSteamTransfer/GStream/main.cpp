// GStream.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <thread>
#include "CSharedMem.h"
#include "GStreamSender.h"
#include "CStreamManager.h"

CStreamManager* pStreamManager;



void ThreadFuncInit()
{

}


int main() {

   
    SAppArgs args;
    args.ShareMemName = "MARTIVISSM";
    
    args.PortNo = 1045;
    args.IpAddr = "127.0.0.1";// "15.0.91.65";
    args.Width = 720;
    args.Height = 576;
    args.Size = args.Width * args.Height * 4;
    args.FrameRate = 30;

    pStreamManager = new CStreamManager();
    pStreamManager->Init(args);

    while (true) {
        
    }
   
    return 0;
}