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
	UStaticMeshComponent* pMainMesh;
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent *HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	TSet< AActor*> CollidedActors;
public:
	virtual void BeginPlay();
	virtual void Update(UCSOAObserverArgs* p_args) override; // obsever update
	virtual bool  IsColldingWithBlockingObject() override;
	virtual bool CanMoveTowards(FVector dir) override;
};
