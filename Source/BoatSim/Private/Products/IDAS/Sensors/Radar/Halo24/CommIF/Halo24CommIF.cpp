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
	//if (p_in->IsFullScanned()) {
		//p_in->CurrentSector = 0;
		CurrentRequest.Add(p_new);
	//}
	
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

	pUDPConnection->SendData((INT8U*)pack, pack->GetTransmitSize());

	RestorePacket(pack);
}

void UHalo24CommIF::BeginPlay()
{
	Super::BeginPlay();
	pUDPConnection->AddConnectionDataReceiver(this);
	for (int i = 0; i < 16; i++) {
		Packets.Add(new SRadarSimSDKPacket());
	}

	//SpokeImage = CreateImage(2048, 2048);
	memset(SpokeImage, 0, sizeof(SpokeImage));

	SenderThread = FRunnableThread::Create(this, *(GetOwner()->GetName()));
	RegisterConnectionInfo(0, pUDPConnection->GetConnectionInfo(), pUDPConnection);



	FString ImagePath = FPaths::ProjectDir() / TEXT("Textures/Radar/RadarNoise.png");

	if (LoadPNGImageAsFColorArray(ImagePath, NoiseTexture.Colors, NoiseTexture.Width, NoiseTexture.Height))
	{
		// Successfully loaded the image, Colors array now contains the pixel data
		// Width and Height contain the dimensions of the image
	}
	else
	{
		// Failed to load the image
	}

	RadarNoiseTextureSpeed.X = 2;
	RadarNoiseTextureSpeed.Y = 1.5;

}
INT8U** UHalo24CommIF::CreateImage(int width, int height) {
	// Allocate memory for the array of row pointers
	INT8U** image = new INT8U * [height];

	// Allocate memory for each row
	for (int i = 0; i < height; ++i) {
		image[i] = new INT8U[width];
	}

	// Initialize the array with zeros
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			image[i][j] = 0;
		}
	}

	return image;
}

bool UHalo24CommIF::LoadPNGImageAsFColorArray(const FString& ImagePath, TArray<FColor>& OutColors, int32& OutWidth, int32& OutHeight)
{
	TArray<uint8> RawFileData;
	if (!FFileHelper::LoadFileToArray(RawFileData, *ImagePath))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load image file: %s"), *ImagePath);
		return false;
	}

	// Get the image wrapper module and create a wrapper for the PNG image
	IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
	TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG);

	// Decode the image
	if (ImageWrapper.IsValid() && ImageWrapper->SetCompressed(RawFileData.GetData(), RawFileData.Num()))
	{
		TArray<uint8> RawData;
		if (ImageWrapper->GetRaw(ERGBFormat::BGRA, 8, RawData))
		{
			OutWidth = ImageWrapper->GetWidth();
			OutHeight = ImageWrapper->GetHeight();
			OutColors.SetNumUninitialized(OutWidth * OutHeight);

			// Convert the raw data to FColor array
			for (int32 i = 0; i < RawData.Num(); i += 4)
			{
				int32 Index = i / 4;
				OutColors[Index] = FColor((RawData)[i + 2], (RawData)[i + 1], (RawData)[i], (RawData)[i + 3]);
			}

			return true;
		}
	}

	UE_LOG(LogTemp, Error, TEXT("Failed to decode image file: %s"), *ImagePath);
	return false;
}
//SRadarSimSDKPacket packspoke;
#if false
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
			pUDPConnection->SendData((const INT8U*)&packspoke, packspoke.GetTransmitSize());		
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
#endif


#if true
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
		ARadarBase* p_radar = (ARadarBase*)pHostIF->GetOwningActor();
		float measurement_noise_mag = p_radar->MeasurementErrorUncertainy;
		double amp = FMath::Lerp(5, 20, measurement_noise_mag / 50);
		double noise_depedent_gain_loss = FMath::Lerp(0, 50, measurement_noise_mag / 50);
		RadarTimeVaryingGain = amp *FMath::Sin(2 * PI * Freq * CTime::GetTimeSecond());
		//ApplyMerge(8);
		ApplyGain(pHostIF->GetGain()- noise_depedent_gain_loss + RadarTimeVaryingGain);
		
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
			p_spoke_payload->SerialData.SetSerial(pHostIF->GetSerial(), strlen(pHostIF->GetSerial()));
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
#endif

void UHalo24CommIF::ApplyGain(double gain_level)
{
	if (gain_level < 0) {
		gain_level = 0;
	}
	auto start_time = CTime::GetTimeSecond();
	float one_over_size = 1.0 / SpokeImageSize;
	INT8U gain_threshold = FMath::Clamp(gain_level, 0, 255);
	int texture_shift_w = FMath::Fmod(CTime::GetTimeSecond() * RadarNoiseTextureSpeed.X, NoiseTexture.Width);
	int texture_shift_h = FMath::Fmod(CTime::GetTimeSecond() * RadarNoiseTextureSpeed.Y, NoiseTexture.Height);

	double wind_speed;
	ASystemManagerBase::GetInstance()->GetWindSpeedMeterPerSec(0, wind_speed);
	int max_sea_clutter_point = 300 * (1 + wind_speed);

	int sea_clutter_pt = FMath::Lerp(max_sea_clutter_point, max_sea_clutter_point * 0.001, pHostIF->GetSeaClutterLevel()/255.0);
	

	double rain_percent;
	
    ASystemManagerBase::GetInstance()->GetRainPercent(0, rain_percent);
	int max_rain_pt = FMath::Lerp(0, 5000,  rain_percent/100);

	int rain_clutter_pt = FMath::Lerp(max_rain_pt, 0, pHostIF->GetRainClutterLevel() / 255.0);

	INT64S dist_square;
	int half_of_spoke_image = SpokeImageSize / 2;
	double one_over_diagonal_square = 1.0/(SpokeImageSize * SpokeImageSize);


	for (int h = 0; h < SpokeImageSize; h++) {
		for (int w = 0; w < SpokeImageSize; w++) {
			dist_square = (h - half_of_spoke_image) * (h - half_of_spoke_image) + (w - half_of_spoke_image) * (w - half_of_spoke_image);
			double tf = dist_square * one_over_diagonal_square;
			

			int val = NoiseTexture.GetColor(((w + texture_shift_w) % NoiseTexture.Width) * one_over_size, ((h + texture_shift_h) % NoiseTexture.Height) * one_over_size);

			if (val > (gain_threshold - tf * 20)) {
				SpokeImage[h][w] = 0;
			}

		}
	}

	for (int i = 0; i < sea_clutter_pt; i++) {
		int w = FMath::RandRange(0, SpokeImageSize-1);
		int h = FMath::RandRange(0, SpokeImageSize-1);
		SpokeImage[h][w] = 1; // ~SpokeImage[h][w];
	}

	for (int i = 0; i < rain_clutter_pt; i++) {
		int w = FMath::RandRange(0, SpokeImageSize-1);
		int h = FMath::RandRange(0, SpokeImageSize-1);
		SpokeImage[h][w] = 1;// ~SpokeImage[h][w];
	}

	auto end_time = CTime::GetTimeSecond();
	CUtil::DebugLog("ApplyGain (ms): " + CUtil::FloatToString((end_time - start_time) * 1000));
}

void UHalo24CommIF::ApplyMerge(int size)
{
	auto start_time = CTime::GetTimeSecond();

	
	for (int h = 0; h < SpokeImageSize - size; h++) {
		for (int w = 0; w < SpokeImageSize - size; w++) {
			bool is_on = false;

			for (int i = 0; i < size; i++) {
				for (int j = 0; j < size; j++) {
					if (SpokeImage[h+i][w+j] > 0) {
						is_on = true;
						break;
					}
				}
			}
			if (is_on) {
				for (int i = 0; i < size; i++) {
					for (int j = 0; j < size; j++) {
						SpokeImage[h + i][w + j] = 1;
					}
				}
			}			
		
		}
	}
	auto end_time = CTime::GetTimeSecond();
	CUtil::DebugLog("ApplyMerge (ms): " + CUtil::FloatToString((end_time - start_time) * 1000));

}

void UHalo24CommIF::SpokeToImage(FLOAT64 spoke_azimuth_deg, INT8U* p_spoke, INT32U num_samples)
{
	auto angle_rad = spoke_azimuth_deg * DEGTORAD;

	double ratio = (0.5 * SpokeImageSize) / num_samples;

	for (int i = 0; i < (int)num_samples; i++) {

		int byte_ind = i>>1;
		int order = i & 0x1;

		unsigned char merged = p_spoke[byte_ind];
		unsigned char data = 0;

		if (order==0) {
			data = merged & 0xF;
		}
		else {
			data = (merged >> 4) & 0xf;
		}

		

		double x = FMath::Sin(angle_rad) * i* ratio + SpokeImageSize/2;
		double y = FMath::Cos(angle_rad) * i* ratio + SpokeImageSize/2;

		SpokeImage[(int)y][(int)x] = data;

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

void UHalo24CommIF::SendTrackedObjects(const STargetTrackStatusData& info, char* p_serial)
{
	for (auto p_info : *(info.Tracks)) {
		STrackingTargetStatusPayload payload;
		SRadarSimSDKPacket pack;
		
		payload.TargetData.targetValid = 1;
		payload.TargetData.targetID = p_info->TrackerId; //p_info->ClientId; //todo fixme id karmaşası
		payload.TargetData.serverTargetID = p_info->TrackerId;
			 
		payload.TargetData.infoAbsolute.distance_m = p_info->AbsoluteDistanceMeter;
		payload.TargetData.infoAbsolute.bearing_ddeg = (p_info->AbsoluteBearingDeg)*10; // ship coordinate system
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
		pack.SetPayload((INT8U*) & payload, sizeof(STrackingTargetStatusPayload));


		pUDPConnection->SendData((const INT8U*)&pack, pack.GetTransmitSize());


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
