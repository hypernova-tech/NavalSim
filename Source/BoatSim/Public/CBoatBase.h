// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CBoatBase.generated.h"


UCLASS()
class BOATSIM_API ACBoatBase : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACBoatBase();

	UFUNCTION(BlueprintCallable)
		void OnForwardKey(bool pressed, bool released);
	UFUNCTION(BlueprintCallable)
		void OnBackwardkKey(bool pressed, bool released);
	UFUNCTION(BlueprintCallable)
		void OnLeftKey(bool pressed, bool released);
	UFUNCTION(BlueprintCallable)
		void OnRightKey(bool pressed, bool released);

	UFUNCTION(BlueprintCallable)
		void StopPawn();


	UPROPERTY(BlueprintReadWrite)
	float MovementSpeedUnitPerSec = 500;

	UPROPERTY(EditAnywhere)
	float OscillationAmplitude = 1;
	UPROPERTY(EditAnywhere)
	float OscillationFreq = 1;

	virtual void PostInitializeComponents() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


private:
	bool IsForward;
	bool IsBackward;
	bool IsLeft;
	bool IsRight;

	float TimeSec;


	void Oscillate();

};
