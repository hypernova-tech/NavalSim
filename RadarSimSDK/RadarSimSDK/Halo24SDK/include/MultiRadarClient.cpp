#include "MultiRadarClient.h"
#include "../../Lib/Types/Halo24Types.h"
#include <thread>
#include "../../CHost.h"
#include "ImageClient.h"
#include "ClientErrors.h"
using namespace Navico::Protocol::NRP;
using namespace Navico::Protocol;

tMultiRadarClient* tMultiRadarClient::pInstance = nullptr;

tMultiRadarClient::tMultiRadarClient()
{
    pUnlockKeySupplier = nullptr;
}

tMultiRadarClient* tMultiRadarClient::GetInstance()
{
    if (pInstance == nullptr) {
        pInstance = new tMultiRadarClient();
    }

    return pInstance;
}

int tMultiRadarClient::Connect()
{
    CHost::GetInstance()->Init();
    return 1;
}

bool tMultiRadarClient::Disconnect()
{
    return false;
}

void tMultiRadarClient::AddRadarListObserver(iRadarListObserver* pObserver)
{
    RadarListObservers.push_back(pObserver);
}

void tMultiRadarClient::RemoveRadarListObserver(iRadarListObserver* pObserver)
{
    RadarListObservers.remove(pObserver);
}

void tMultiRadarClient::AddUnlockStateObserver(iUnlockStateObserver* pObserver)
{
    UnlockStateObserver.push_back(pObserver);
}

void tMultiRadarClient::RemoveUnlockStateObserver(iUnlockStateObserver* pObserver)
{
    UnlockStateObserver.remove(pObserver);
}

void tMultiRadarClient::SetUnlockKeySupplier(iUnlockKeySupplier* pSupplier)
{
    pUnlockKeySupplier = pSupplier;;
    
}

int tMultiRadarClient::SetUnlockKey(const char* pSerialNumber, const uint8_t* pUnlockKey, unsigned unlockKeySize)
{

    string serial = string(pSerialNumber);
    SRadar* p_radar = FindRadar(pSerialNumber);
    bool ret = false;

    if (p_radar != nullptr) {
        ret = p_radar->SetUnlockKey(pUnlockKey, unlockKeySize);
    }
    if (ret) {
        for (auto unlock_state_observer : UnlockStateObserver) {
            unlock_state_observer->UpdateUnlockState(p_radar->Serial.c_str(), 1); //todo fixme
        }

        return 0;
    }
    else {
        return eErrors::ETimedOut;
    }
  
   
}

void tMultiRadarClient::ClearUnlockKeys()
{
}

int tMultiRadarClient::UnlockRadar(const char* pSerialNumber, uint32_t wait_ms)
{
	auto* p_radar = FindRadar(pSerialNumber);

	    if (p_radar->GetIsUnlocked()) {
	        return 1;
	    }

	    if (p_radar != nullptr) {
	        SRadarSimSDKPacket* p_pack = new SRadarSimSDKPacket();



	        SUnlockKeysPayload* p_payload = (SUnlockKeysPayload*)p_pack->Payload;

	        memcpy(p_payload->SerialKey, pSerialNumber, strlen(pSerialNumber));
	        p_payload->SerialLen = strlen(pSerialNumber);

	        memcpy(p_payload->UnlockKey, p_radar->UnlockKey, p_radar->UnlockKeySize);
	        p_payload->UnlockKeyLen = p_radar->UnlockKeySize;

	        p_pack->SetID(ESimSDKDataIDS::UnlockKeys);
	        p_pack->SetPayloadSize(sizeof(SUnlockKeysPayload));
	        p_radar->pConnection->SendData((INT8U*)p_pack, p_pack->GetTransmitSize(), p_radar->pConnection->GetRemotePort());

	        if (wait_ms > 0) {
	            std::this_thread::sleep_for(std::chrono::milliseconds(wait_ms));
	            if (p_radar->GetIsUnlocked()) {
	                return 1;
	            }
	            else {
	                return 0;
	            }
	        }
	    }

	    return 0;
}

int tMultiRadarClient::UnlockRadar(const char* pSerialNumber, const uint8_t* pUnlockKey, unsigned unlockKeySize, uint32_t wait_ms)
{
    
    auto* p_radar = FindRadar(pSerialNumber);

    if (p_radar->GetIsUnlocked()) {
        return 1;
    }

    if (p_radar != nullptr) {
        SRadarSimSDKPacket* p_pack = new SRadarSimSDKPacket();

        
       
        SUnlockKeysPayload* p_payload = (SUnlockKeysPayload*)p_pack->Payload;

        memcpy(p_payload->SerialKey, pSerialNumber, strlen(pSerialNumber));
        p_payload->SerialLen = strlen(pSerialNumber);

        memcpy(p_payload->UnlockKey, pUnlockKey, unlockKeySize);
        p_payload->UnlockKeyLen = unlockKeySize;

        p_pack->SetID(ESimSDKDataIDS::UnlockKeys);
        p_pack->SetPayloadSize(sizeof(SUnlockKeysPayload));
        p_radar->pConnection->SendData((INT8U*)p_pack, p_pack->GetTransmitSize(), p_radar->pConnection->GetRemotePort());

        if (wait_ms > 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(wait_ms));
            if (p_radar->GetIsUnlocked()) {
                return 1;
            }
            else {
                return 0;
            }
        }
    }
   
    return 0;
}

unsigned tMultiRadarClient::GetRadars(char radars[][MAX_SERIALNUMBER_SIZE], unsigned maxRadars)
{
    INT32U ind = 0;

  

    for (auto& radar : Radars) {
        if (radars != nullptr && ind < maxRadars) {
            memcpy(radars[ind], radar->Serial.c_str(), radar->Serial.length());
            radars[ind][radar->Serial.length()] = 0;
        }
        
        ind++;
    }


    return ind;
}

int tMultiRadarClient::GetLockID(char lockID[MAX_LOCKID_SIZE * 2], const char* pSerialNumber)
{
    return 0;
}

void tMultiRadarClient::ClearRadars()
{
}

bool tMultiRadarClient::QueryRadars()
{
    return false;
}

int tMultiRadarClient::GetImageStreamCount(const char* pSerialNumber)
{
    return 2;
}

int tMultiRadarClient::IsServiceSupported(const char* pSerialNumber, eService service, unsigned imageStream)
{
    return 1;
}

bool tMultiRadarClient::ResetDeviceIDs()
{
    return false;
}

void tMultiRadarClient::RegisterRadar(IConnection* p_conn, const char* serial_number)
{
    string new_serial = string(serial_number);
   
    for (auto* p_radar : Radars) {
        if (p_radar->Serial == new_serial) {
            return;
        }
    }

    SRadar *p_radar = new SRadar();
    p_radar->Serial = new_serial;
    p_radar->pConnection = p_conn;
    Radars.push_back(p_radar);

    if (true) {
        for (auto &radar_observers : RadarListObservers) {
            radar_observers->UpdateRadarList(serial_number, iRadarListObserver::eAction::Added);
        }
    }
  
}

SRadar* tMultiRadarClient::FindRadar(const char* serial_number)
{
    string serial_str = string(serial_number);

    for (auto radar : Radars) {
        if (serial_str == radar->Serial) {
            return radar;
        }
    }

    return nullptr;
}
SRadar* tMultiRadarClient::FindRadarFromConnection(IConnection* p_conn)
{
    for (auto& radar : Radars) {
        if (radar->pConnection == p_conn) {
            return radar;
        }
    }

    return nullptr;
}

SRadar* Navico::Protocol::tMultiRadarClient::GetRadar(INT32U ind)
{

    if (ind >= Radars.size()){
        return nullptr;
    }
    else {
        return Radars[ind];
    }
}


void Navico::Protocol::tMultiRadarClient::ExternalUpdate()
{
    StateMachine();
}
void Navico::Protocol::tMultiRadarClient::HandleReponse(IConnection* p_conn, SCommandAckNackResponse* p_res)
{
    auto* p_radar = FindRadarFromConnection(p_conn);
    if (p_radar == nullptr) {
        return;
    }

    switch (p_res->Id)
    {
    case ESimSDKDataIDS::UnlockKeys:
        if (p_res->Result == 1) {
            p_radar->SetIsUnlocked(true);
        }
        else {
            p_radar->SetIsUnlocked(false);
        }
        break;
    case ESimSDKDataIDS::ConnectRadar:
        if (p_radar->pImageClient == nullptr) {
            return;
        }
        p_radar->pImageClient->SetIsStreamConnected(p_res->StreamNo, true);
        break;

    default:
        break;
    }
}

void Navico::Protocol::tMultiRadarClient::HandleRadarState(IConnection* p_conn, SRadarState* p_res)
{
    auto* p_radar = FindRadarFromConnection(p_conn);
    if (p_radar == nullptr) {
        return;
    }

    if (p_radar->pImageClient == nullptr) {
        return;
    }

    p_radar->pImageClient->OnReceivedRadarState(p_res);
}

void Navico::Protocol::tMultiRadarClient::HandleRadarSetup(IConnection* p_conn, SRadarSetupPayload* p_setup)
{
    auto* p_radar = FindRadarFromConnection(p_conn);
    if (p_radar == nullptr) {
        return;
    }
    if (p_radar->pImageClient == nullptr) {
        return;
    }

    p_radar->pImageClient->OnReceivedRadarSetup(p_setup);
}

void Navico::Protocol::tMultiRadarClient::HandleSpoke(IConnection* p_conn, SHalo24SpokePayload* p_res)
{
    auto* p_radar = FindRadarFromConnection(p_conn);
    if (p_radar == nullptr) {
        return;
    }
    if (p_radar->pImageClient == nullptr) {
        return;
    }
    p_radar->pImageClient->OnReceivedRadarSpoke(p_res);
}
void Navico::Protocol::tMultiRadarClient::HandleTrackingStatus(IConnection* p_conn, STrackingTargetStatusPayload* p_res)
{
    auto* p_radar = FindRadarFromConnection(p_conn);
    if (p_radar == nullptr) {
        return;
    }
    if (p_radar->pTargetTrackingClient == nullptr) {
        return;
    }
    p_radar->pTargetTrackingClient->OnReceivedTrackingStatus(p_res);
}

void Navico::Protocol::tMultiRadarClient::StateMachine()
{
    auto curr_state = MultiRadarState;
    auto next_state = curr_state;
    int max_radar_count = 1;

    switch (curr_state)
    {
    case Idle:
        next_state = EMultiRadarState::Init;

        break;
    case Init:
        if (RadarListObservers.size() > max_radar_count && UnlockStateObserver.size() > max_radar_count && pUnlockKeySupplier != nullptr) {
            next_state = EMultiRadarState::WaitForRadars;
        }
        break;
    case WaitForRadars:
        next_state = EMultiRadarState::Run;
        break;
    case Run: 
        break;
    default:
        break;
    }

    MultiRadarState = curr_state;
}

bool Navico::Protocol::SRadar::GetIsImageStreamConnected(int stream_no)
{
    tImageClient* p_client = (tImageClient*)pImageClient;
    if (p_client == nullptr) {
        return false;
    }
    return p_client->GetIsStreamConnected(stream_no);
}
