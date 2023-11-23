// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/Connection/UDPBaseCommIF.h"

void UUDPBaseCommIF::BeginPlay()
{
	Super::BeginPlay();
	RegisterConnectionInfo(0, pUDPConnection->GetConnectionInfo(), pUDPConnection);
}
