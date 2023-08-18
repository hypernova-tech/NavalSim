#pragma once

#include "Halo24Types.h"

class IHalo24HostIF
{
public:
	IHalo24HostIF() {};
	virtual void OnRecievedMessage(SRadarSimSDKPacket*p_commands) = 0;
	virtual char* GetSerial() = 0;
};