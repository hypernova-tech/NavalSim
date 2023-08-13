// Fill out your copyright notice in the Description page of Project Settings.


#include "Products/IDAS/Sensors/Radar/Halo24/CommIF/Halo24CommIF.h"
#include "HAL/PlatformNamedPipe.h"


UHalo24CommIF::~UHalo24CommIF()
{
	if (SenderThread != nullptr) {
		SenderThread->Kill();
		delete SenderThread;
	}


	
}

void UHalo24CommIF::SendData(void* p_data, uint32 size_in_bytes)
{
	HasNewData = true;
	SScanResult* p_in = (SScanResult*)p_data;
	SScanResult* p_new = new SScanResult();
	p_new->CopyTrackPoint3DOnly(p_in);
	CurrentRequest.Add(p_new);

}

uint32 UHalo24CommIF::Run()
{
	while (!IsStoped) {
		FPlatformProcess::Sleep(0.1);

		if (CurrentRequest.Num() > 0) {
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

	pUDPConnection->SendUDPData((INT8U*)pack, pack->GetTransmitSize());
	
	RestorePacket(pack);
}

void UHalo24CommIF::BeginPlay()
{
	Super::BeginPlay();
	pUDPConnection->AddConnectionDataReceiver(this);
	for (int i = 0; i < 16; i++) {
		Packets.Add(new SRadarSimSDKPacket());
	}
	


	SenderThread = FRunnableThread::Create(this, *(GetOwner()->GetName()));
	
}

void UHalo24CommIF::SendRadarTrack()
{
	SScanResult* p_current = CurrentRequest[0];
	CurrentRequest.RemoveAt(0);
	delete p_current;
	return;

	if (pPipeInstance->IsReadyForRW()) {

		

		INT8U data[] = { 0,1,2,3,4,5,6 };
		INT32U fvector_size = sizeof(FVector);
		INT32U total_size = fvector_size * p_current->Track3DCount;
		INT32U struct_size = sizeof(p_current->Track3DWorld);
		FLOAT64 start_time_ref = CUtil::GetTimeSeconds();
		bool ret = pPipeInstance->WriteBytes(total_size, (INT8U*)p_current->Track3DWorld);
		FLOAT64 diff = CUtil::GetTimeSeconds() - start_time_ref;
		CUtil::DebugLog(FText::AsNumber(total_size).ToString());
		delete p_current;
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

	pUDPConnection->SendUDPData((const INT8U*) & pack, pack.GetTransmitSize());
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
	pHostIF->OnRecievedMessage(p_pack);


}
