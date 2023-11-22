#pragma once
#include <thread>

#include "Halo24SDK/include/MultiRadarClient.h"
#include "CBoatSimListener.h"
#include "CHalo24SimIF.h"

#if _WIN32
#include "CWinUDPSocket.h"
#else
#include "CLinuxUDPSocket.h"
#endif

#include "Halo24SDK/include/MultiRadarClient.h"
#include "CHost.h"
#include "RadarSimSdkConfig.h"



using namespace  Navico::Protocol;

enum EHostState
{
	GetRadars,
	UnlockRadars,
	InitImageClients,
	ConnectRadars,
	SetFastScanMode,
	SetScanerRPM,
	PowerOn,
	TransmitOn,
	SetGain,
	SetRange,
	SetSeaClutter,
	SetRain,
	SetSectorBlanking,
	TrackingInitTrackers,
	ConnectTrackers,
	AcquireTrack,
	CancelTrack,
	CancalAll,
	OwnShipNavigationData,
	PeriodicUpdate,
};


class CHost:public iRadarListObserver, public iUnlockStateObserver
{

private:
	CHost()
	{
	}
	void ThreadFunction();
	CBoatSimListener* pBoatSimListener;
	CHalo24IF* pHalo24SimSDK;
	IConnection* pRadarStreamConnection;
	static CHost* pInstance;

public:
	std::thread* pHostThread;

	void Init();

	// Inherited via iRadarListObserver
	virtual void UpdateRadarList(const char* pSerialNumber, eAction action) override;

	// Inherited via iUnlockStateObserver
	virtual void UpdateUnlockState(const char* pSerialNumber, int lockState) override;


	void StateMachine();

	static CHost* GetInstance();
	
};

