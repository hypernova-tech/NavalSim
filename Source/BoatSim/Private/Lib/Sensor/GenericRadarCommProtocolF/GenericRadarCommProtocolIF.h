// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Lib/Sensor/GenericCommIF/GenericCommIF.h"
#include <Lib/Types/Primitives.h>
#include <Networking.h>
#include <Lib/Utils/CScanResult.h>
#include <Lib/UDP/UdpConnection.h>
#include "Lib/Tracker/TrackerBase.h"
#include "IRadarHostIF.h"
#include "GenericRadarCommProtocolIF.generated.h"

#define POINT_CLOUD_DIST_SF 0.001
#define ONE_OVER_POINT_CLOUD_DIST_SF 1000
#pragma pack(push, 1)
enum EPointCloudId:INT8U
{
	PointCloudInfo = 0,
	PointCloudData,
};
#pragma pack(pop)

#define MAX_3D_POINT_CLOUD_MESSAGE_RAW_DATA_SIZE (1550-10)
#define POINT_CLOUND_ENTRY_COUNT 75

#pragma pack(push, 1)
struct SPointCloudInfoPayload
{

	INT32U CaptureInd;
	INT32U PointCount;
	INT8U IsPoint3D;

	FLOAT64 ScanCenterX;
	FLOAT64 ScanCenterY;
	double ScanCenterZ;



};
#pragma pack(pop)

#pragma pack(push, 1)
struct S3DPointCloudDataPayloadEntry
{
	int CaptureInd;
	int DataIndex;
	INT32S X;
	INT32S Y;
	INT32S Z;

	inline void SetPoint(FLOAT64 x_in, FLOAT64 y_in, FLOAT64 z_in)
	{
		X = x_in * ONE_OVER_POINT_CLOUD_DIST_SF;
		Y = y_in * ONE_OVER_POINT_CLOUD_DIST_SF;
		Z = z_in * ONE_OVER_POINT_CLOUD_DIST_SF;
	}

	inline void GetPoint(FVector &out)
	{
		out.X = X * POINT_CLOUD_DIST_SF;
		out.Y = Y * POINT_CLOUD_DIST_SF;
		out.Z = Z * POINT_CLOUD_DIST_SF;
	}
};
#pragma pack(pop)

#pragma pack(push, 1)
struct S3DPointCloudDataPayload
{
	S3DPointCloudDataPayloadEntry Entries[POINT_CLOUND_ENTRY_COUNT];
};
#pragma pack(pop)


#pragma pack(push, 1)
struct S3DPointCloudMessage
{
public:
	INT8U MessageID;
	INT32S DataLength;
	 
	INT8U Data[MAX_3D_POINT_CLOUD_MESSAGE_RAW_DATA_SIZE];

	void Reset()
	{
		DataLength = 0;
		//memset(this, 0, sizeof(S3DPointCloudMessage));
	}

	void SetMessageId(INT8U id)
	{
		MessageID = id;
	}

	INT8U* GetPayload()
	{
		return Data;
	}


	void SetDataLength(int len)
	{
		DataLength = len;
	}

	void SetMessage(INT32S id, INT8U* payload, INT32S payload_size)
	{
		MessageID = id;
		DataLength = payload_size;
		memcpy(Data, payload, payload_size);
	}

	INT32S GetMessageSize()
	{
		return sizeof(MessageID) + sizeof(DataLength) + DataLength;
	}

	INT32S GetDataPointCount()
	{
		return DataLength / sizeof(S3DPointCloudDataPayloadEntry);
	}


	virtual void Visualize(TArray<FVector>& cloud);

};
#pragma pack(pop)




/**
 * 
 */

struct STargetTrackStatusData {
	TArray< STrackedObjectInfo*>* Tracks;
	FLOAT64 ClosestPointOfApproachMeters;
	FLOAT64 TimeToClosestPointOfApproachSec;
	bool TowardsCPA; //(0 target moving away from CPA, 1 towards CPA)

};

struct STextureData
{
	TArray<FColor> Colors;
	int32 Width, Height;

public:
	STextureData() {
		Width = 0;
		Height = 0;
	}

	inline INT8U GetColor(float w, float h) {
		int ind_w = Width * w;
		int ind_h = Height * h;
		auto val = Colors[ind_h * Width + ind_w];
		return val.R;
	}
};

/**
 *
 */
#define SpokeImageSize  2048
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UGenericRadarCommProtocolIF : public UGenericCommIF, public FRunnable
{
	GENERATED_BODY()


protected:
	virtual void BeginPlay() override;
	virtual void SendRadarTrack();

public:
	~UGenericRadarCommProtocolIF();
	virtual void SendData(void* p_data, uint32 size_in_bytes) override;


	// Inherited via FRunnable
	virtual uint32 Run() override;


	virtual void SetHostIF(IHostIF* p_val) override;
	
	void SendTrackedObjects(const STargetTrackStatusData& info, char* p_serial);


protected:

	bool HasNewData;
	FRunnableThread* SenderThread;
	bool IsStoped = false;
	int PointCloudCaptureInd = 0;
	

	virtual void Stop() override;

	TArray< SScanResult*> CurrentRequest;

	IRadarHostIF* pRadarHostIF;
	FCriticalSection Mutex;

	UPROPERTY(BlueprintReadWrite)
		UUdpConnection* pUDPConnection;

	virtual void OnReceivedConnectionData(void* connection, INT8U* p_data, INT32U count) override;

		STextureData NoiseTexture;


	int SpokeSequanceNumber = 0;
	int PrevSequenceNumber = -1;

	float RadarTimeVaryingGain = 0;
	FVector2D RadarNoiseTextureSpeed;

	INT8U SpokeImage[SpokeImageSize][SpokeImageSize];
	FVector SpokeImage3D[SpokeImageSize][SpokeImageSize];

	INT8U** CreateImage(int width, int height);
	void SpokeToImage(FLOAT64 spoke_azimuth_deg, INT8U* p_spoke, INT32U num_samples);
	void SpokePointCloudToImage(FLOAT64 spoke_azimuth_deg,FVector scan_center, FLOAT64 scan_range_meter, FLOAT64 cell_size_meter, TArray<FVector>& point_cloud, INT32U num_samples);
	bool LoadPNGImageAsFColorArray(const FString& ImagePath, TArray<FColor>& OutColors, int32& OutWidth, int32& OutHeight);
	virtual void ApplyGain(double gain_level);
	virtual void ApplyMerge(int size);

	void RenderPointCloud(const TArray<FVector> &pts);
};
