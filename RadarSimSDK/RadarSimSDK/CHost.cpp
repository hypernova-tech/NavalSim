#include "CHost.h"
#include <iostream>

CHost* CHost::pInstance = nullptr;

void CHost::Init()
{
	pHostThread = new std::thread(&CHost::ThreadFunction, this);


	pHalo24SimSDK = new CHalo24IF();
	pBoatSimListener = new CBoatSimListener();

	pRadarStreamConnection = new CWinUDPSocket();
	SConnectionArgs args = { "127.0.0.1",4143, 4142 };
	pRadarStreamConnection->Create(&args);


	pBoatSimListener->SetDataIF(pHalo24SimSDK);
	pBoatSimListener->SetConnectionIF(0, pRadarStreamConnection);
	pBoatSimListener->Init();


	pHalo24SimSDK->AddSDK(pRadarStreamConnection);

	




}


void CHost::ThreadFunction()
{

	tMultiRadarClient::GetInstance()->AddRadarListObserver(this);
	tMultiRadarClient::GetInstance()->AddUnlockStateObserver(this);
	char radars[2][MAX_SERIALNUMBER_SIZE];
	char radars_unlockkey[2][MAX_UNLOCKKEY_SIZE];

	memset(radars_unlockkey, 0, sizeof(radars_unlockkey));

	strcpy(radars_unlockkey[0], "123456789");
	strcpy(radars_unlockkey[1], "987654321");


	

	int RadarCount = 0;

	tMultiRadarClient::GetInstance()->Connect();

	while (true) {

		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		tMultiRadarClient::GetInstance()->ExternalUpdate();
		StateMachine();

	
		while (true) {
			std::this_thread::sleep_for(std::chrono::milliseconds(10));

			RadarCount = tMultiRadarClient::GetInstance()->GetRadars(radars, 2);

			if (RadarCount >= 1) {
				break;
			}
		}

		for (INT32U i = 0; i < RadarCount; i++) {
			auto *p_radar = tMultiRadarClient::GetInstance()->FindRadar(radars[i]);
			
			if (p_radar && !p_radar->GetIsUnlocked()) {
				auto ret = tMultiRadarClient::GetInstance()->UnlockRadar(radars[i], (const uint8_t * )radars_unlockkey[i], strlen(radars_unlockkey[i]), 50);
				if (ret) {
					cout << "Radar Unclocked " << string(radars[i]) << endl;
				}

			}
			
		}

	}
}
void CHost::UpdateRadarList(const char* pSerialNumber, eAction action)
{
	cout << "UpdateRadarList serial: " << pSerialNumber << endl;

}
void CHost::UpdateUnlockState(const char* pSerialNumber, int lockState)
{
	cout << "UpdateUnlockState serial: " << pSerialNumber << endl;
}

void CHost::StateMachine()
{

}
