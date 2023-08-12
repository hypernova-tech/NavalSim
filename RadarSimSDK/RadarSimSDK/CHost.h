#pragma once
#include <thread>

#include "Halo24SDK/include/MultiRadarClient.h"
#include "CBoatSimListener.h"
#include "CHalo24SimIF.h"
#include "CWinUDPSocket.h"
#include "Halo24SDK/include/MultiRadarClient.h"
#include "CHost.h"



using namespace  Navico::Protocol;


class CHost:public iRadarListObserver, public iUnlockStateObserver
{

private:
	void ThreadFunction();
	CBoatSimListener* pBoatSimListener;
	CHalo24IF* pHalo24SimSDK;
	IConnection* pRadarStreamConnection;
	CHost* pHost;
public:
	std::thread* pHostThread;

	void Init();

	// Inherited via iRadarListObserver
	virtual void UpdateRadarList(const char* pSerialNumber, eAction action) override;

	// Inherited via iUnlockStateObserver
	virtual void UpdateUnlockState(const char* pSerialNumber, int lockState) override;


	void StateMachine();
};

