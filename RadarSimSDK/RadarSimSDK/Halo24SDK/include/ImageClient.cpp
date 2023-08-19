#if true
#include "../include/ImageClient.h"
#include "../include/MultiRadarClient.h"
#include "../../Lib/Types/Halo24Types.h"

using namespace Navico::Protocol::NRP;

Navico::Protocol::NRP::tImageClient::tImageClient()
{
}

Navico::Protocol::NRP::tImageClient::~tImageClient()
{
}

int Navico::Protocol::NRP::tImageClient::Connect(const char* pSerialNumber, unsigned imageStream)
{
    auto *p_radar = tMultiRadarClient::GetInstance()->FindRadar(pSerialNumber);
    if (p_radar == nullptr) {
        return -1;
    }
    SConnectRadar connect_radar_payload;
    connect_radar_payload.ImageStreamNo = imageStream;
    p_radar->SendPacket< SConnectRadar>(ESimSDKDataIDS::ConnectRadar, (char*)pSerialNumber, &connect_radar_payload);

    strcpy(ConnectedSerialNumber, pSerialNumber);

    return 0;
}

bool Navico::Protocol::NRP::tImageClient::Disconnect()
{
    auto* p_radar = tMultiRadarClient::GetInstance()->FindRadar(ConnectedSerialNumber);
    if (p_radar == nullptr) {
        return 0;
    }
    SConnectRadar connect_radar_payload;
    p_radar->SendPacket< SConnectRadar>(ESimSDKDataIDS::DisconnectRadar, (char*)ConnectedSerialNumber, &connect_radar_payload);

    return true;
}

bool Navico::Protocol::NRP::tImageClient::AddStateObserver(iImageClientStateObserver* pObserver)
{
    ClientStateObserver.push_back(pObserver);
    return true;
}

bool Navico::Protocol::NRP::tImageClient::RemoveStateObserver(iImageClientStateObserver* pObserver)
{
    ClientStateObserver.remove(pObserver);
    return true;
}

bool Navico::Protocol::NRP::tImageClient::AddSpokeObserver(iImageClientSpokeObserver* pObserver)
{
    CientSpokeObservers.push_back(pObserver);
    return true;
}

bool Navico::Protocol::NRP::tImageClient::RemoveSpokeObserver(iImageClientSpokeObserver* pObserver)
{
    CientSpokeObservers.remove(pObserver);
    return true;
}

bool Navico::Protocol::NRP::tImageClient::AddFeatureObserver(iFeatureObserver* pObserver)
{
    ClientFeatureObservers.push_back(pObserver);
    return true;
}

bool Navico::Protocol::NRP::tImageClient::RemoveFeatureObserver(iFeatureObserver* pObserver)
{
    ClientFeatureObservers.remove(pObserver);
    return true;
}

bool Navico::Protocol::NRP::tImageClient::SetPower(bool state)
{
    auto* p_radar = tMultiRadarClient::GetInstance()->FindRadar(ConnectedSerialNumber);
    if (p_radar == nullptr) {
        return 0;
    }

    SPowerControl power_control;
    power_control.PowerOn = state?1:0;
    p_radar->SendPacket< SPowerControl>(ESimSDKDataIDS::PowerControl, (char*)ConnectedSerialNumber, &power_control);
    return true;
}

bool Navico::Protocol::NRP::tImageClient::SetTransmit(bool state)
{
    auto* p_radar = tMultiRadarClient::GetInstance()->FindRadar(ConnectedSerialNumber);
    if (p_radar == nullptr) {
        return 0;
    }

    STransmitControl xmt_control;
    xmt_control.TransmitOn = state ? 1 : 0;
    p_radar->SendPacket< STransmitControl>(ESimSDKDataIDS::TransmitControl, (char*)ConnectedSerialNumber, &xmt_control);
    return true;
}

bool Navico::Protocol::NRP::tImageClient::SetTimedTransmit(bool state)
{
    return false;
}

bool Navico::Protocol::NRP::tImageClient::SetTimedTransmitSetup(uint32_t transmitPeriod_sec, uint32_t standbyPeriod_sec)
{
    return false;
}

bool Navico::Protocol::NRP::tImageClient::SetRange(uint32_t range_m)
{
    auto* p_radar = tMultiRadarClient::GetInstance()->FindRadar(ConnectedSerialNumber);
    if (p_radar == nullptr) {
        return 0;
    }

    SRangeControl mes;
    mes.RangeMeter = range_m;
    p_radar->SendPacket< SRangeControl>(ESimSDKDataIDS::RangeControl, (char*)ConnectedSerialNumber, &mes);

    return true;
}

bool Navico::Protocol::NRP::tImageClient::SendClientWatchdog()
{
    return false;
}

void Navico::Protocol::NRP::tImageClient::SetAutoSendClientWatchdog(bool state)
{
}

bool Navico::Protocol::NRP::tImageClient::SetUseMode(eUseMode mode)
{
    return false;
}

bool Navico::Protocol::NRP::tImageClient::SetGain(eUserGainManualAuto type, uint8_t level)
{
    auto* p_radar = tMultiRadarClient::GetInstance()->FindRadar(ConnectedSerialNumber);
    if (p_radar == nullptr) {
        return 0;
    }

    SGainMode mes;
    mes.Type = type;
    mes.Level = level;

    p_radar->SendPacket< SGainMode>(ESimSDKDataIDS::GainControl, (char*)ConnectedSerialNumber, &mes);
    return true;
}

bool Navico::Protocol::NRP::tImageClient::SetSeaClutter(eUserGainManualAuto type, uint8_t level)
{
    auto* p_radar = tMultiRadarClient::GetInstance()->FindRadar(ConnectedSerialNumber);
    if (p_radar == nullptr) {
        return 0;
    }

    SSeaClutter mes;
    mes.SeaClutterType = type;
    mes.SeaClutterLevel = level;
    mes.SeaClutterAutoOffset = 0;

    p_radar->SendPacket<SSeaClutter>(ESimSDKDataIDS::SeaClutterControl, (char*)ConnectedSerialNumber, &mes);

    return true;
}

bool Navico::Protocol::NRP::tImageClient::SetSeaClutter(eUserGainManualAuto type, uint8_t level, int8_t autoOffset, eUserGainValid valid)
{
    auto* p_radar = tMultiRadarClient::GetInstance()->FindRadar(ConnectedSerialNumber);
    if (p_radar == nullptr) {
        return 0;
    }

    SSeaClutter mes;
    mes.SeaClutterLevel = type;
    mes.SeaClutterLevel = level;
    mes.SeaClutterAutoOffset = autoOffset;
    mes.UserGainValid = valid;

    p_radar->SendPacket<SSeaClutter>(ESimSDKDataIDS::SeaClutterControl, (char*)ConnectedSerialNumber, &mes);

    return true;
}

bool Navico::Protocol::NRP::tImageClient::SetSTCCurveType(eStcCurveType type)
{
    return false;
}

bool Navico::Protocol::NRP::tImageClient::SetRain(uint8_t level)
{
    auto* p_radar = tMultiRadarClient::GetInstance()->FindRadar(ConnectedSerialNumber);
    if (p_radar == nullptr) {
        return 0;
    }

    SRainClutter mes;
    mes.Level = level;


    p_radar->SendPacket<SRainClutter>(ESimSDKDataIDS::RainClutterControl, (char*)ConnectedSerialNumber, &mes);

    return true;
}

bool Navico::Protocol::NRP::tImageClient::SetSideLobe(eUserGainManualAuto type, uint8_t level)
{
    return false;
}

bool Navico::Protocol::NRP::tImageClient::SetFTC(uint8_t level)
{
    return false;
}

bool Navico::Protocol::NRP::tImageClient::SetInterferenceReject(uint8_t level)
{
    return false;
}

bool Navico::Protocol::NRP::tImageClient::SetNoiseReject(uint8_t level)
{
    return false;
}

bool Navico::Protocol::NRP::tImageClient::SetBeamSharpening(uint8_t level)
{
    return false;
}

bool Navico::Protocol::NRP::tImageClient::SetDopplerMode(uint8_t level)
{
    return false;
}

bool Navico::Protocol::NRP::tImageClient::SetDopplerThreshold_cmps(uint16_t threshold_cmps)
{
    return false;
}

bool Navico::Protocol::NRP::tImageClient::SetLEDsLevel(uint8_t level)
{
    return false;
}

bool Navico::Protocol::NRP::tImageClient::SetFastScanMode(uint8_t level)
{
    auto* p_radar = tMultiRadarClient::GetInstance()->FindRadar(ConnectedSerialNumber);
    if (p_radar == nullptr) {
        return 0;
    }

    SFastScanMode mes;
    mes.Level = level;


    p_radar->SendPacket<SFastScanMode>(ESimSDKDataIDS::FastScanMode, (char*)ConnectedSerialNumber, &mes);

    return true;
}

bool Navico::Protocol::NRP::tImageClient::SetTargetStretch(bool state)
{
    return false;
}

bool Navico::Protocol::NRP::tImageClient::SetTargetStretch(uint8_t level)
{
    return false;
}

bool Navico::Protocol::NRP::tImageClient::SetTargetBoost(uint8_t level)
{
    return false;
}

bool Navico::Protocol::NRP::tImageClient::SetScannerRPM(unsigned rpm)
{
    auto* p_radar = tMultiRadarClient::GetInstance()->FindRadar(ConnectedSerialNumber);
    if (p_radar == nullptr) {
        return 0;
    }

    SScanerRPM mes;
    mes.Rpm = rpm;


    p_radar->SendPacket<SScanerRPM>(ESimSDKDataIDS::ScannerRPM, (char*)ConnectedSerialNumber, &mes);

    return true;
}

bool Navico::Protocol::NRP::tImageClient::SetUserMinSNR(float level_dB)
{
    return false;
}

bool Navico::Protocol::NRP::tImageClient::SetVideoAperture(float level_dB)
{
    return false;
}

bool Navico::Protocol::NRP::tImageClient::SetRangeSTCTrim(float level_dB)
{
    return false;
}

bool Navico::Protocol::NRP::tImageClient::SetRangeSTCRate(float level_dBpDec)
{
    return false;
}

bool Navico::Protocol::NRP::tImageClient::SetSeaSTCTrim(float level_dB)
{
    return false;
}

bool Navico::Protocol::NRP::tImageClient::SetSeaSTCRate1(float level_dBpDec)
{
    return false;
}

bool Navico::Protocol::NRP::tImageClient::SetSeaSTCRate2(float level_dBpDec)
{
    return false;
}

bool Navico::Protocol::NRP::tImageClient::SetRainSTCTrim(float level_dB)
{
    return false;
}

bool Navico::Protocol::NRP::tImageClient::SetRainSTCRate(float level_dBpDec)
{
    return false;
}

bool Navico::Protocol::NRP::tImageClient::SetSectorBlanking(uint32_t sectorID, bool state)
{
    auto* p_radar = tMultiRadarClient::GetInstance()->FindRadar(ConnectedSerialNumber);
    if (p_radar == nullptr) {
        return 0;
    }

    SSectorBlankingPayload mes;
    mes.SetSectorControl(true, sectorID, state);


    p_radar->SendPacket<SSectorBlankingPayload>(ESimSDKDataIDS::SectorBlanking, (char*)ConnectedSerialNumber, &mes);

    return true;
}

bool Navico::Protocol::NRP::tImageClient::SetSectorBlankingSetup(uint32_t sectorID, bool state, uint16_t startBearing_degrees, uint16_t endBearing_degrees)
{
    auto* p_radar = tMultiRadarClient::GetInstance()->FindRadar(ConnectedSerialNumber);
    if (p_radar == nullptr) {
        return 0;
    }

    SSectorBlankingPayload mes;
    mes.SetSectorSetup(true, sectorID, state, startBearing_degrees, endBearing_degrees);
    p_radar->SendPacket<SSectorBlankingPayload>(ESimSDKDataIDS::SectorBlanking, (char*)ConnectedSerialNumber, &mes);

    return true;
}

bool Navico::Protocol::NRP::tImageClient::SetMainBangSuppression(bool state)
{
    return false;
}

bool Navico::Protocol::NRP::tImageClient::QueryAll()
{
    return false;
}

bool Navico::Protocol::NRP::tImageClient::QueryMode()
{
    return false;
}

bool Navico::Protocol::NRP::tImageClient::QuerySetup()
{
    return false;
}

bool Navico::Protocol::NRP::tImageClient::QueryAdvancedSetup()
{
    return false;
}

bool Navico::Protocol::NRP::tImageClient::QueryProperties()
{
    return false;
}

bool Navico::Protocol::NRP::tImageClient::QueryConfiguration()
{
    return false;
}

bool Navico::Protocol::NRP::tImageClient::SetTuneState(bool automatic)
{
    return false;
}

bool Navico::Protocol::NRP::tImageClient::SetTuneCoarse(uint8_t level)
{
    return false;
}

bool Navico::Protocol::NRP::tImageClient::SetTuneFine(uint8_t level)
{
    return false;
}

bool Navico::Protocol::NRP::tImageClient::QueryFeatures()
{
    return false;
}

bool Navico::Protocol::NRP::tImageClient::SetParkPosition(uint32_t angle_deg)
{
    return false;
}

bool Navico::Protocol::NRP::tImageClient::SetZeroRangeOffset(uint16_t offset)
{
    return false;
}

bool Navico::Protocol::NRP::tImageClient::SetZeroBearingOffset(uint16_t bearing_ddeg)
{
    return false;
}

bool Navico::Protocol::NRP::tImageClient::SetAntennaHeight(uint32_t antennaHeight_mm)
{
    return false;
}

bool Navico::Protocol::NRP::tImageClient::SetAntennaOffsets(int32_t xOffset_mm, int32_t yOffset_mm)
{
    return false;
}

bool Navico::Protocol::NRP::tImageClient::SetAntennaType(uint16_t antennaType)
{
    return false;
}

bool Navico::Protocol::NRP::tImageClient::SetToFactoryDefaults()
{
    return false;
}

bool Navico::Protocol::NRP::tImageClient::SetGuardZoneEnable(uint8_t zone, bool state)
{
    auto* p_radar = tMultiRadarClient::GetInstance()->FindRadar(ConnectedSerialNumber);
    if (p_radar == nullptr) {
        return 0;
    }

    SGuardZonePayload mes;
    mes.SetZoneControl(true, zone, state);

    p_radar->SendPacket<SGuardZonePayload>(ESimSDKDataIDS::GuardZoneControl, (char*)ConnectedSerialNumber, &mes);

    return true;
}

bool Navico::Protocol::NRP::tImageClient::SetGuardZoneSetup(uint8_t zone, uint32_t startRange_m, uint32_t endRange_m, uint16_t bearing_deg, uint16_t width_deg)
{
    auto* p_radar = tMultiRadarClient::GetInstance()->FindRadar(ConnectedSerialNumber);
    if (p_radar == nullptr) {
        return 0;
    }

    SGuardZonePayload mes;
    mes.SetZoneSetup(true, zone, startRange_m, endRange_m, bearing_deg, width_deg);

    p_radar->SendPacket<SGuardZonePayload>(ESimSDKDataIDS::GuardZoneControl, (char*)ConnectedSerialNumber, &mes);

    return true;
}

bool Navico::Protocol::NRP::tImageClient::SetGuardZoneAlarmSetup(uint8_t zone, eGuardZoneAlarmType type)
{
    auto* p_radar = tMultiRadarClient::GetInstance()->FindRadar(ConnectedSerialNumber);
    if (p_radar == nullptr) {
        return 0;
    }

    SGuardZonePayload mes;
    mes.SetZoneAlarmSetup(true, zone, type);

    p_radar->SendPacket<SGuardZonePayload>(ESimSDKDataIDS::GuardZoneControl, (char*)ConnectedSerialNumber, &mes);

    return true;
}

bool Navico::Protocol::NRP::tImageClient::SetGuardZoneAlarmSuppress(uint8_t zone, bool state)
{
    auto* p_radar = tMultiRadarClient::GetInstance()->FindRadar(ConnectedSerialNumber);
    if (p_radar == nullptr) {
        return 0;
    }

    SGuardZonePayload mes;
    mes.SetZoneAlarmSuppress(true, zone, state);

    p_radar->SendPacket<SGuardZonePayload>(ESimSDKDataIDS::GuardZoneControl, (char*)ConnectedSerialNumber, &mes);

    return true;
}

bool Navico::Protocol::NRP::tImageClient::SetGuardZoneAlarmCancel(uint8_t zone, eGuardZoneAlarmType type)
{
    auto* p_radar = tMultiRadarClient::GetInstance()->FindRadar(ConnectedSerialNumber);
    if (p_radar == nullptr) {
        return 0;
    }

    SGuardZonePayload mes;
    mes.SetZoneAlarmCancel(true, zone, type);

    p_radar->SendPacket<SGuardZonePayload>(ESimSDKDataIDS::GuardZoneControl, (char*)ConnectedSerialNumber, &mes);

    return true;
}

bool Navico::Protocol::NRP::tImageClient::SetGuardZoneSensitivity(uint8_t level)
{
    auto* p_radar = tMultiRadarClient::GetInstance()->FindRadar(ConnectedSerialNumber);
    if (p_radar == nullptr) {
        return 0;
    }

    SGuardZonePayload mes;
    mes.SetAlarmSensitivty(true, level);

    p_radar->SendPacket<SGuardZonePayload>(ESimSDKDataIDS::GuardZoneControl, (char*)ConnectedSerialNumber, &mes);

    return true;
}
bool Navico::Protocol::NRP::tImageClient::GetIsStreamConnected(int stream_no)
{
    return IsImageStreamConnected[stream_no];
}
void Navico::Protocol::NRP::tImageClient::SetIsStreamConnected(int stream_no, bool val)
{
    IsImageStreamConnected[stream_no] = val;
}
void Navico::Protocol::NRP::tImageClient::OnReceivedRadarState(const SRadarState* p_state)
{
    Mode.state = p_state->State;

    for (auto* p_obs : ClientStateObserver) {
        
        p_obs->UpdateMode(&Mode);
    }
}
void Navico::Protocol::NRP::tImageClient::OnReceivedRadarSetup(const SRadarSetupPayload* p_state)
{

    memcpy(&Setup, &p_state->RadarSetupData, sizeof(Setup));
    for (auto* p_obs : ClientStateObserver) {

        p_obs->UpdateSetup(&Setup);
    }
}
void Navico::Protocol::NRP::tImageClient::OnReceivedRadarSpoke(const SHalo24SpokePayload* p_spoke)
{
    for (auto* p_obs : CientSpokeObservers) {

        p_obs->UpdateSpoke((Spoke::tSpokeV9174*)&p_spoke->SpokeData);
    }
}


#endif