#pragma once

#include "Halo24Types.h"

class IHalo24HostIF
{
public:
	IHalo24HostIF() {};
	virtual void OnRecievedMessage(SRadarSimSDKPacket*p_commands) = 0;
	virtual char* GetSerial() = 0;
	virtual double GetGain() = 0;
	virtual double GetRainClutterLevel() = 0;
	virtual double GetSeaClutterLevel() = 0;
	virtual AActor* GetOwningActor()  = 0;

};