// Fill out your copyright notice in the Description page of Project Settings.


#include "Products/IDAS/Sensors/Lidar/RobosenseM1/RobosenseM1CommIF.h"
#include "Products/IDAS/Sensors/Lidar/RobosenseM1/RobosenseM1Types.h"
#include <Lib/SystemManager/SystemManagerBase.h>

URobosenseM1CommIF::~URobosenseM1CommIF()
{
	if (SenderThread != nullptr) {
		SenderThread->Kill();
		delete SenderThread;
	}
}

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
	//pUdpConnection = GetOwner()->FindComponentByClass<UUdpConnection>();
	//pUdpConnection = GetOwner()->CreateCh <UUdpConnection>();
	SenderThread = FRunnableThread::Create(this, *(GetOwner()->GetName()));
	
	GetConfigurationPacket()->Reset()->SetIPAddr("127.0.0.1", pMainOutputStreamConnection->GetRemoteIP(), "00:01:00:02:00:03", pMainOutputStreamConnection->GetRemotePort(), pDIFOP->GetRemotePort());
}
void URobosenseM1CommIF::Stop()
{ 
	IsStoped = true;
}
uint32 URobosenseM1CommIF::Run()
{

	while (!IsStoped) {
		FPlatformProcess::Sleep(0.1);

		if (CurrentRequest.Num() > 0) {
			SendMainStreamOutputPacket();
		}
	}
	return uint32();
}

SDIFOP* URobosenseM1CommIF::GetConfigurationPacket()
{
	return &DIOPPacket;
}

inline bool URobosenseM1CommIF::FillMainOutputStreamPacket(SMSOPPacket* p_pack, INT32U &block_ind, INT32U &channel_ind, FLOAT32 azimuth_deg, FLOAT32 elevation_deg, FLOAT32 intensity, FLOAT32 range_meter)
{
	SLidarDataPacket* p_blok = &p_pack->LidarDataPackets[block_ind];
	SLidarChannelData* p_channel = nullptr;

	switch (channel_ind) {
	case 0: p_channel = &p_blok->ChannelData1; break;
	case 1: p_channel = &p_blok->ChannelData2; break;
	case 2: p_channel = &p_blok->ChannelData3; break;
	case 3: p_channel = &p_blok->ChannelData4; break;
	case 4: p_channel = &p_blok->ChannelData5; break;
	}

	if (p_channel == nullptr) {
		return false;
	}
	
	p_pack->SetPacketSequenceNumber(MessageSequenceNumber);
	
	INT16S azimuth_raw = (azimuth_deg * OneOverAzimuthScaleFactor + 0x8000);

	p_channel->ChAzimuth[0] = azimuth_raw >> 8 ;
	p_channel->ChAzimuth[1] = azimuth_raw & 0xFF;

	INT16S elevation_row = (elevation_deg * OneOverElevationScaleFactor + 0x8000);
	p_channel->ChElevation[0] = elevation_row >> 8;
	p_channel->ChElevation[1] = elevation_row & 0xFF ;


	INT16U radius_raw = (range_meter * OneOverRadiusScaleFactorMeter);
	p_channel->ChRadius[0] = radius_raw >> 8 ;
	p_channel->ChRadius[1] = radius_raw & 0xFF;

	p_channel->ChIntensity[0] = intensity* OneIntensitiyScaleFactor;

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



	MessageSequenceNumber = 0;
#if true
	for (int i = 0; i < azimuth_count; i++) {
		for (int j = 0; j < elevation_count; j++) {
			FLOAT32 azimuth_deg   = p_current->AzimuthRange.X + i * SensorAzimuthStepAngle;
			FLOAT32 elevation_deg = p_current->ElevationRange.X + j * SensorElevationStepAngle;
			FLOAT32 intensity = 0;
			FLOAT32 distance_meter = p_current->Interpolate(azimuth_deg, elevation_deg, intensity);


			//azimuth_deg = -135;
			//elevation_deg = 0;

			//azimuth_deg += FMath::Abs(p_current->AzimuthRange.X);

			if (distance_meter == 0) {
				//continue;
			}
			else {
				distance_meter = distance_meter;
			}

			//distance_meter = 5;

			//FLOAT32 mapped_azimuth_deg =   min_azimuth_deg + i * SensorAzimuthStepAngle;
			//FLOAT32 mapped_elevation_deg =  min_elevation_deg + j * SensorElevationStepAngle;
			intensity = FMath::Lerp(0.0f, 1.0f, ((intensity + 1) * 0.5f)*0.75f + 0.25f*distance_meter*0.001f);
			bool ret = FillMainOutputStreamPacket(&packet, block_ind, channel_ind, azimuth_deg, -elevation_deg, intensity, distance_meter);
			if (ret) {
				SendPacket(&packet);
				MessageSequenceNumber++;
				packet.ZeroizePayload();

			}

			last_ret = ret;

		}
	}

	if (!last_ret) {
		SendPacket(&packet);
	}

#endif

}

inline bool URobosenseM1CommIF::SendPacket(SMSOPPacket* p_pack)
{
	if (pMainOutputStreamConnection != nullptr) {
		bool ret = pMainOutputStreamConnection->SendUDPData((INT8U*)p_pack, sizeof(SMSOPPacket));
		return ret;
	}
	else {
		return false;
	}
	
	
}


 void URobosenseM1CommIF::SendConfig()
{


	Super::SendConfig();

	return;

	if (pDIFOP != nullptr) {
		bool ret = pDIFOP->SendUDPData((INT8U*)&DIOPPacket, sizeof(DIOPPacket));
		
	}
	else {
		
	}

}