// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */

#include "Lib/Types/Primitives.h"


struct SConnectionInfo {
	FString IpAddr;
	INT32S LocalPort;
	INT32S RemotePort;

};

struct SConnectionEntry
{
	SConnectionInfo ConnectionInfo;
	UActorComponent* pOwner;
};

class ConnectionDefs
{
public:
	ConnectionDefs();
	~ConnectionDefs();
};
