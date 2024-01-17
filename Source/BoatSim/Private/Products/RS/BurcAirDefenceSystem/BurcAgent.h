// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NiagaraSystem.h"
#include "CoreMinimal.h"
#include "Lib/Agent/Agent.h"
#include <Lib/Gimbal/GimbalBase.h>
#include "BurcAgent.generated.h"


/**
 * 
 */

enum EBurcState
{
	BurcStateIdle,
	BurcStateWaitTargetDetection,
	BurcStateWaitRange,
	BurcStateFire,
	BurcStateFiring,
	BurcStateFinished,
};

UCLASS()
class ABurcAgent : public AAgent
{
	GENERATED_BODY()
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void OnStep(float DeltaTime) override;
	virtual void OnPreStep(float DeltaTime);
	UFUNCTION(BlueprintCallable)
		void Fire_();

	UFUNCTION(BlueprintCallable)
		void FireSerial_(int count, double time_interval);

	UFUNCTION(BlueprintCallable)
		void AimGun_(FVector pos);


	UPROPERTY(EditAnywhere)
		FString GunTipName_;

	UPROPERTY(EditAnywhere)
		FString GunName_;

	UPROPERTY(EditAnywhere)
		FString BulletAgentName_ = "BULLET";

	UPROPERTY(EditAnywhere)
		FString TargetNameName_;

	UPROPERTY(EditAnywhere)
		double BulletLifeTimeSec_ = 1;

	UPROPERTY(EditAnywhere)
		FString TaretGimbalName_;

	UPROPERTY(EditAnywhere)
		FString EOGimbalName_;

	UPROPERTY(EditAnywhere)
		FString GunGimbalName_;

	UPROPERTY(EditAnywhere)
		double MaxFireDistanceMeter_;

	UPROPERTY(EditAnywhere)
		double MinFireDistanceMeter_;


	UPROPERTY(EditAnywhere)
		int SerialFireCount_;

	UPROPERTY(EditAnywhere)
		double SerialFireTimeIntervalSec_;



	UFUNCTION(BlueprintCallable)
		bool AssignTarget_(FString target_name, double duration);


	FVector GetAimDirection();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AActor* pGunTip;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AActor* pGun;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AGimbalBase* pTaretGimbal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AGimbalBase* pEOGimbal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AGimbalBase* pGunGimbal;

	FTimerHandle SerialFireTimerHandle;

	AActor* pTarget;
	

	AActor* CloneBullet();
	void SerialFireNext();

	


	UPROPERTY(EditAnywhere, Category = "Shake")
		float ShakeDuration = 0.5f;  // Duration of the shake in seconds

	UPROPERTY(EditAnywhere, Category = "Shake")
		float ShakeIntensity = 0.0f;  // Intensity of the shake
	UPROPERTY(EditAnywhere, Category = "Shake")
		float ShakeRotationIntensity = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Shake")
		float ShakeFrequency = 0.05f;  // Time between each shake update


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
		UNiagaraSystem* MuzzleEffect;


	void SpawnMuzzleVFX();

	float ShakeTimer = 0.0f;
	bool bIsShaking = false;
	bool IsSerialFiring = false;
	INT32S RemainingSerialFire = 0;
	FLOAT64 NextFireTime;
	FRotator TotalAddedRotation;
	FVector TotalAddedOffset;
	EBurcState BurcState = EBurcState::BurcStateIdle;
	void StartShake();
	void ApplyShake();
	void AimGimbal(AGimbalBase* p_gimbal, FVector target);
	void StateMachine();
	double GetTargetRange();
	bool IsInFireRange();
};
