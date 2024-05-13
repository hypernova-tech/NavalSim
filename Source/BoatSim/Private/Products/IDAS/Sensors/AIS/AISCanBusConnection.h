// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Lib/Connection/Serial/SerialCommIF.h"
#include "AISCanBusConnection.generated.h"

#pragma pack(push, 1)
union UFrameInfo {
    INT32U FrameId;
    struct  {
        INT32U SrcAddr : 8;
        INT32U Png : 17;
        INT32U Reserved1 : 1;
        INT32U Priority : 3;
        INT32U Reserved2 : 3;
    }FrameBits;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct SCanControllerPacket
{
    INT8U Header1;
    INT8U Header2;
    INT8U Type;
    INT8U FrameType;
    INT8U FrameFormat;
    UFrameInfo FrameInfo;

    


    INT8U Length;
    INT8U Data[8];
    INT8U Reserved;
    INT8U Checksum;

public:
    SCanControllerPacket() {
        
    }

 
    void PrepareFrame(INT32U frame_id, INT8U* p_data, INT8U len) {
       
        Header1 = 0xaa;
        Header2 = 0x55;
        Type = 0x1;
        FrameType = 0x2;
        
        FrameFormat = 0x1;
        FrameInfo.FrameId = frame_id;
        Length = len;
        memcpy(Data, p_data, len);
        Reserved = 0;

        INT8U* p_frame_buff = (INT8U*)&FrameInfo.FrameId;

        Checksum = Type + FrameType + FrameFormat + p_frame_buff[0] + p_frame_buff[1] + p_frame_buff[2] + p_frame_buff[3] + Length + Reserved;

        for (int i = 0; i < 8; i++) {
            Checksum += Data[i];
        }


    }

    void SetData(INT32U src_addr, INT32U prio, INT32U png, INT8U* p_data, INT8U len)
    {
        UFrameInfo frame;
        frame.FrameId = 0;

        frame.FrameBits.SrcAddr = src_addr;
        frame.FrameBits.Priority = prio;
        frame.FrameBits.Png = png;
        PrepareFrame(frame.FrameId, p_data, len);

    }
};
#pragma pack(pop)

enum ECanFastPackStates
{
    CanFastPackStateIdle,
    CanFastPackStateRunning,
    CanFastPackStateWait
};

struct SCanMessage {
    INT32U SrcAddr;
    INT32U Prio;
    INT32U Png;
    INT32U Length;
    INT8U Data[1024];
};


/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UAISCanBusConnection : public USerialCommIF
{
	GENERATED_BODY()

protected:
    TArray< SCanMessage> MessageQ;
    INT32U CurrentMessageSequence;
    
    INT32U CurrentRemaingDataCount;
    SCanMessage CurrentMessage;
    FLOAT64 LastMessageSendTimeSec;

    ECanFastPackStates CanFastPackState = ECanFastPackStates::CanFastPackStateIdle;
    virtual bool SendAISNMEA200Message(const int src_addr, const int prio, const int png, const INT8U* p_bytes, INT32U count);

public:
    virtual bool SendData(const INT8U* p_hdr, INT32U header_size, const INT8U* p_bytes, INT32U count) override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
    void FastPacketStateMachine();
};
