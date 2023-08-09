#include "MultiRadarClient.h"
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
    return 0;
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
    }
    else {
        return -1;
    }
  
   
}

void tMultiRadarClient::ClearUnlockKeys()
{
}

int tMultiRadarClient::UnlockRadar(const char* pSerialNumber, uint32_t wait_ms)
{
    return 0;
}

int tMultiRadarClient::UnlockRadar(const char* pSerialNumber, const uint8_t* pUnlockKey, unsigned unlockKeySize, uint32_t wait_ms)
{
    return 0;
}

unsigned tMultiRadarClient::GetRadars(char radars[][MAX_SERIALNUMBER_SIZE], unsigned maxRadars)
{
    return 0;
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
    return 0;
}

int tMultiRadarClient::IsServiceSupported(const char* pSerialNumber, eService service, unsigned imageStream)
{
    return 0;
}

bool tMultiRadarClient::ResetDeviceIDs()
{
    return false;
}

void tMultiRadarClient::RegisterRadar(const char* serial_number)
{
    SRadar *p_radar = new SRadar();
    p_radar->Serial = string(serial_number);

    Radars.push_back(p_radar);
    for (auto radar_observers : RadarListObservers) {
        radar_observers->UpdateRadarList(serial_number, iRadarListObserver::eAction::Added);
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

void Navico::Protocol::tMultiRadarClient::ExternalUpdate()
{
    StateMachine();
}

void Navico::Protocol::tMultiRadarClient::StateMachine()
{
    auto curr_state = MultiRadarState;
    auto next_state = curr_state;


    switch (curr_state)
    {
    case Idle:
        next_state = EMultiRadarState::Init;

        break;
    case Init:
        if (RadarListObservers.size() > 0 && UnlockStateObserver.size() > 0 && pUnlockKeySupplier != nullptr) {
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
