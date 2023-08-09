#pragma once
#include "Lib/Types/Primitives.h"

struct SRadarTrack3DData {

};

struct SRadarSimSDKPacket
{
	INT32U PacketId;
	INT32U PacketCount;
	INT32U Data[1024];

};

enum ESimSDKDataIDS :INT32U
{
	PowerOn,
	ScanOn,
	TransmitOn,
	PowerOff,
	Serials,
	UnlockKeys

};

struct SSIMSDKCommands
{
	ESimSDKDataIDS DataID;

};

