// Fill out your copyright notice in the Description page of Project Settings.


#include "Products/IDAS/Sensors/Radar/Halo24/CommIF/Halo24CommIF.h"
#include "HAL/PlatformNamedPipe.h"
#include <Lib/Utils/CUtil.h>
#include <Lib/Time/CTime.h>
#include "IImageWrapperModule.h"
#include "IImageWrapper.h"
#include "Modules/ModuleManager.h"
#include "Engine/Texture2D.h"
#include "Misc/FileHelper.h"
#include <Lib/SystemManager/SystemManagerBase.h>




void UHalo24CommIF::BeginPlay()
{
	Super::BeginPlay();

	for (int i = 0; i < 16; i++) {
		Packets.Add(new SRadarSimSDKPacket());
	}
}

void UHalo24CommIF::SendSerial(INT8U* p_data, INT32U count)
{
	auto pack = GetPacket();
	pack->SetPayload(ESimSDKDataIDS::Serials, p_data, count);

	pUDPConnection->SendData((INT8U*)pack, pack->GetTransmitSize());

	RestorePacket(pack);
}




void UHalo24CommIF::SendRadarTrack()
{
	Mutex.Lock();

	SScanResult* p_current = CurrentRequest[0];
	bool is_full_scanned = p_current->CurrentSector == (p_current->SectorCount - 1);


	FLOAT32 each_spoke_step = 360.0f / 4096;
	FLOAT32 each_cell_size = p_current->ScanRangeMeter / 1024.0f;
	CurrentRequest.RemoveAt(0);


	auto* p_current_sector = p_current->GetSectorContainer()->GetSector(p_current->CurrentSector);


	int SpokeCountPerSector = 4096 / p_current->SectorCount;

	if (p_current->CurrentSector == 0 && SpokeSequanceNumber != 0) {
		SpokeSequanceNumber = 0;
	}

	Mutex.Unlock();

	SRadarSimSDKPacket  packspoke;
	for (int i = 0; i < SpokeCountPerSector; i++) {
		
		memset(&packspoke, 0, sizeof(SRadarSimSDKPacket));

		SHalo24SpokePayload* p_spoke_payload = (SHalo24SpokePayload*)packspoke.Payload;		

		bool ret = p_current_sector->MapSpoke4Bits(p_current->ScanCenter, p_current_sector->StartAzimuthDeg + i * each_spoke_step, each_cell_size, 512, p_spoke_payload->SpokeData.Data);

		if (ret) {
			SpokeToImage(p_current_sector->StartAzimuthDeg + i * each_spoke_step, p_spoke_payload->SpokeData.Data, 1024);
		}
		else {
			break;
		}



	}
	auto start_time = CTime::GetTimeSecond();

	

	if (is_full_scanned) {

		// apply noise filter according to gain level
		float Freq = 0.1f;
		ARadarBase* p_radar = (ARadarBase*)pRadarHostIF->GetOwningActor();
		float measurement_noise_mag = p_radar->MeasurementErrorUncertainy;
		double amp = FMath::Lerp(5, 20, measurement_noise_mag / 50);
		double noise_depedent_gain_loss = FMath::Lerp(0, 50, measurement_noise_mag / 50);
		RadarTimeVaryingGain = amp *FMath::Sin(2 * PI * Freq * CTime::GetTimeSecond());
		//ApplyMerge(8);
		ApplyGain(pRadarHostIF->GetGain()- noise_depedent_gain_loss + RadarTimeVaryingGain);
		
		for (int i = 0; i < 4096; i++) {
			//memset(&packspoke, 0, sizeof(SRadarSimSDKPacket));

			//SHalo24SpokePayload* p_spoke_payload = (SHalo24SpokePayload*)packspoke.Payload;

			SRadarSimSDKPacket* p_curr_pack = &packspoke;;
			memset(p_curr_pack, 0, sizeof(SRadarSimSDKPacket));
			SHalo24SpokePayload* p_spoke_payload = (SHalo24SpokePayload*)p_curr_pack->Payload;
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
			INT8U* p_data = p_spoke_payload->SpokeData.Data;
			auto angle_rad = azim_deg * DEGTORAD;

			auto cos_of_ang = FMath::Cos(angle_rad);
			auto sin_of_ang = FMath::Sin(angle_rad);

			double ratio = (0.5 * SpokeImageSize) / 1024;

			for (int j = 0; j < 1024; j++) {

				double x = sin_of_ang * j * ratio;
				double y = cos_of_ang * j * ratio;

				int im_x = SpokeImageSize/2 + x;
				int im_y = SpokeImageSize/2 + y;

				int byte_ind = j >> 1;
				int order = j & 0x1;

				
				if (order == 0) {
					p_data[byte_ind] = SpokeImage[im_y][im_x];
				}
				else {
					p_data[byte_ind] |= (SpokeImage[im_y][im_x])<<4;
				}		

			}			

			p_curr_pack->SetID(ESimSDKDataIDS::SpokeData);
			p_spoke_payload->SerialData.SetSerial(pRadarHostIF->GetSerial(), strlen(pRadarHostIF->GetSerial()));
			p_curr_pack->SetPayload(ESimSDKDataIDS::SpokeData, (INT8U*)p_curr_pack->Payload, sizeof(SHalo24SpokePayload));
			bool ret = pUDPConnection->SendData((const INT8U*)p_curr_pack, p_curr_pack->GetTransmitSize());
			if (!ret) {
				ret = ret;
			}

			SpokeSequanceNumber++;

			if (SpokeSequanceNumber >= 4096) {
				SpokeSequanceNumber = 0;				
			}
		}

		auto elp_time = CTime::GetTimeSecond() - start_time;
		CUtil::DebugLog("Total Radar Proc Sec: " + CUtil::FloatToString(elp_time * 1000) + "ms");
		memset(SpokeImage, 0, sizeof(SpokeImage));
	
	}

}



SRadarSimSDKPacket* UHalo24CommIF::GetPacket()
{
	SRadarSimSDKPacket* p_pack = Packets[0];
	Packets.RemoveAt(0);

	return p_pack;

}
void UHalo24CommIF::SendResponseAckNack(ESimSDKDataIDS id, char* p_serial, bool is_ack, INT8U stream_no)
{
	SRadarSimSDKPacket pack;
	pack.SetResponse(id, p_serial, is_ack, stream_no);

	pUDPConnection->SendData((const INT8U*) & pack, pack.GetTransmitSize());
}
void UHalo24CommIF::SendRadarState(ERadarState radar_state, char* p_serial)
{
	SRadarState state;

	memset(&state, 0, sizeof(state));
	state.SerialData.SetSerial(p_serial, strlen(p_serial));
	SRadarSimSDKPacket pack;
	pack.SetPayload(ESimSDKDataIDS::RadarState, (INT8U*)&state, sizeof(SRadarState));
	pUDPConnection->SendData((const INT8U*)&pack, pack.GetTransmitSize());

}
void UHalo24CommIF::SendRadarSetup(const SRadarSetupData& setup, char *p_serial)
{

	SRadarSimSDKPacket pack;
	SRadarSetupPayload payload;
	memcpy(&payload.RadarSetupData, &setup, sizeof(SRadarSetupData));
	payload.SerialData.SetSerial(p_serial, strlen(p_serial));
	pack.SetPayload(ESimSDKDataIDS::RadarSetup, (INT8U*)&payload, sizeof(SRadarSetupPayload));
	pUDPConnection->SendData((const INT8U*)&pack, pack.GetTransmitSize());
}

void UHalo24CommIF::SetHostIF(IHostIF* p_host_if)
{
	Super::SetHostIF(p_host_if);
	pRadarHostIF = (IRadarHostIF*)p_host_if;
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

void UHalo24CommIF::SendTrackedObjects(const STargetTrackStatusData& info, char* p_serial)
{
	for (auto p_info : *(info.Tracks)) {
		STrackingTargetStatusPayload payload;
		SRadarSimSDKPacket pack;

		payload.TargetData.targetValid = 1;
		payload.TargetData.targetID = p_info->TrackerId; //p_info->ClientId; //todo fixme id karmaşası
		payload.TargetData.serverTargetID = p_info->TrackerId;

		payload.TargetData.infoAbsolute.distance_m = p_info->AbsoluteDistanceMeter;
		payload.TargetData.infoAbsolute.bearing_ddeg = (p_info->AbsoluteBearingDeg) * 10; // ship coordinate system
		payload.TargetData.infoAbsolute.speed_dmps = p_info->AbsoulteTargetSpeedMetersPerSec * 10; // ship coordinate system
		payload.TargetData.infoAbsolute.course_ddeg = (p_info->AbsoulteTargetCourseDeg) * 10;

		payload.TargetData.infoRelative.distance_m = p_info->RelativeDistanceMeter;
		payload.TargetData.infoRelative.bearing_ddeg = (p_info->RelativeBearingDeg) * 10; // ship coordinate system
		payload.TargetData.infoRelative.speed_dmps = p_info->RelativeTargetSpeedMetersPerSec * 10; // ship coordinate system
		payload.TargetData.infoRelative.course_ddeg = (p_info->RelativeTargetCourseDeg) * 10;

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
		pack.SetPayload((INT8U*)&payload, sizeof(STrackingTargetStatusPayload));


		pUDPConnection->SendData((const INT8U*)&pack, pack.GetTransmitSize());


	}
}

