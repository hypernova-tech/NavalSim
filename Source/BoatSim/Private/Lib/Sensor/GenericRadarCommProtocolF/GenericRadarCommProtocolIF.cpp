﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/Sensor/GenericRadarCommProtocolF/GenericRadarCommProtocolIF.h"




#include <Lib/Utils/CUtil.h>
#include <Lib/Time/CTime.h>
#include "IImageWrapperModule.h"
#include "IImageWrapper.h"
#include "Modules/ModuleManager.h"
#include "Engine/Texture2D.h"
#include "Misc/FileHelper.h"
#include <Lib/SystemManager/SystemManagerBase.h>
#include "Products/IDAS/Sensors/Radar/Halo24/CommIF/Halo24CommIF.h"

UGenericRadarCommProtocolIF::~UGenericRadarCommProtocolIF()
{
	if (SenderThread != nullptr) {
		SenderThread->Kill();
		delete SenderThread;
	}
}

void UGenericRadarCommProtocolIF::SendData(void* p_data, uint32 size_in_bytes)
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



uint32 UGenericRadarCommProtocolIF::Run()
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

void UGenericRadarCommProtocolIF::SetHostIF(IHostIF* p_val)
{
	Super::SetHostIF(p_val);
	pRadarHostIF = (IRadarHostIF*) p_val;
}


void UGenericRadarCommProtocolIF::BeginPlay()
{
	Super::BeginPlay();
	pUDPConnection->AddConnectionDataReceiver(this);
	
	
	

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
INT8U** UGenericRadarCommProtocolIF::CreateImage(int width, int height) {
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

bool UGenericRadarCommProtocolIF::LoadPNGImageAsFColorArray(const FString& ImagePath, TArray<FColor>& OutColors, int32& OutWidth, int32& OutHeight)
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


#if true
void UGenericRadarCommProtocolIF::SendRadarTrack()
{

#if true
	Mutex.Lock();

	SScanResult* p_current = CurrentRequest[0];
	bool is_full_scanned = p_current->CurrentSector == (p_current->SectorCount - 1);
	auto azim_range_deg = p_current->HorizontalFovDeg;

	INT32S total_spoke = (INT32S)(azim_range_deg / p_current->ScanAzimuthStepDeg + 0.5);



	FLOAT32 each_spoke_step = p_current->HorizontalFovDeg / total_spoke;
	FLOAT32 each_cell_size = p_current->ScanRangeMeter / 1024.0f;
	CurrentRequest.RemoveAt(0);


	auto* p_current_sector = p_current->GetSectorContainer()->GetSector(p_current->CurrentSector);


	int SpokeCountPerSector = total_spoke / p_current->SectorCount;

	if (p_current->CurrentSector == 0 && SpokeSequanceNumber != 0) {
		SpokeSequanceNumber = 0;
	}

	Mutex.Unlock();
	ARadarBase* p_radar = (ARadarBase*)pRadarHostIF->GetOwningActor();

	TArray<FVector> points;
	for (int i = 0; i < SpokeCountPerSector; i++) {


		points.Reset();
		
		bool ret = p_current_sector->MapSpokePointCloud(p_current->ScanCenter, p_current_sector->StartAzimuthDeg + i * each_spoke_step, each_cell_size, 512, points);

		if (ret) {
			SpokePointCloudToImage(p_current_sector->StartAzimuthDeg + i * each_spoke_step, p_current->ScanCenter, p_current->ScanRangeMeter, each_cell_size, points, 1024);
		}
		else {
			break;
		}
	}
	auto start_time = CTime::GetTimeSecond();

	int CaptureInd = 0;

	if (is_full_scanned) {

		// apply noise filter according to gain level
		float Freq = 0.1f;
		
		float measurement_noise_mag = p_radar->MeasurementErrorUncertainy;
		double amp = FMath::Lerp(5, 20, measurement_noise_mag / 50);
		double noise_depedent_gain_loss = FMath::Lerp(0, 50, measurement_noise_mag / 50);
		RadarTimeVaryingGain = amp * FMath::Sin(2 * PI * Freq * CTime::GetTimeSecond());
		//ApplyMerge(8);
		ApplyGain(pRadarHostIF->GetGain() - noise_depedent_gain_loss + RadarTimeVaryingGain);


		SPointCloudInfoPayload info;
	
		int cloud_data_ind = 0;
		int entry_index_in_payload = 0;

		S3DPointCloudMessage cloud_data_mes;
		cloud_data_mes.Reset();


		for (int i = 0; i < total_spoke; i++) {
			//memset(&packspoke, 0, sizeof(SRadarSimSDKPacket));

			//SHalo24SpokePayload* p_spoke_payload = (SHalo24SpokePayload*)packspoke.Payload;

			info.PointCount = total_spoke;
			info.CaptureInd = i;
			auto scan_center_w = TOW(p_current->ScanCenter);
			info.ScanCenterX = scan_center_w.X;
			info.ScanCenterY = scan_center_w.Y;
			info.ScanCenterZ = scan_center_w.Z;
			info.IsPoint3D = 1;
			S3DPointCloudMessage message;
			message.SetMessage(EPointCloudId::PointCloudInfo, (INT8U*) & info, sizeof(info));
			pUDPConnection->SendData((const INT8U*)&message, message.GetMessageSize());

			
			auto azim_deg = i * p_current->HorizontalFovDeg / total_spoke;
			auto angle_rad = azim_deg * DEGTORAD;

			auto cos_of_ang = FMath::Cos(angle_rad);
			auto sin_of_ang = FMath::Sin(angle_rad);

			double ratio = (0.5 * SpokeImageSize) / 1024;	

			S3DPointCloudDataPayload* p_point_cloud_data = (S3DPointCloudDataPayload*)cloud_data_mes.GetPayload();			

			for (int j = 0; j < 1024; j++) {

				double x = sin_of_ang * j * ratio;
				double y = cos_of_ang * j * ratio;

				int im_x = SpokeImageSize / 2 + x;
				int im_y = SpokeImageSize / 2 + y;

				int byte_ind = j >> 1;
				int order = j & 0x1;

				
				auto p_entry = &p_point_cloud_data->Entries[entry_index_in_payload];
				p_entry->CaptureInd = PointCloudCaptureInd;
				p_entry->DataIndex = cloud_data_ind;
				FVector *p_pos = &SpokeImage3D[im_y][im_x];
				INT8U im_val = SpokeImage[im_y][im_x];

				if (im_val) {
					if (p_pos->X == 0) { // noise ile  eklenmiş bir veri seti olabilir

						p_entry->SetPoint(x * p_current->ScanRangeMeter / 1024, y * p_current->ScanRangeMeter / 1024, 0);
				
					}
					else {
						p_entry->SetPoint(p_pos->X, p_pos->Y, p_pos->Z);
					}		

				}
				else {
					p_entry->SetPoint(0, 0, 0);
				}
				entry_index_in_payload++;
				cloud_data_ind++;

				if (entry_index_in_payload == POINT_CLOUND_ENTRY_COUNT) {
					cloud_data_mes.SetMessageId(EPointCloudId::PointCloudData);
					cloud_data_mes.SetDataLength(entry_index_in_payload * sizeof(S3DPointCloudDataPayloadEntry));
					bool ret = pUDPConnection->SendData((const INT8U*)&cloud_data_mes, cloud_data_mes.GetMessageSize());
					entry_index_in_payload = 0;
				}
			}
		
		}

		if (entry_index_in_payload != 0) {
			cloud_data_mes.SetMessageId(EPointCloudId::PointCloudData);
			cloud_data_mes.SetDataLength(entry_index_in_payload * sizeof(S3DPointCloudDataPayloadEntry));
			bool ret = pUDPConnection->SendData((const INT8U*)&cloud_data_mes, cloud_data_mes.GetMessageSize());
			entry_index_in_payload = 0;
		}

		auto elp_time = CTime::GetTimeSecond() - start_time;
		CUtil::DebugLog("Total Radar Proc Sec: " + CUtil::FloatToString(elp_time * 1000) + "ms");
		memset(SpokeImage, 0, sizeof(SpokeImage));

	}

	PointCloudCaptureInd++;

#endif
}
#endif

void UGenericRadarCommProtocolIF::ApplyGain(double gain_level)
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

	int sea_clutter_pt = FMath::Lerp(max_sea_clutter_point, max_sea_clutter_point * 0.001, pRadarHostIF->GetSeaClutterLevel() / 255.0);


	double rain_percent;

	ASystemManagerBase::GetInstance()->GetRainPercent(0, rain_percent);
	int max_rain_pt = FMath::Lerp(0, 5000, rain_percent / 100);

	int rain_clutter_pt = FMath::Lerp(max_rain_pt, 0, pRadarHostIF->GetRainClutterLevel() / 255.0);

	INT64S dist_square;
	int half_of_spoke_image = SpokeImageSize / 2;
	double one_over_diagonal_square = 1.0 / (SpokeImageSize * SpokeImageSize);


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
		int w = FMath::RandRange(0, SpokeImageSize - 1);
		int h = FMath::RandRange(0, SpokeImageSize - 1);
		SpokeImage[h][w] = 1; // ~SpokeImage[h][w];
	}

	for (int i = 0; i < rain_clutter_pt; i++) {
		int w = FMath::RandRange(0, SpokeImageSize - 1);
		int h = FMath::RandRange(0, SpokeImageSize - 1);
		SpokeImage[h][w] = 1;// ~SpokeImage[h][w];
	}

	auto end_time = CTime::GetTimeSecond();
	CUtil::DebugLog("ApplyGain (ms): " + CUtil::FloatToString((end_time - start_time) * 1000));
}

void UGenericRadarCommProtocolIF::ApplyMerge(int size)
{
	auto start_time = CTime::GetTimeSecond();


	for (int h = 0; h < SpokeImageSize - size; h++) {
		for (int w = 0; w < SpokeImageSize - size; w++) {
			bool is_on = false;

			for (int i = 0; i < size; i++) {
				for (int j = 0; j < size; j++) {
					if (SpokeImage[h + i][w + j] > 0) {
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


void UGenericRadarCommProtocolIF::SpokePointCloudToImage(FLOAT64 spoke_azimuth_deg, FVector scan_center, FLOAT64 scan_range_meter, FLOAT64 cell_size_meter, TArray<FVector>& point_cloud, INT32U num_samples)
{
	auto angle_rad = spoke_azimuth_deg * DEGTORAD;

	double ratio = (0.5 * SpokeImageSize) / num_samples;

	for (int i = 0; i < point_cloud.Num(); i++) {
		const FVector& pos = point_cloud[i];


		double dist = TOW((pos - scan_center).Length());
		int j = dist / scan_range_meter * num_samples;

		double x = FMath::Sin(angle_rad) * j * ratio + SpokeImageSize / 2;
		double y = FMath::Cos(angle_rad) * j * ratio + SpokeImageSize / 2;

		SpokeImage[(int)y][(int)x] = 1;
		SpokeImage3D[(int)y][(int)x] = pos- scan_center;
	}

}

void UGenericRadarCommProtocolIF::SpokeToImage(FLOAT64 spoke_azimuth_deg, INT8U* p_spoke, INT32U num_samples)
{
	auto angle_rad = spoke_azimuth_deg * DEGTORAD;

	double ratio = (0.5 * SpokeImageSize) / num_samples;

	for (int i = 0; i < (int)num_samples; i++) {

		int byte_ind = i >> 1;
		int order = i & 0x1;

		unsigned char merged = p_spoke[byte_ind];
		unsigned char data = 0;

		if (order == 0) {
			data = merged & 0xF;
		}
		else {
			data = (merged >> 4) & 0xf;
		}



		double x = FMath::Sin(angle_rad) * i * ratio + SpokeImageSize / 2;
		double y = FMath::Cos(angle_rad) * i * ratio + SpokeImageSize / 2;

		SpokeImage[(int)y][(int)x] = data;

	}

}

void UGenericRadarCommProtocolIF::Stop()
{
	IsStoped = true;
}



void UGenericRadarCommProtocolIF::SendTrackedObjects(const STargetTrackStatusData& info, char* p_serial)
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

void UGenericRadarCommProtocolIF::OnReceivedConnectionData(void* connection, INT8U* p_data, INT32U count)
{

}
