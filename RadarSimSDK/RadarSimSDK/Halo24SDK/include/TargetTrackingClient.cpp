#include "TargetTrackingClient.h"
#include "MultiRadarClient.h"

Navico::Protocol::NRP::tTargetTrackingClient::tTargetTrackingClient()
{

}

Navico::Protocol::NRP::tTargetTrackingClient::~tTargetTrackingClient()
{
}

int Navico::Protocol::NRP::tTargetTrackingClient::Connect(const char* pSerialNumber, unsigned imageStream)
{

	auto* p_radar = tMultiRadarClient::GetInstance()->FindRadar(pSerialNumber);
	p_radar->pTargetTrackingClient = this;
	if (p_radar == nullptr) {
		return -1;
	}
	SConnectTrackingClientPayload payload;
	payload.ImageStreamNo = imageStream;
	p_radar->SendPacket< SConnectTrackingClientPayload>(ESimSDKDataIDS::ConnectTrackingClient, (char*)pSerialNumber, &payload);
	strcpy(ConnectedSerialNumber, pSerialNumber);

	return 0;

}

bool Navico::Protocol::NRP::tTargetTrackingClient::Disconnect()
{
	auto* p_radar = tMultiRadarClient::GetInstance()->FindRadar(ConnectedSerialNumber);
	if (p_radar == nullptr) {
		return false;
	}
	SConnectTrackingClientPayload payload;

	p_radar->SendPacket< SConnectTrackingClientPayload>(ESimSDKDataIDS::DisconnectRadar, (char*)ConnectedSerialNumber, &payload);

	return true;

}

bool Navico::Protocol::NRP::tTargetTrackingClient::AddStateObserver(iTargetTrackingStateObserver* pObserver)
{
	StateObservers.push_back(pObserver);
	return true;
}

bool Navico::Protocol::NRP::tTargetTrackingClient::RemoveStateObserver(iTargetTrackingStateObserver* pObserver)
{
	StateObservers.remove(pObserver);
	return true;
}

bool Navico::Protocol::NRP::tTargetTrackingClient::AddTargetTrackingObserver(iTargetTrackingObserver* pObserver)
{
	TrackingObservers.push_back(pObserver);
	return true;
}

bool Navico::Protocol::NRP::tTargetTrackingClient::RemoveTargetTrackingObserver(iTargetTrackingObserver* pObserver)
{
	TrackingObservers.remove(pObserver);
	return true;
}

bool Navico::Protocol::NRP::tTargetTrackingClient::Acquire(uint32_t id, uint32_t range_m, uint16_t bearing_deg, eBearingType bearingType)
{
	auto* p_radar = tMultiRadarClient::GetInstance()->FindRadar(ConnectedSerialNumber);
	if (p_radar == nullptr) {
		return false;
	}
	STrackingAcquireTarget payload;

	payload.Id = id;
	payload.RangeMeter = range_m;
	payload.BearingDeg = bearing_deg;
	payload.BearingType = bearingType;

	p_radar->SendPacket< STrackingAcquireTarget>(ESimSDKDataIDS::TrackingAcquire, (char*)ConnectedSerialNumber, &payload);

	return true;
}

bool Navico::Protocol::NRP::tTargetTrackingClient::Cancel(uint32_t serverID)
{
	auto* p_radar = tMultiRadarClient::GetInstance()->FindRadar(ConnectedSerialNumber);
	if (p_radar == nullptr) {
		return false;
	}
	STrackingCancelTarget payload;

	payload.Id = serverID;


	p_radar->SendPacket< STrackingCancelTarget>(ESimSDKDataIDS::TrackingCancel, (char*)ConnectedSerialNumber, &payload);

	return true;
}

bool Navico::Protocol::NRP::tTargetTrackingClient::CancelAll()
{
	auto* p_radar = tMultiRadarClient::GetInstance()->FindRadar(ConnectedSerialNumber);
	if (p_radar == nullptr) {
		return false;
	}
	STrackingCancelTarget payload;
	payload.CancelAll = true?1:0;
	p_radar->SendPacket< STrackingCancelTarget>(ESimSDKDataIDS::TrackingCancel, (char*)ConnectedSerialNumber, &payload);

	return true;
}

bool Navico::Protocol::NRP::tTargetTrackingClient::SetBoatSpeed(eSpeedType speedType, uint32_t speed_dmps, eDirectionType directionType, uint32_t direction_deg)
{
	return false;
}

bool Navico::Protocol::NRP::tTargetTrackingClient::SetOwnVesselNavigation(eSpeedType speedType, uint32_t speed_dmps, eDirectionType directionType, uint32_t direction_ddeg, eMagVarType magvarType, int32_t magvar_ddeg)
{
	auto* p_radar = tMultiRadarClient::GetInstance()->FindRadar(ConnectedSerialNumber);
	if (p_radar == nullptr) {
		return false;
	}
	STrackingOwnShipNavigation payload;
	
	payload.SpeedType = (STrackingOwnShipNavigation::ESpeedType)speedType;
	payload.SpeedMetersPerSec = speed_dmps * 0.1f;
	payload.DirectionType = (STrackingOwnShipNavigation::EDirectionType)directionType;
	payload.DirectionDeg = direction_ddeg * 0.1f;
	payload.MagVarType = (STrackingOwnShipNavigation::EMagVarType)(magvarType);
	payload.MagVarDeg = magvar_ddeg * 0.1f;
	p_radar->SendPacket< STrackingOwnShipNavigation>(ESimSDKDataIDS::TrackingOwnshipData, (char*)ConnectedSerialNumber, &payload);

	return true;
}

bool Navico::Protocol::NRP::tTargetTrackingClient::SetN2KOverEthernetPriority(uint8_t priority)
{
	return false;
}

bool Navico::Protocol::NRP::tTargetTrackingClient::QueryAll()
{
	return false;
}

bool Navico::Protocol::NRP::tTargetTrackingClient::QueryAllTargets()
{
	return false;
}

bool Navico::Protocol::NRP::tTargetTrackingClient::QueryAlarmSetup()
{
	return false;
}

bool Navico::Protocol::NRP::tTargetTrackingClient::QueryProperties()
{
	return false;
}

bool Navico::Protocol::NRP::tTargetTrackingClient::SetDangerDistance(uint32_t range_m)
{
	return false;
}

bool Navico::Protocol::NRP::tTargetTrackingClient::SetDangerTime(uint32_t time_sec)
{
	return false;
}
