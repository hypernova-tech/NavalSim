// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Lib/ActorBase/ActorBase.h"
#include "Lib/SOA/ISOAObserver.h"
#include <Lib/SOACommonObserverArgs/PlatformKinematicData.h>
#include "ActorAsPlatform.generated.h"

/**
 * 
 */
UCLASS()
class AActorAsPlatform : public AActorBase, public ISOAObserver
{
	GENERATED_BODY()

protected:
	UPlatformKinematicData* pLastData;
	bool IsDirty;
	void UpdateKinematicData();
public:
	virtual void BeginPlay();
	virtual void Update(UCSOAObserverArgs* p_args) override;
};
