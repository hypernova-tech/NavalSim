#pragma once
#include <Lib/HostIF/HostIF.h>

class ISonarHostIF:public IHostIF
{
public:
	ISonarHostIF() {};
	

	virtual bool   GetBottomDetectEnabled() = 0;
	virtual double GetSquelchSensitivity() = 0;
	virtual bool   GetAutoSquelchEnabled() = 0;


};

