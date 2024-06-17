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
#include <Lib/Math/CMath.h>
#include "Containers/List.h"
#include "GenericRadarCommProtocolIF.generated.h"



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
		h = h < 0 ? 0 : (h > 1 ? 1 : h);
		w = w < 0 ? 0 : (w > 1 ? 1 : w);
		int ind_w = Width * w;
		int ind_h = Height * h;
		auto val = Colors[ind_h * Width + ind_w];
		return val.R;
	}

};



class CClutterEntry
{
public:
	INT32S X;
	INT32S Y;
	FLOAT64 StartTimeSec;
	FLOAT64 EndTimeSec;

};

class CClutterContainer
{
	TDoubleLinkedList<CClutterEntry> Entries;
	INT32S MaxCnt;
	INT32S MinCnt;
	INT32S SpokeSize;
	SNeighbour Neighbours[128];
	void Add(INT32S cnt);
public:

	void Init(INT32S spoke_size);
	void Update(INT32S min_cnt, INT32S max_cnt, INT32S point_density, void *p_spoke_image);
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
	
	int TotalNeighbours;


public:
	~UGenericRadarCommProtocolIF();
	virtual void SendData(void* p_data, uint32 size_in_bytes) override;


	// Inherited via FRunnable
	virtual uint32 Run() override;


	virtual void SetHostIF(IHostIF* p_val) override;
	
	void SendTrackedObjects(const STargetTrackStatusData& info, char* p_serial);
	virtual void InitCommIF() override;

protected:

	bool HasNewData;
	FRunnableThread* SenderThread;
	bool IsStoped = false;
	int PointCloudCaptureInd = 0;
	CClutterContainer RainClutterContainer;
	CClutterContainer SeaClutterContainer;
	FVector2D RandomWalk;
	virtual void Stop() override;

	TArray< SScanResult*> CurrentRequest;

	IRadarHostIF* pRadarHostIF;
	FCriticalSection Mutex;

	UPROPERTY(BlueprintReadWrite)
		UUdpConnection* pUDPConnection;



	virtual void OnReceivedConnectionData(void* connection, INT8U* p_data, INT32U count) override;

	STextureData NoiseTexture;
	STextureData TerrainTexture;

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
	void FillArrond(int x, int y, int cnt);

};
