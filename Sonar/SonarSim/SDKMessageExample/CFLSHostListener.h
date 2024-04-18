#pragma once

using namespace std;
#include <thread>
#include "types.h"
#include <string>
#include "CFlsIF.h"


class CFLSHostListener
{
protected:

	std::thread* pThread;

	virtual void ThreadFunc();
	void ZeroMQProtoServer();
	CFlsIF* pFlsIf;


public:
	virtual void Init(CFlsIF *p_if);
};

