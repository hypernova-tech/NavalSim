#include "CHost.h"
#include <iostream>


#include "Halo24SDK/include/ImageClient.h"
using namespace Navico::Protocol::NRP;


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


#if true

	tImageClient* ImageClients[2];


#endif

	

	int RadarCount = 0;

	EHostState HostState = EHostState::GetRadars;

	while (true) {

		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		tMultiRadarClient::GetInstance()->ExternalUpdate();
		


		auto curr_state = HostState;
		auto next_state = curr_state;

		switch (HostState) {
			case EHostState::GetRadars:
				{
					RadarCount = tMultiRadarClient::GetInstance()->GetRadars(radars, 2);

					if (RadarCount >= 1) {
						next_state = EHostState::UnlockRadars;
						break;
					}
				}

				break;

			case EHostState::UnlockRadars:

				{
					bool is_all_unlocked = true;

					for (INT32U i = 0; i < RadarCount; i++) {
				
						auto* p_radar = tMultiRadarClient::GetInstance()->FindRadar(radars[i]);

						if (p_radar && !p_radar->GetIsUnlocked()) {
							auto ret = tMultiRadarClient::GetInstance()->UnlockRadar(radars[i], (const uint8_t*)radars_unlockkey[i], strlen(radars_unlockkey[i]), 50);
							if (ret) {
								cout << "Radar Unclocked " << string(radars[i]) << endl;
							}
							else {
								is_all_unlocked = false;
							}

						}

					}

					if (is_all_unlocked) {
						next_state = EHostState::InitImageClients;
					}
				}
			case EHostState::InitImageClients:
				{
					for (int i = 0; i < 2; i++) {
						ImageClients[i] = new tImageClient();
						auto* p_radar = tMultiRadarClient::GetInstance()->FindRadar(radars[i]);
						if (p_radar != nullptr) {
							p_radar->pImageClient = ImageClients[i];
						}
					}

					next_state = EHostState::ConnectRadars;
				}
				break;
			case EHostState::ConnectRadars:
				{
					
					for (INT32U i = 0; i < RadarCount; i++) {
						auto* p_radar = tMultiRadarClient::GetInstance()->FindRadar(radars[i]);

						for (int stream = 0; stream < 2; stream++) {
							if (p_radar && !p_radar->GetIsImageStreamConnected(stream)) {
								auto ret = p_radar->pImageClient->Connect(p_radar->Serial.c_str(), stream);
								if (ret == 0) {
									cout << "connected to stream sent " << string(radars[i]) <<"stream"<< stream<< endl;
								}
								std::this_thread::sleep_for(std::chrono::milliseconds(10));
								

							}
						}

					}


					bool is_all_radars_connected = true;
					int success_cnt = 0;

					for (INT32U i = 0; i < RadarCount; i++) {
						auto* p_radar = tMultiRadarClient::GetInstance()->FindRadar(radars[i]);

						for (int stream = 0; stream < 2; stream++) {
							if (p_radar){
								if (p_radar->GetIsImageStreamConnected(stream)) {
									success_cnt++;
								}
								else {
									is_all_radars_connected = false;
								}
							}
						}
					}

					if (is_all_radars_connected && success_cnt > 0) {
						cout << "All Stream Connected " << endl;
						next_state = EHostState::PeriodicUpdate;
					}
				}
				break;


			case EHostState::PeriodicUpdate:
				break;
		}

		HostState = next_state;
	

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

CHost* CHost::GetInstance()
{
	
	if (pInstance == nullptr) {
		pInstance = new CHost();
	}

	return pInstance;

}
