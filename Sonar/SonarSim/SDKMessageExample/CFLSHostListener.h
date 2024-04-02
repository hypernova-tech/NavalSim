#pragma once

using namespace std;
#include <thread>
#include "types.h"
#include <string>


class CFLSHostListener
{
protected:

	std::thread* pThread;

	virtual void ThreadFunc();
	void ZeroMQProtoServer();


public:
	virtual void Init();
};

