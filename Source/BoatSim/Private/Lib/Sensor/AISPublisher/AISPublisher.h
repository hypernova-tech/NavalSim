// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Lib/ActorBase/ActorBase.h"
#include "AISPublisher.generated.h"
struct SAISHdr {
	INT32U SrcAddr;
	INT32U Prio;
	INT32U Png;
};
/**
 * 
 */
UCLASS()
class AAISPublisher : public AActorBase
{
	GENERATED_BODY()
	
public:
	void SendAISData(INT32U src_addr, INT32U prrio, INT32U png, void* data, INT32S data_len);
};
