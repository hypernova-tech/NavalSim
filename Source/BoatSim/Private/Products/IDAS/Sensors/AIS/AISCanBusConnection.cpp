// Fill out your copyright notice in the Description page of Project Settings.


#include "Products/IDAS/Sensors/AIS/AISCanBusConnection.h"
#include "Lib/Sensor/AISPublisher/AISPublisher.h"
#include <Lib/Time/CTime.h>
bool UAISCanBusConnection::SendAISNMEA200Message(const int src_addr, const int prio, const int png, const INT8U* p_bytes, INT32U count)
{
	SCanControllerPacket pack;
	if (count <= 8) {
		
		pack.SetData(src_addr, prio, png, (INT8U*)p_bytes, count);
		bool ret = Super::SendData((const INT8U*)&pack, sizeof(pack));
		return ret;
	}
	else { // fast packet
		INT8U frame[8];
		memset(frame, 0xFF, sizeof(frame));

		if (CurrentMessageSequence == 0) {
			frame[0] = 0xA0;
			frame[1] = count;
			INT32U current_ind = count - CurrentRemaingDataCount;

			memcpy(&frame[2], &p_bytes[current_ind], 6);
#if 0
			for (int i = 0; i < 6; i++) {
				frame[2 + i] = i;
			}
#endif
			CurrentMessageSequence++;
			CurrentRemaingDataCount -= 6;

		}
		else {
			frame[0] = 0xA0 + CurrentMessageSequence;
			INT32U current_ind = count - CurrentRemaingDataCount;
			INT32U cnt;
			cnt = CurrentRemaingDataCount > 7 ? 7 : CurrentRemaingDataCount;

			memcpy(&frame[1], &p_bytes[current_ind], cnt);
			
#if 0
			for (int i = 0; i < 7; i++) {
				frame[1 + i] = 6 + (CurrentMessageSequence-1) *7 + i;
			}
#endif
			
			
			CurrentMessageSequence++;
			CurrentRemaingDataCount -= cnt;
		}

		pack.SetData(src_addr, prio, png, frame, 8);
		bool ret = Super::SendData((const INT8U*)&pack, sizeof(pack));
		return ret;
	}
	
}
bool UAISCanBusConnection::SendData(const INT8U* p_hdr, INT32U header_size, const INT8U* p_bytes, INT32U count)
{
	SAISHdr* p_ais_hdr = (SAISHdr*)p_hdr;

	SCanMessage mes;
	mes.SrcAddr = p_ais_hdr->SrcAddr;
	mes.Prio = p_ais_hdr->Prio;
	mes.Png = p_ais_hdr->Png;
	mes.Length = count;
	memcpy(mes.Data, p_bytes, count);
	MessageQ.Add(mes);

	return true;
}

void UAISCanBusConnection::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	FastPacketStateMachine();
}

void UAISCanBusConnection::FastPacketStateMachine()
{
	auto curr_state = CanFastPackState;
	auto next_state = curr_state;

	switch (curr_state)
	{
	case CanFastPackStateIdle:
		if (MessageQ.Num() > 0) {
			next_state = CanFastPackStateRunning;
			CurrentMessage = MessageQ[0];
			MessageQ.RemoveAt(0);
			CurrentMessageSequence = 0;
			CurrentRemaingDataCount = CurrentMessage.Length;
		}
		break;
	case CanFastPackStateRunning:
		SendAISNMEA200Message(CurrentMessage.SrcAddr, CurrentMessage.Prio, CurrentMessage.Png, CurrentMessage.Data, CurrentMessage.Length);
		LastMessageSendTimeSec = CTime::GetTimeSecond();
		if (CurrentRemaingDataCount == 0) {
			next_state = CanFastPackStateIdle;
		}
		else {
			next_state = CanFastPackStateWait;
		}
		break;

	case CanFastPackStateWait:
		if (CTime::GetTimeSecond() - LastMessageSendTimeSec > 50e-3) {
			next_state = CanFastPackStateRunning;
		}
		break;
	default:
		break;
	}

	CanFastPackState = next_state;
}
