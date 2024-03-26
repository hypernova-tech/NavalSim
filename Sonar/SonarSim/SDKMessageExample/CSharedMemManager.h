#pragma once
using namespace std;
#include <thread>
#include "types.h"
#include <string>
#include "CSharedMem.h"


struct SAppArgs{
	INT32U Size;
	string ShareMemName;
	string IpAddr;
	INT32S PortNo;
	INT32S Width;
	INT32S Height;
	INT32S FrameRate;
};

class CSharedMemManager
{

protected:
	std::thread *pThread;
	SAppArgs Args;
	CSharedMem* pSharedMem;
	

	
	virtual void ThreadFunc();
public:
	virtual void Init(const SAppArgs& args);
	void SetVerticalFOVDeg(float ang);
	void SetHorizontalFOVDeg(float ang);
};

