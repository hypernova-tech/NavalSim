// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Lib/EthernetDataProcessor/EthernetDataProcessorBase.h"
#include <Lib/SOACommonObserverArgs/PlatformKinematicData.h>
#include <Lib/Types/Primitives.h>
using namespace std;
#include <vector>
#include <list>
#include "IDASEthernetDataProcessor.generated.h"

#pragma pack(push, 1)
struct  SIdasAsToIdasGERaw
{
	uint8 BodId;
	int32 BotPosLat;
	int32 BotPosLon;
	int16 VelNorthNed;
	int16 VelEastNed;
	int16 VelDownNed;
	int32 BotPosNorth;
	int32 BotPosEast;
	int32 BotPosDown;
	int16 EulerPhiDeg;
	int16 EulerThetaDeg;
	int16 EulerPsiDeg;
	int16 BodyVelSurge;
	int16 BodyVelSway;
	int16 BodyVelHeave;
	uint8 Checksum;
};
#pragma pack(pop)

/**
 * 
 */


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UIDASEthernetDataProcessor : public UEthernetDataProcessorBase
{
	GENERATED_BODY()

protected:
		vector< SIdasAsToIdasGERaw*> KinematicDataBuff;
		
		FCriticalSection CritcalSection;
		virtual void BeginPlay() override;
		INT32S KinematicDataBuffInd = 0;
		void ProcessReceivedData();
		virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	virtual void OnReceivedData(const TArray<uint8>& data, int32 bytes_read) override;
};
