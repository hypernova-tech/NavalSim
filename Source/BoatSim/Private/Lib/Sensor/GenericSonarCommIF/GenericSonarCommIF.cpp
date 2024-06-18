// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/Sensor/GenericSonarCommIF/GenericSonarCommIF.h"
#include <Lib/SystemManager/SystemManagerBase.h>
#include <Lib/PointVisualizer/PointCloudTypes.h>
#include <Lib/Math/CMath.h>


UGenericSonarCommIF::~UGenericSonarCommIF()
{
	if (SenderThread != nullptr) {
		SenderThread->Kill();
		delete SenderThread;
	}
}

void UGenericSonarCommIF::SendData(void* p_data, uint32 size_in_bytes)
{
	HasNewData = true;
	SScanResult* p_in = (SScanResult*)p_data;
	CurrentRequest.Add(p_in);

}


void UGenericSonarCommIF::BeginPlay()
{
	Super::BeginPlay();

	pLastScanResult = new SScanResult();
	SenderThread = FRunnableThread::Create(this, *(GetOwner()->GetName()));

	RegisterConnectionInfo(0, pUDPConnection->GetConnectionInfo(), pUDPConnection);

}

void UGenericSonarCommIF::SetHostIF(IHostIF* p_val)
{
	Super::SetHostIF(p_val);
	pSonarHostIF = (ISonarHostIF*)p_val;
}
void UGenericSonarCommIF::Stop()
{
	IsStoped = true;
}
uint32 UGenericSonarCommIF::Run()
{

	while (!IsStoped) {
		FPlatformProcess::Sleep(0.1);

		if (CurrentRequest.Num() > 0) {
			SendMainStreamOutputPacket();
		}
	}
	return uint32();
}


void UGenericSonarCommIF::SendMainStreamOutputPacket()
{
	SScanResult* pScanResult = CurrentRequest[0];
	CurrentRequest.RemoveAt(0);

	ASensorBase* p_owning_actor = (ASensorBase*)pHostIF->GetOwningActor();

	

	S3DPointCloudMessage cloud_data_mes;
	cloud_data_mes.Reset();

	int cloud_data_ind = 0;
	int entry_index_in_payload = 0;



	bool is_visualize = p_owning_actor->GetPoint3DVisualize();
	TArray<FVector> point_cloud;
	S3DPointCloudDataPayload* p_point_cloud_data = (S3DPointCloudDataPayload*)cloud_data_mes.GetPayload();



	auto sector_info = pScanResult->SectorContainer.GetSector(pScanResult->CurrentSector);

	auto forward = p_owning_actor->GetActorForwardVector();
	forward.Normalize();

	auto loc = p_owning_actor->GetPositionXYZMeters();



	auto llh = p_owning_actor->GetPositionLatLongHeightMSL();

	int cnt = sector_info->SectorData.Num();
	int ind = 0;
	bool is_ground;

	double filtered_squelch = FMath::Clamp(pSonarHostIF->GetSquelchSensitivity(), 110, 210);
	double detection_prop_percent = 100;

	if (pSonarHostIF->GetAutoSquelchEnabled()) {
		detection_prop_percent = CMath::GetRandomRange(95, 100);
	}
	else {
		detection_prop_percent = CMath::Remap(filtered_squelch, 110, 210, 99, 10);
	}
	SPointCloudInfoPayload info;
	info.TimeStamp = CTime::GetTimeSecond();
	info.PointCount = -1;
	info.CaptureInd = CaptureInd++;
	auto scan_center_w = TOW(pScanResult->ScanCenter);
	info.ScanCenterX = scan_center_w.X;
	info.ScanCenterY = scan_center_w.Y;
	info.ScanCenterZ = scan_center_w.Z;
	info.IsPoint3D = 1;
	S3DPointCloudMessage message;
	message.SetMessage(EPointCloudId::PointCloudInfo, (INT8U*)&info, sizeof(info));
	pUDPConnection->SendData((const INT8U*)&message, message.GetMessageSize());


	for (int i = 0; i < cnt; i++) {

		FVector& data = (sector_info->SectorData[i]);
		EScanObjectType& object_type = sector_info->ObjectType[i];
		FLOAT32& normal_strength = sector_info->NormalStrength[i];
		is_ground = (object_type == EScanObjectType::ScanObjectTypeTerrain);


		INT16U intensity = FMath::Lerp(110.0, 210.0, FMath::Abs(normal_strength));
		bool has_pt = false;
		if (is_ground) {
			if (pSonarHostIF->GetBottomDetectEnabled()) {
				if (detection_prop_percent >= CMath::GetRandomRange(0, 100)) {

					has_pt = true;
					ind++;
				}

			}
		}
		else {
			if (detection_prop_percent >= CMath::GetRandomRange(0, 100)) {
				ind++;
				has_pt = true;
			}
		}

		if (has_pt) {
			auto  actor_pos = pScanResult->ScanCenter;
			auto p_entry = &p_point_cloud_data->Entries[entry_index_in_payload];
			p_entry->CaptureInd = PointCloudCaptureInd;
			p_entry->DataIndex = cloud_data_ind;
			auto rel_pos = TOW(data - actor_pos);
	
			p_entry->SetPoint(rel_pos.X, -rel_pos.Y, rel_pos.Z, intensity, is_ground);
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

}
void UGenericSonarCommIF::RenderPointCloud(const TArray<FVector>& pts)
{
	AActor* p_actor = (AActor*)pHostIF->GetOwningActor();

	auto loc = p_actor->GetActorLocation();
	int cnt = 0;
	for (auto pt : pts) {
		if (!pt.IsZero()) {
			cnt++;
			if (cnt < 2000) {
				pt.Y *= -1;
				CUtil::DebugBox(GetWorld(), loc + TOUE(pt), 10, FColor::Red, 0.2);
			}
		}


	}
}


