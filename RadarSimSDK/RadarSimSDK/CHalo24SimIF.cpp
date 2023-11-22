#include "CHalo24SimIF.h"
#include "Lib/Types/Halo24Types.h"
#include "IConnection.h"
using namespace Navico::Protocol;

CHalo24IF::CHalo24IF()
{
	pRes = new SRadarSimSDKPacket();
}

void CHalo24IF::AddSDK(IConnection *p_conn)
{
	SHaloEntry* p_entry = new SHaloEntry();
	p_entry->pConnection = p_conn;

	Entries.push_back(p_entry);
	pMultiRadarClient = tMultiRadarClient::GetInstance();
}

void CHalo24IF::RadarStart()
{
}

void CHalo24IF::RadarStop()
{
}

void CHalo24IF::AdjustRange(FLOAT32 range_meter)
{
}
void CHalo24IF::OnReceivedData(void* p_if, INT8U* p_data, INT32U len)
{
	IConnection* p_conn = (IConnection*)p_if;
	SRadarSimSDKPacket* p_pack = (SRadarSimSDKPacket*)p_data;
	p_pack->Init();

	switch (p_pack->Header.PacketType)
	{
	case ESimSDKDataIDS::Serials:
		{
			auto* p_entry = FindEntry(p_conn);

			if (p_entry != nullptr) {
				pMultiRadarClient->RegisterRadar(p_conn, (char*)p_pack->Payload);
				
				
			}
		}
	case ESimSDKDataIDS::Response:
		{
			auto* p_entry = FindEntry(p_conn);

			if (p_entry != nullptr) {
				pMultiRadarClient->HandleReponse(p_conn, (SCommandAckNackResponse*)p_pack->Payload);
			}
		}
	
		break;

	case ESimSDKDataIDS::RadarState:
		pMultiRadarClient->HandleRadarState(p_conn, (SRadarState*)p_pack->Payload);
		break;

	case ESimSDKDataIDS::RadarSetup:
		pMultiRadarClient->HandleRadarSetup(p_conn, (SRadarSetupPayload*)p_pack->Payload);
		break;
	case ESimSDKDataIDS::SpokeData:
	{
		SHalo24SpokePayload* tempMssg = reinterpret_cast<SHalo24SpokePayload*> (p_pack->Payload);
		pMultiRadarClient->HandleSpoke(p_conn, tempMssg);
		break;
	}
	case ESimSDKDataIDS::TrackingStatus:
		pMultiRadarClient->HandleTrackingStatus(p_conn, (STrackingTargetStatusPayload*)p_pack->Payload);
		break;
	default:
		break;
	}


}

SHaloEntry* CHalo24IF::FindEntry(IConnection* p_con)
{
	for (auto* p_entry : Entries) {
		if (p_entry->pConnection == p_con) {
			return p_entry;
		}
	}

	return nullptr;
}
