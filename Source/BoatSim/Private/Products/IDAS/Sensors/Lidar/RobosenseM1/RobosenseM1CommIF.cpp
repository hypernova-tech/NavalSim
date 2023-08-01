// Fill out your copyright notice in the Description page of Project Settings.


#include "Products/IDAS/Sensors/Lidar/RobosenseM1/RobosenseM1CommIF.h"
#include "Products/IDAS/Sensors/Lidar/RobosenseM1/RobosenseM1Types.h"

void URobosenseM1CommIF::SendData(void* p_data, uint32 size_in_bytes)
{
	HasNewData = true;
	SScanResult* p_in = (SScanResult*)p_data;
	CurrentRequest.Add(p_in);

}
void URobosenseM1CommIF::BeginPlay()
{
	Super::BeginPlay();
	pLastScanResult = new SScanResult();

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


void URobosenseM1CommIF::SendMainStreamOutputPacket()
{
	SScanResult* p_current = CurrentRequest[0];
	CurrentRequest.RemoveAt(0);

	int azimuth_count = (p_current->AzimuthRange.Y - p_current->AzimuthRange.X) / SensorAzimuthStepAngle;
	int elevation_count = (p_current->ElevationRange.Y - p_current->ElevationRange.X) / SensorElevationStepAngle;

	SMSOPPacket packet;
	INT32U message_index;
	for (int i = 0; i < azimuth_count; i++) {
		for (int j = 0; j < elevation_count; j++) {
			FLOAT32 azimuth_deg   = p_current->AzimuthRange.X + i * SensorAzimuthStepAngle;
			FLOAT32 elevation_deg = p_current->ElevationRange.X + i * SensorElevationStepAngle;

			FLOAT32 distance = p_current->Interpolate(azimuth_deg, elevation_deg);


			

		}
	}


}