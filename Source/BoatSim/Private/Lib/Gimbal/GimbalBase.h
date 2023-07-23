// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GimbalBase.generated.h"


USTRUCT(BlueprintType)
struct FSGimbalAxisInfo {
	GENERATED_BODY()

public:


	UPROPERTY(EditAnywhere)
		bool Enabled;

	UPROPERTY(EditAnywhere)
		double InitialAngleDeg;

	UPROPERTY(EditAnywhere)
		double MaxLimitAngleDeg;

	UPROPERTY(EditAnywhere)
		double MinLimitAngleDeg;
	
	UPROPERTY(EditAnywhere)
		double AngleSpeedDegPerSec;

	UPROPERTY(EditAnywhere)
		double CommandAngleDeg;
private:
	double CurrentAngleDeg;


public:
	double GetCurrentAngleDeg()
	{
		return CurrentAngleDeg;
	}

	void SetCurrentAngleDeg(double val)
	{
		CurrentAngleDeg = val;
	}

	float GetCommandAngleDeg()
	{
		return CommandAngleDeg;
	}

	void SetCommandAngleDeg(double val)
	{
		CommandAngleDeg = val;
	}
};


enum EGimbalState
{
	GimbalInit,
	GimbalRunning,
	GimbalFinish,

};

enum EGimbalAxis
{
	Roll = 0,
	Pitch,
	Yaw
};


UCLASS()
class AGimbalBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGimbalBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
		FSGimbalAxisInfo RollAxis;

	UPROPERTY(EditAnywhere)
		FSGimbalAxisInfo PitchAxis;

	UPROPERTY(EditAnywhere)
		FSGimbalAxisInfo YawAxis;

	UPROPERTY(EditAnywhere)
		TArray<AActor*> AttachedActors;


	EGimbalState GimbalState = EGimbalState::GimbalInit;


	virtual void GimbalStateMachine(float delta_time_sec);

	virtual void InitGimbal();
	virtual void Run(float delta_time_sec);
	virtual void FinalizeGimbal();

	virtual void UpdateAxis(EGimbalAxis axis, float delta_time_sec);
	FSGimbalAxisInfo* GetAxis(EGimbalAxis axis);
	float GetAxisAngleDeg(EGimbalAxis axis);
	UFUNCTION(BlueprintCallable)
	void UpdateAttachedActors(FVector rotation);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void SetCommand(float roll_angle_deg, float pitch_angle_deg, float yaw_angle_deg);

};
