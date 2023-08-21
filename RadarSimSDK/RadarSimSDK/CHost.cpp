#include "CHost.h"
#include <iostream>


#include "Halo24SDK/include/ImageClient.h"
#include "Halo24SDK/include/TargetTrackingClient.h"

using namespace Navico::Protocol::NRP;

#define DEBUG_HOST

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
	double time = 0;
	tMultiRadarClient::GetInstance()->AddRadarListObserver(this);
	tMultiRadarClient::GetInstance()->AddUnlockStateObserver(this);
	char radars[2][MAX_SERIALNUMBER_SIZE];
	char radars_unlockkey[2][MAX_UNLOCKKEY_SIZE];

	


	memset(radars_unlockkey, 0, sizeof(radars_unlockkey));

	strcpy(radars_unlockkey[0], "123456789");
	strcpy(radars_unlockkey[1], "987654321");




#if true

	tImageClient* ImageClients[1];
	tTargetTrackingClient* TargetTrackingClients[1];


#endif

	

	int RadarCount = 0;

	EHostState HostState = EHostState::GetRadars;

	while (true) {

		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		tMultiRadarClient::GetInstance()->ExternalUpdate();
		time += 10e-3;


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
#ifdef HOST_DEBUG
					for (int i = 0; i < 1; i++) {
						ImageClients[i] = new tImageClient();
						

						auto* p_radar = tMultiRadarClient::GetInstance()->FindRadar(radars[i]);
						if (p_radar != nullptr) {
							p_radar->pImageClient = ImageClients[i];
						}

					}
#endif

					ImageClients[0] = new tImageClient();

					next_state = EHostState::ConnectRadars;
				}
				break;
			case EHostState::ConnectRadars:
				{
					for (INT32U i = 0; i < RadarCount; i++) {
						auto* p_radar = tMultiRadarClient::GetInstance()->FindRadar(radars[i]);

						for (int stream = 0; stream < 2; stream++) {
							
							auto ret = ImageClients[i]->Connect(p_radar->Serial.c_str(), stream);
							if (ret == 0) {
								cout << "connected to stream sent " << string(radars[i]) << "stream" << stream << endl;
							}
							std::this_thread::sleep_for(std::chrono::milliseconds(10));

						}
					}

					

					bool is_all_radars_connected = true;
					int success_cnt = 0;

					for (INT32U i = 0; i < RadarCount; i++) {
						auto* p_radar = tMultiRadarClient::GetInstance()->FindRadar(radars[i]);

						for (int stream = 0; stream < 2; stream++) {
							if (p_radar) {
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
						next_state = EHostState::SetFastScanMode;
					}
				}

#ifdef HOST_DEBUG
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
						next_state = EHostState::SetFastScanMode;
					}
				}
#endif
				break;
			case EHostState::SetFastScanMode:
			{
				for (INT32U i = 0; i < RadarCount; i++) {
					auto* p_radar = tMultiRadarClient::GetInstance()->FindRadar(radars[i]);
					p_radar->pImageClient->SetFastScanMode(21);
				}

				next_state = EHostState::SetScanerRPM;
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
			}
			break;
			case EHostState::SetScanerRPM:
			{
				for (INT32U i = 0; i < RadarCount; i++) {
					auto* p_radar = tMultiRadarClient::GetInstance()->FindRadar(radars[i]);
					p_radar->pImageClient->SetScannerRPM(601);
				}

				next_state = EHostState::PowerOn;
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
			}
			break;
			case EHostState::PowerOn:
			{
				for (INT32U i = 0; i < RadarCount; i++) {
					auto* p_radar = tMultiRadarClient::GetInstance()->FindRadar(radars[i]);
					p_radar->pImageClient->SetPower(true);
				}

				next_state = EHostState::TransmitOn;
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
			}
			break;

			case EHostState::TransmitOn:
			{
				for (INT32U i = 0; i < RadarCount; i++) {
					auto* p_radar = tMultiRadarClient::GetInstance()->FindRadar(radars[i]);
					p_radar->pImageClient->SetTransmit(true);
				}

				next_state = EHostState::SetGain;
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
			}
			break;

			case EHostState::SetGain:
			{
				for (INT32U i = 0; i < RadarCount; i++) {
					auto* p_radar = tMultiRadarClient::GetInstance()->FindRadar(radars[i]);
					p_radar->pImageClient->SetGain(eUserGainManualAuto::eUserGainAuto, 5);
				}

				next_state = EHostState::SetRange;
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
			}
			break;

			case EHostState::SetRange:
			{
				for (INT32U i = 0; i < RadarCount; i++) {
					auto* p_radar = tMultiRadarClient::GetInstance()->FindRadar(radars[i]);
					p_radar->pImageClient->SetRange(8500);
				}

				next_state = EHostState::SetSeaClutter;
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
			}
			break;

			case EHostState::SetSeaClutter:
			{
				for (INT32U i = 0; i < RadarCount; i++) {
					auto* p_radar = tMultiRadarClient::GetInstance()->FindRadar(radars[i]);
					p_radar->pImageClient->SetSeaClutter(eUserGainManualAuto::eUserGainAutoOffshore, 25);
				}

				next_state = EHostState::SetRain;
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
			}
			break;

			case EHostState::SetRain:
			{
				for (INT32U i = 0; i < RadarCount; i++) {
					auto* p_radar = tMultiRadarClient::GetInstance()->FindRadar(radars[i]);
					p_radar->pImageClient->SetRain(41);
				}

				next_state = EHostState::SetSectorBlanking;
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
			}
			break;


			case EHostState::SetSectorBlanking:
			{
				for (INT32U i = 0; i < RadarCount; i++) {
					auto* p_radar = tMultiRadarClient::GetInstance()->FindRadar(radars[i]);
					p_radar->pImageClient->SetSectorBlanking(0, true);
					std::this_thread::sleep_for(std::chrono::milliseconds(100));
					p_radar->pImageClient->SetSectorBlanking(2, true);
				}

				next_state = EHostState::TrackingInitTrackers;
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
			}
			break;
			
			case EHostState::TrackingInitTrackers:
			{

				// create Target Trackers
				for (int i = 0; i < 1; i++) {
					TargetTrackingClients[i] = new tTargetTrackingClient();
					
				}

				next_state = EHostState::ConnectTrackers;
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
			}
			break;
			
				
			case EHostState::ConnectTrackers:
			{

				for (INT32U i = 0; i < RadarCount; i++) {
					for (INT32S stream = 0; stream < 2; stream++) {
						auto ret = TargetTrackingClients[i]->Connect(radars[i], stream);
						if (ret == 0) {
							cout << "connected to stream sent " << string(radars[i]) << "stream" << stream << endl;
						}
						std::this_thread::sleep_for(std::chrono::milliseconds(10));
					}
				
				}

				next_state = EHostState::OwnShipNavigationData;
			}
			break;

			case EHostState::OwnShipNavigationData:
			{

				for (INT32U i = 0; i < RadarCount; i++) {

					auto ret = TargetTrackingClients[i]->SetOwnVesselNavigation(eSpeedType::eSpeedOverGround, 50, eDirectionType::eHeadingTrue, 600, eMagVarType::eNoMagVar, 30);
					if (ret == 0) {
						cout << "OwnShipNavigationData" << endl;
					}
					std::this_thread::sleep_for(std::chrono::milliseconds(10));
				}

				next_state = EHostState::AcquireTrack;
			}
			break;
		
			case EHostState::AcquireTrack:
			{

				for (INT32U i = 0; i < RadarCount; i++) {

					auto ret = TargetTrackingClients[i]->Acquire(0, 500, 50, eBearingType::eAbsolute);
					if (ret == 0) {
						cout << "Acquired Track " << endl;
					}
					std::this_thread::sleep_for(std::chrono::milliseconds(10));

					 ret = TargetTrackingClients[i]->Acquire(1, 1000, 120, eBearingType::eAbsolute);
					if (ret == 0) {
						cout << "Acquired Track " << endl;
					}
					std::this_thread::sleep_for(std::chrono::milliseconds(10));
				}

				next_state = EHostState::CancelTrack;
			}
			break;
			case EHostState::CancelTrack:
			{

				for (INT32U i = 0; i < RadarCount; i++) {

					auto ret = TargetTrackingClients[i]->Cancel(0);
					if (ret == 0) {
						cout << "CancelTrack Track " << endl;
					}
					std::this_thread::sleep_for(std::chrono::milliseconds(10));

				    ret = TargetTrackingClients[i]->Cancel(1);
					if (ret == 0) {
						cout << "CancelTrack Track " << endl;
					}
					std::this_thread::sleep_for(std::chrono::milliseconds(10));


					ret = TargetTrackingClients[i]->Acquire(0, 500, 50, eBearingType::eAbsolute);
					if (ret == 0) {
						cout << "Acquired Track " << endl;
					}
					std::this_thread::sleep_for(std::chrono::milliseconds(10));
				}

				next_state = EHostState::CancalAll;
			}
			break;
			case EHostState::CancalAll:
			{

				for (INT32U i = 0; i < RadarCount; i++) {

					auto ret = TargetTrackingClients[i]->CancelAll();
					if (ret == 0) {
						cout << "Cancel All Track " << endl;
					}
					std::this_thread::sleep_for(std::chrono::milliseconds(1000));
				}

				for (INT32U i = 0; i < RadarCount; i++) {

					auto ret = TargetTrackingClients[i]->Acquire(0, 45, 270, eBearingType::eAbsolute);
					if (ret == 0) {
						cout << "Acquired Track " << endl;
					}
					std::this_thread::sleep_for(std::chrono::milliseconds(10));
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(1000));

				for (INT32U i = 0; i < RadarCount; i++) {

					auto ret = TargetTrackingClients[i]->Acquire(1, 1000, 180, eBearingType::eAbsolute);
					if (ret == 0) {
						cout << "Acquired Track " << endl;
					}
					std::this_thread::sleep_for(std::chrono::milliseconds(10));
				}

				next_state = EHostState::PeriodicUpdate;
			}
			break;
			
			case EHostState::PeriodicUpdate:

				for (INT32U i = 0; i < RadarCount; i++) {
					auto* p_radar = tMultiRadarClient::GetInstance()->FindRadar(radars[i]);
					p_radar->pImageClient->SetRange(10000 + 0*sin(2*3.14* 0.2f * time));
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
				for (INT32U i = 0; i < RadarCount; i++) {

					auto ret = TargetTrackingClients[i]->SetOwnVesselNavigation(eSpeedType::eSpeedOverGround, 50, eDirectionType::eHeadingTrue, 600, eMagVarType::eNoMagVar, 30);
					if (ret == 0) {
						cout << "OwnShipNavigationData" << endl;
					}
					std::this_thread::sleep_for(std::chrono::milliseconds(10));
				}


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
