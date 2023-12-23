// Fill out your copyright notice in the Description page of Project Settings.


#include "Products/IDAS/Sensors/Radar/Halo24/CommIF/Halo24CommIF.h"
#include "HAL/PlatformNamedPipe.h"
#include <Lib/Utils/CUtil.h>


UHalo24CommIF::~UHalo24CommIF()
{
	if (SenderThread != nullptr) {
		SenderThread->Kill();
		delete SenderThread;
	}
}

void UHalo24CommIF::SendData(void* p_data, uint32 size_in_bytes)
{
	Mutex.Lock();
	HasNewData = true;
	SScanResult* p_in = (SScanResult*)p_data;
	SScanResult* p_new = p_in;

	CurrentRequest.Add(p_new);
	Mutex.Unlock();
}



uint32 UHalo24CommIF::Run()
{


	while (!IsStoped) {
		FPlatformProcess::Sleep(0.02);
		Mutex.Lock();
		bool has_data = CurrentRequest.Num() > 0;
		Mutex.Unlock();
		if (has_data) {
			SendRadarTrack();
		}

	}
	return uint32();
}

void UHalo24CommIF::SetHostIF(IHalo24HostIF* p_val)
{
	pHostIF = p_val;
}
void UHalo24CommIF::SendSerial(INT8U* p_data, INT32U count)
{
	auto pack = GetPacket();
	pack->SetPayload(ESimSDKDataIDS::Serials, p_data, count);

	pUDPConnection->SendUDPData((INT8U*)pack, pack->GetTransmitSize());

	RestorePacket(pack);
}

void UHalo24CommIF::BeginPlay()
{
	Super::BeginPlay();
	pUDPConnection->AddConnectionDataReceiver(this);
	for (int i = 0; i < 16; i++) {
		Packets.Add(new SRadarSimSDKPacket());
	}

	SenderThread = FRunnableThread::Create(this, *(GetOwner()->GetName()));
	RegisterConnectionInfo(0, pUDPConnection->GetConnectionInfo(), pUDPConnection);

}
SRadarSimSDKPacket packspoke;
void UHalo24CommIF::SendRadarTrack()
{
	Mutex.Lock();
	SScanResult* p_current = CurrentRequest[0];


	FLOAT32 each_spoke_step = 360.0f / 4096;
	FLOAT32 each_cell_size = p_current->ScanRangeMeter / 1024.0f;
	CurrentRequest.RemoveAt(0);


	auto* p_current_sector = p_current->GetSectorContainer()->GetSector(p_current->CurrentSector);


	int SpokeCountPerSector = 4096 / p_current->SectorCount;

	if (p_current->CurrentSector == 0 && SpokeSequanceNumber != 0) {
		SpokeSequanceNumber = 0;
	}

	Mutex.Unlock();

	for (int i = 0; i < SpokeCountPerSector; i++) {


		//SRadarSimSDKPacket pack;
		memset(&packspoke, 0, sizeof(SRadarSimSDKPacket));

		SHalo24SpokePayload* p_spoke_payload = (SHalo24SpokePayload*)packspoke.Payload;

		S9174SpokeHeaderNoneBitField* p_hdr = &(p_spoke_payload->HeaderNoneBitField);
		p_hdr->spokeLength_bytes = 536;
		p_hdr->sequenceNumber = SpokeSequanceNumber;
		p_hdr->sampleEncoding = 0;
		p_hdr->nOfSamples = 1024;
		p_hdr->bitsPerSample = 4;
		p_hdr->rangeCellSize_mm = (each_cell_size) * 1000;
		p_hdr->spokeAzimuth = SpokeSequanceNumber;
		p_hdr->bearingZeroError = 0;
		p_hdr->spokeCompass = p_current->ScanRPYWorld.Z / 4096.0f;
		p_hdr->trueNorth = 1;
		p_hdr->rangeCellsDiv2 = p_current->ScanRangeMeter / (each_cell_size) * 0.5f;

		auto azim_deg = p_hdr->spokeAzimuth * 360.0 / 4096;

		bool ret = p_current_sector->MapSpoke4Bits(p_current->ScanCenter, p_current_sector->StartAzimuthDeg + i * each_spoke_step, each_cell_size, 512, p_spoke_payload->SpokeData.Data);

		if (ret) {
			packspoke.SetID(ESimSDKDataIDS::SpokeData);
			p_spoke_payload->SerialData.SetSerial(pHostIF->GetSerial(), strlen(pHostIF->GetSerial()));
			packspoke.SetPayload(ESimSDKDataIDS::SpokeData, (INT8U*)packspoke.Payload, sizeof(SHalo24SpokePayload));
			//pack.SetPayloadSize(sizeof(SHalo24SpokePayload));
			pUDPConnection->SendUDPData((const INT8U*)&packspoke, packspoke.GetTransmitSize());

		
		}
		else {
			break;
		}
		
		CUtil::DebugLog("SpokeSequanceNumber: " + CUtil::IntToString(SpokeSequanceNumber)+ "azim: "+ CUtil::FloatToString(azim_deg));
		SpokeSequanceNumber++;

		if (PrevSequenceNumber >= 0) {
			if ((SpokeSequanceNumber - PrevSequenceNumber) != 1) {
				CUtil::DebugLog("Spoke Sequenc Error prev" + CUtil::IntToString(PrevSequenceNumber) + " curr: " + CUtil::IntToString(SpokeSequanceNumber));
			}
		}

		if (SpokeSequanceNumber >= 4096) {
			SpokeSequanceNumber = 0;
		}

		PrevSequenceNumber = SpokeSequanceNumber;

	}

}

void UHalo24CommIF::Stop()
{
	IsStoped = true;
}

SRadarSimSDKPacket* UHalo24CommIF::UHalo24CommIF::GetPacket()
{
	SRadarSimSDKPacket* p_pack = Packets[0];
	Packets.RemoveAt(0);

	return p_pack;

}
void UHalo24CommIF::SendResponseAckNack(ESimSDKDataIDS id, char* p_serial, bool is_ack, INT8U stream_no)
{
	SRadarSimSDKPacket pack;
	pack.SetResponse(id, p_serial, is_ack, stream_no);

	pUDPConnection->SendUDPData((const INT8U*) & pack, pack.GetTransmitSize());
}
void UHalo24CommIF::SendRadarState(ERadarState radar_state, char* p_serial)
{
	SRadarState state;

	memset(&state, 0, sizeof(state));
	state.SerialData.SetSerial(p_serial, strlen(p_serial));
	SRadarSimSDKPacket pack;
	pack.SetPayload(ESimSDKDataIDS::RadarState, (INT8U*)&state, sizeof(SRadarState));
	pUDPConnection->SendUDPData((const INT8U*)&pack, pack.GetTransmitSize());

}
void UHalo24CommIF::SendRadarSetup(const SRadarSetupData& setup, char *p_serial)
{

	SRadarSimSDKPacket pack;
	SRadarSetupPayload payload;
	memcpy(&payload.RadarSetupData, &setup, sizeof(SRadarSetupData));
	payload.SerialData.SetSerial(p_serial, strlen(p_serial));
	pack.SetPayload(ESimSDKDataIDS::RadarSetup, (INT8U*)&payload, sizeof(SRadarSetupPayload));
	pUDPConnection->SendUDPData((const INT8U*)&pack, pack.GetTransmitSize());
}

void UHalo24CommIF::SendTrackedObjects(const STargetTrackStatusData& info, char* p_serial)
{
	for (auto p_info : *(info.Tracks)) {
		STrackingTargetStatusPayload payload;
		SRadarSimSDKPacket pack;
		

		payload.TargetData.targetID = p_info->ClientId;
		payload.TargetData.serverTargetID = p_info->TrackerId;
			 
		payload.TargetData.infoAbsolute.distance_m = p_info->AbsoluteDistanceMeter;
		payload.TargetData.infoAbsolute.bearing_ddeg = (360 - p_info->AbsoluteBearingDeg)*10; // ship coordinate system
		payload.TargetData.infoAbsolute.speed_dmps = p_info->AbsoulteTargetSpeedMetersPerSec * 10; // ship coordinate system
		payload.TargetData.infoAbsolute.course_ddeg = (360 - p_info->AbsoulteTargetCourseDeg) * 10;
			 
		payload.TargetData.infoRelative.distance_m = p_info->RelativeDistanceMeter;
		payload.TargetData.infoRelative.bearing_ddeg = (360 - p_info->RelativeBearingDeg) * 10; // ship coordinate system
		payload.TargetData.infoRelative.speed_dmps = p_info->RelativeTargetSpeedMetersPerSec * 10; // ship coordinate system
		payload.TargetData.infoRelative.course_ddeg = (360 - p_info->RelativeTargetCourseDeg) * 10;

		payload.TargetData.infoAbsoluteValid = 1;

		switch (p_info->TrackState) {
		case 	EObjectTrackState::Acquiring:
			payload.TargetData.targetState = eTargetState::eAcquiringTarget;
			break;
		case EObjectTrackState::AcquiredAndSafe:
		case EObjectTrackState::AcquiredSafeAndTemprorayLoss:
			payload.TargetData.targetState = eTargetState::eSafeTarget;
			break;
		case EObjectTrackState::AcquiredAndDangerous:
		case EObjectTrackState::AcquiredDangerousAndTemprorayLoss:
			payload.TargetData.targetState = eTargetState::eDangerousTarget;
			break;
		case EObjectTrackState::LostTarget:
			payload.TargetData.targetState = eTargetState::eLostTarget;
			break;
		case EObjectTrackState::AcquireFailure:
			payload.TargetData.targetState = eTargetState::eAcquireFailure;
			break;
		case EObjectTrackState::OutOfRange:
			payload.TargetData.targetState = eTargetState::eOutOfRange;
			break;
		case EObjectTrackState::LostOutOfRange:
			payload.TargetData.targetState = eTargetState::eLostOutOfRange;
			break;
		case EObjectTrackState::AquireFailedTargetTrackCapacityFull:
			break;
	
		}
		
		payload.TargetData.CPA_m = info.ClosestPointOfApproachMeters;
		payload.TargetData.TCPA_sec = info.TimeToClosestPointOfApproachSec;
		payload.TargetData.towardsCPA = info.TowardsCPA;

		payload.SerialData.SetSerial(p_serial, strlen(p_serial));
		pack.SetID(ESimSDKDataIDS::TrackingStatus);
		pack.SetPayload((INT8U*) & payload, sizeof(STrackingTargetStatusPayload));


		pUDPConnection->SendUDPData((const INT8U*)&pack, pack.GetTransmitSize());


	}
}
void UHalo24CommIF::RestorePacket(SRadarSimSDKPacket* p_pack)
{
	Packets.Add(p_pack);
}

void UHalo24CommIF::OnReceivedConnectionData(void* connection, INT8U* p_data, INT32U count)
{
	Super::OnReceivedConnectionData(connection, p_data, count);
	SRadarSimSDKPacket* p_pack = (SRadarSimSDKPacket*)p_data;
	p_pack->Init();

	if (pHostIF) {
		pHostIF->OnRecievedMessage(p_pack);
	}
	


}
