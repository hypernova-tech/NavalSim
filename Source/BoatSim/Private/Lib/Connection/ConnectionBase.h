// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <Lib/UDP/IConnectionDataReceiver.h>
#include "ConnectionDefs.h"
#include "ConnectionBase.generated.h"




UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UConnectionBase : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UConnectionBase();

	UPROPERTY(EditAnywhere)
		bool IsActiveAtStartup = false;

	

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	TArray< IConnectionDataReceiver*> ConnectionDataReceivers;
	bool IsRunning = false;
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void InitConnection();
	void AddConnectionDataReceiver(IConnectionDataReceiver* p_rcv);
	bool GetIsRunning();
	virtual void OnModifiedConnection(SConnectionInfo info);
};
