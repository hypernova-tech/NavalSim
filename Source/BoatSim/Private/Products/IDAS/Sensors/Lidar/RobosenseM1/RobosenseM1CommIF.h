// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Lib/Sensor/GenericCommIF/GenericCommIF.h"
#include <Lib/Utils/CScanResult.h>
#include <Lib/UDP/UdpConnection.h>
#include <Products/IDAS/Sensors/Lidar/RobosenseM1/RobosenseM1Types.h>
#include "RobosenseM1CommIF.generated.h"



/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class URobosenseM1CommIF : public UGenericCommIF, public FRunnable
{
	GENERATED_BODY()


protected:
	virtual void BeginPlay() override;
	virtual void SendMainStreamOutputPacket();
	UUdpConnection* pUdpConnection;

public:

	virtual void SendData(void* p_data, uint32 size_in_bytes) override;
	

	// Inherited via FRunnable
	virtual uint32 Run() override;


	SScanResult* pLastScanResult;

	const FLOAT32 SensorAzimuthStepAngle = 0.01f;
	const FLOAT32 SensorElevationStepAngle = 0.01f;


	const FLOAT32 RadiusScaleFactorMete = 5e-3f;
	const FLOAT32 ElevationScaleFactor = 0.01f;
	const FLOAT32 AzimuthScaleFactor = 0.01f;
	const INT32U  IntensitiyScaleFactor = 255;

	const FLOAT32 OneOverRadiusScaleFactorMeter = 200;
	const FLOAT32 OneOverElevationScaleFactor = 100;
	const FLOAT32 OneOverAzimuthScaleFactor = 100;
	const FLOAT32  OneIntensitiyScaleFactor = 0.003921568627451f;


private:

	bool HasNewData;
	TArray< SScanResult*> CurrentRequest;


	inline bool FillMainOutputStreamPacket(SMSOPPacket* p_pack, INT32U& block_ind, INT32U& channel_ind, FLOAT32 azimuth_deg, FLOAT32 elevation_deg, FLOAT32 intensity, FLOAT32 range_meter);
	inline bool SendPacket(SMSOPPacket* p_pack);

	INT16U MessageSequenceNumber;

	FRunnableThread* SenderThread;

};
