#pragma once
using namespace std;
#include <thread>
#include "types.h"
#include <string>
#include "CSharedMem.h"
#include "CFlsIF.h"


struct SAppArgs{
	INT32U Size;
	string ShareMemName;
	CFlsIF* pFlsIf;
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

