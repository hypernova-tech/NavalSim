// Fill out your copyright notice in the Description page of Project Settings.


#include "Products/IDAS/Sensors/Lidar/RobosenseM1/RobosenseM1CommIF.h"
#include "Products/IDAS/Sensors/Lidar/RobosenseM1/RobosenseM1Types.h"
#include <Lib/SystemManager/SystemManagerBase.h>

void URobosenseM1CommIF::SendData(void* p_data, uint32 size_in_bytes)
{
	HasNewData = true;
	SScanResult* p_in = (SScanResult*)p_data;
	CurrentRequest.Add(p_in);

}
void URobosenseM1CommIF::BeginPlay()
{
	Super::BeginPlay();
	MessageSequenceNumber = 0;
	pLastScanResult = new SScanResult();
	pUdpConnection = GetOwner()->FindComponentByClass<UUdpConnection>();
	SenderThread = FRunnableThread::Create(this, *(GetOwner()->GetName()));
	

}

uint32 URobosenseM1CommIF::Run()
{

	while (true) {
		FPlatformProcess::Sleep(0.1);

		if (CurrentRequest.Num() > 0) {
			SendMainStreamOutputPacket();
		}
	}
	return uint32();
}

inline bool URobosenseM1CommIF::FillMainOutputStreamPacket(SMSOPPacket* p_pack, INT32U &block_ind, INT32U &channel_ind, FLOAT32 azimuth_deg, FLOAT32 elevation_deg, FLOAT32 intensity, FLOAT32 range_meter)
{
	SLidarDataPacket* p_blok = &p_pack->LidarDataPackets[block_ind];
	SLidarChannelData* p_channel = nullptr;

	switch (channel_ind) {
	case 0: p_channel = &p_blok->ChannelData1;
	case 1: p_channel = &p_blok->ChannelData2;
	case 2: p_channel = &p_blok->ChannelData3;
	case 3: p_channel = &p_blok->ChannelData4;
	case 4: p_channel = &p_blok->ChannelData5;
	}

	if (p_channel == nullptr) {
		return false;
	}
	
	p_pack->SetPacketSequenceNumber(MessageSequenceNumber);
	
	INT16U azimuth_raw = (azimuth_deg * OneOverAzimuthScaleFactor);

	p_channel->Ch1Azimuth[0] = azimuth_raw & 0xFF;
	p_channel->Ch1Azimuth[1] = azimuth_raw >> 8;

	INT16U elevation_row = (elevation_deg * OneOverRadiusScaleFactorMeter);
	p_channel->Ch1Elevation[0] = elevation_row & 0xFF;
	p_channel->Ch1Elevation[1] = elevation_row >> 8;


	INT16U radius_raw = (elevation_deg * OneOverRadiusScaleFactorMeter);
	p_channel->Ch1Radius[0] = radius_raw & 0xFF;
	p_channel->Ch1Radius[1] = radius_raw >> 8;

	p_channel->Ch1Intensity[0] = intensity * OneIntensitiyScaleFactor;

	channel_ind++;
	if (channel_ind >= 5) {
		channel_ind = 0;
		block_ind++;
	}

	if (block_ind >= DATA_BLOCK_COUNT) {
		block_ind = 0;
		return true;
	}

	return false;
	
}




void URobosenseM1CommIF::SendMainStreamOutputPacket()
{
	SScanResult* p_current = CurrentRequest[0];
	CurrentRequest.RemoveAt(0);

	int azimuth_count = (p_current->AzimuthRange.Y - p_current->AzimuthRange.X) / SensorAzimuthStepAngle;
	int elevation_count = (p_current->ElevationRange.Y - p_current->ElevationRange.X) / SensorElevationStepAngle;

	SMSOPPacket packet;
	INT32U channel_ind = 0;
	INT32U block_ind = 0;
	bool last_ret;
	memset(&packet, 0, sizeof(SMSOPPacket));
	packet.Init();
	packet.SetTimeStamp(ASystemManagerBase::GetInstance()->GetTimeStamp());

	for (int i = 0; i < azimuth_count; i++) {
		for (int j = 0; j < elevation_count; j++) {
			FLOAT32 azimuth_deg   = p_current->AzimuthRange.X + i * SensorAzimuthStepAngle;
			FLOAT32 elevation_deg = p_current->ElevationRange.X + i * SensorElevationStepAngle;

			FLOAT32 distance_meter = p_current->Interpolate(azimuth_deg, elevation_deg);

			bool ret = FillMainOutputStreamPacket(&packet, block_ind, channel_ind, azimuth_deg, elevation_deg, 1.0f, distance_meter);
			if (ret) {
				SendPacket(&packet);
				MessageSequenceNumber++;

			}

			last_ret = ret;

		}
	}

	if (!last_ret) {
		SendPacket(&packet);
	}

}

inline bool URobosenseM1CommIF::SendPacket(SMSOPPacket* p_pack)
{
	if (pUdpConnection != nullptr) {
		bool ret = pUdpConnection->SendUDPData((INT8U*)p_pack, sizeof(SMSOPPacket));
		return ret;
	}
	else {
		return false;
	}
	
	
}