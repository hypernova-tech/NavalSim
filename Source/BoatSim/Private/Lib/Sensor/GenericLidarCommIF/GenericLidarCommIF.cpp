// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/Sensor/GenericLidarCommIF/GenericLidarCommIF.h"
#include <Lib/SystemManager/SystemManagerBase.h>
#include <Lib/PointVisualizer/PointCloudTypes.h>
#include <Lib/Math/CMath.h>

UGenericLidarCommIF::~UGenericLidarCommIF()
{
	if (SenderThread != nullptr) {
		SenderThread->Kill();
		delete SenderThread;
	}
}

void UGenericLidarCommIF::SendData(void* p_data, uint32 size_in_bytes)
{
	HasNewData = true;
	SScanResult* p_in = (SScanResult*)p_data;
	CurrentRequest.Add(p_in);

}


void UGenericLidarCommIF::BeginPlay()
{
	Super::BeginPlay();

	pLastScanResult = new SScanResult();
	SenderThread = FRunnableThread::Create(this, *(GetOwner()->GetName()));

	RegisterConnectionInfo(0, pUDPConnection->GetConnectionInfo(), pUDPConnection);

}
void UGenericLidarCommIF::Stop()
{
	IsStoped = true;
}
uint32 UGenericLidarCommIF::Run()
{

	while (!IsStoped) {
		FPlatformProcess::Sleep(0.1);

		if (CurrentRequest.Num() > 0) {
			SendMainStreamOutputPacket();
		}
	}
	return uint32();
}


void UGenericLidarCommIF::SendMainStreamOutputPacket()
{
	SScanResult* p_current = CurrentRequest[0];
	CurrentRequest.RemoveAt(0);

	ASensorBase* p_owning_actor = (ASensorBase*)pHostIF->GetOwningActor();

	int azimuth_count = (p_current->AzimuthRange.Y - p_current->AzimuthRange.X) / p_owning_actor->HorizontalScanStepAngleDeg;
	int elevation_count = (p_current->ElevationRange.Y - p_current->ElevationRange.X) / p_owning_actor->VerticalScanStepAngleDeg;

#if true
	S3DPointCloudMessage cloud_data_mes;
	cloud_data_mes.Reset();

	int cloud_data_ind = 0;
	int entry_index_in_payload = 0;



	bool is_visualize = p_owning_actor->GetPoint3DVisualize();
	TArray<FVector> point_cloud;
	S3DPointCloudDataPayload* p_point_cloud_data = (S3DPointCloudDataPayload*)cloud_data_mes.GetPayload();

	for (int i = 0; i < azimuth_count; i++) {
		for (int j = 0; j < elevation_count; j++) {
			FLOAT32 azimuth_deg = p_current->AzimuthRange.X + i * p_owning_actor->HorizontalScanStepAngleDeg;
			FLOAT32 elevation_deg = p_current->ElevationRange.X + j * p_owning_actor->VerticalScanStepAngleDeg;
			FLOAT32 intensity = 0;
			//FLOAT32 distance_meter = p_current->Interpolate(azimuth_deg, elevation_deg, intensity);
			FLOAT32 distance_meter;

			FVector pt =  p_current->GetPointAndReset(i, j, distance_meter, intensity);

			

			intensity = FMath::Lerp(0.0f, 1.0f, ((intensity + 1) * 0.5f) * 0.75f + 0.25f * distance_meter * 0.001f);
			FVector actor_loc = p_owning_actor->GetActorLocation();
			FVector rel_pos;
			if (CMath::IsZero(distance_meter, 1e-6)) {
				rel_pos = FVector::ZeroVector;				
			}
			else {
				rel_pos = TOW(pt - actor_loc);
			}
			


			auto p_entry = &p_point_cloud_data->Entries[entry_index_in_payload];
			p_entry->CaptureInd = PointCloudCaptureInd;
			p_entry->DataIndex = cloud_data_ind;
			p_entry->SetPoint(rel_pos.X, -rel_pos.Y, rel_pos.Z, intensity);


			entry_index_in_payload++;
			cloud_data_ind++;

			if (entry_index_in_payload == POINT_CLOUND_ENTRY_COUNT) {
				cloud_data_mes.SetMessageId(EPointCloudId::PointCloudData);
				cloud_data_mes.SetDataLength(entry_index_in_payload * sizeof(S3DPointCloudDataPayloadEntry));
				bool ret = pUDPConnection->SendData((const INT8U*)&cloud_data_mes, cloud_data_mes.GetMessageSize());
				if (is_visualize) {
					cloud_data_mes.Visualize(point_cloud);
				}
				
				entry_index_in_payload = 0;
			}	

		}
	}

	if (entry_index_in_payload != 0) {
		cloud_data_mes.SetMessageId(EPointCloudId::PointCloudData);
		cloud_data_mes.SetDataLength(entry_index_in_payload * sizeof(S3DPointCloudDataPayloadEntry));
		bool ret = pUDPConnection->SendData((const INT8U*)&cloud_data_mes, cloud_data_mes.GetMessageSize());
		if (is_visualize) {
			cloud_data_mes.Visualize(point_cloud);
		}
		entry_index_in_payload = 0;
	}

	if (is_visualize) {
#if true
		AsyncTask(ENamedThreads::GameThread, [this, point_cloud]()
			{
				RenderPointCloud(point_cloud);
			});
#endif
	}


#endif

}
void UGenericLidarCommIF::RenderPointCloud(const TArray<FVector>& pts)
{
	ASensorBase* p_actor = (ASensorBase*)pHostIF->GetOwningActor();
	p_actor->RenderPointCloud(p_actor->GetActorLocation(), pts);
}


