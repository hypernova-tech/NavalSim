#pragma once

#include "Halo24Types.h"
#include <Lib/Sensor/GenericRadarCommProtocolF/GenericRadarCommProtocolIF.h>

class IHalo24HostIF:public IRadarHostIF
{
public:
	IHalo24HostIF() {};
	virtual void OnRecievedMessage(void*p_commands) = 0; //SRadarSimSDKvoidPacket
	virtual char* GetSerial() = 0;
	virtual double GetGain() = 0;
	virtual double GetRainClutterLevel() = 0;
	virtual double GetSeaClutterLevel() = 0;
	virtual AActor* GetOwningActor()  = 0;

};