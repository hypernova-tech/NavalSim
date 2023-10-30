// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Lib/Types/Primitives.h"
#include <Lib/SaveLoad/ISaveLoader.h>
#include "ActorBase.generated.h"



UCLASS()
class AActorBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AActorBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
		bool IsSaveEnabled = true;

	UPROPERTY(EditAnywhere)
		FString BlueprintName = "";


	UPROPERTY(EditAnywhere)
		int ActorId;

	UPROPERTY(EditAnywhere)
		bool IsExternalUpdate = true;

	UPROPERTY(EditAnywhere)
		bool Enabled = false;


	UPROPERTY(EditAnywhere)
		int AffinityInstanceId = -1;

	UPROPERTY(EditAnywhere)
		AActor* ActorGizmo;

	 virtual void OnStep(float DeltaTime);
	 bool CheckAffinity();

	 virtual void OnActorDisabled();
	 virtual void OnActorEnabled();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void SetEnabled(bool val);
	bool GetEnabled();
	void ExternalUpdate(float DeltaTime);
	void SetAffinityInstanceId(int val);
	int GetAffinityInstanceId();
	FString GetBlueprintName();
	virtual void Save(ISaveLoader* p_save_loader);
	void ShowActorGizmo(bool val);
	virtual void Bake();
	virtual void OnActorPredestroy();
	void SetIsSaveEnabled(bool val);
	bool GetIsSaveEnabled();

	virtual void SetActorPosition(FVector pos);
	virtual void SetActorRelPosition(FVector pos);
	virtual void SetActorRot(FVector rpy_deg);
	virtual void SetActorRelRot(FVector rpy_deg);
	
};
