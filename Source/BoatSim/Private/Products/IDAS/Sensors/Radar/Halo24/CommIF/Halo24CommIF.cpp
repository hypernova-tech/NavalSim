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
	SScanResult* p_new = p_in;
	
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
	FLOAT32 each_spoke_step = 1.0f / 4096;
	FLOAT32 each_cell_size = p_current->ScanRangeMeter / 1024.0f;
	CurrentRequest.RemoveAt(0);

	
	auto* p_current_sector = p_current->GetSectorContainer()->GetSector(p_current->CurrentSector);

	int SpokeSequanceNumber = 0;
	int SpokeCountPerSector = 4096 * FMath::Abs(p_current_sector->EndAzimuthDeg - p_current_sector->StartAzimuthDeg) / 360.0;


	

	for (INT32S i = 0; i < SpokeCountPerSector; i++) {

		SRadarSimSDKPacket pack;
		memset(&pack, 0, sizeof(SRadarSimSDKPacket));

		SHalo24SpokePayload* p_spoke_payload = (SHalo24SpokePayload*)pack.Payload;

		S9174SpokeHeader* p_hdr = &(p_spoke_payload->SpokeData.Header);
		p_hdr->spokeLength_bytes = 536;
		p_hdr->sequenceNumber = SpokeSequanceNumber;
		p_hdr->sampleEncoding = 0;
		p_hdr->nOfSamples = 1024;
		p_hdr->bitsPerSample = 4;
		p_hdr->rangeCellSize_mm = (each_cell_size)*1000;
		p_hdr->spokeAzimuth = p_current->AzimuthRange.X;
		p_hdr->bearingZeroError = 0;
		p_hdr->spokeCompass = p_current->ScanOwnshipHeadingTrueNorth / 4096.0f;
		p_hdr->trueNorth = 1;
		p_hdr->rangeCellsDiv2 = p_current->ScanRangeMeter / (each_cell_size) * 0.5f;

		p_current_sector->MapSpoke4Bits(p_current->ScanCenter, p_current->AzimuthRange.X + i * each_spoke_step, each_cell_size, p_spoke_payload->SpokeData.Data);

		pack.SetID(ESimSDKDataIDS::SpokeData);
		pack.SetPayloadSize(sizeof(SHalo24SpokePayload));
		pUDPConnection->SendUDPData((const INT8U*)&pack, pack.GetTransmitSize());

		SpokeSequanceNumber++;
		SpokeSequanceNumber %= 4096;

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
void UHalo24CommIF::SendRadarState(ERadarState radar_state, char* p_serial)
{
	SRadarState state;

	memset(&state, 0, sizeof(state));
	state.SerialData.SetSerial(p_serial, strlen(p_serial));
	SRadarSimSDKPacket pack;
	pack.SetPayload(ESimSDKDataIDS::RadarState, (INT8U*)&state, sizeof(SRadarState));
	pUDPConnection->SendUDPData((const INT8U*)&pack, pack.GetTransmitSize());

}
void UHalo24CommIF::SendRadarSetup(const SRadarSetupData& setup, char *p_serial)
{

	SRadarSimSDKPacket pack;
	SRadarSetupPayload payload;
	memcpy(&payload.RadarSetupData, &setup, sizeof(SRadarSetupData));
	payload.SerialData.SetSerial(p_serial, strlen(p_serial));
	pack.SetPayload(ESimSDKDataIDS::RadarSetup, (INT8U*)&payload, sizeof(SRadarSetupPayload));
	pUDPConnection->SendUDPData((const INT8U*)&pack, pack.GetTransmitSize());
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
