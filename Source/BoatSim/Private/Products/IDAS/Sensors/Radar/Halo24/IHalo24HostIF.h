#pragma once

#include "Halo24Types.h"

class IHalo24HostIF
{
public:
	IHalo24HostIF() {};
	virtual void OnRecievedCommand(SSIMSDKCommands *p_commands) = 0;
};