// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Lib/SOA/ISOAObserver.h"
#include <Lib/Sensor/SensorBase.h>
#include <Lib/Platform/IPlatform.h>
#include "CBoatBase.generated.h"




class UCameraComponent;

UCLASS()
class  ACBoatBase : public APawn, public ISOAObserver, public IPlatform
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACBoatBase();

	///TArray<ASensorBase> Sensors;

	UFUNCTION(BlueprintCallable)
		void OnForwardKey(bool pressed, bool released);
	UFUNCTION(BlueprintCallable)
		void OnBackwardkKey(bool pressed, bool released);
	UFUNCTION(BlueprintCallable)
		void OnLeftKey(bool pressed, bool released);
	UFUNCTION(BlueprintCallable)
		void OnRightKey(bool pressed, bool released);

	UFUNCTION(BlueprintCallable)
	void OnUpKeyPressed(bool pressed, bool released);

	UFUNCTION(BlueprintCallable)
	void OnDownKeyPressed(bool pressed, bool released);

	UFUNCTION(BlueprintCallable)
		void StopPawn();

	UPROPERTY(EditAnywhere)
		FString BlueprintName = "";

	UPROPERTY(BlueprintReadWrite)
	float MovementSpeedUnitPerSec = 500;

	UPROPERTY(EditAnywhere)
	float OscillationAmplitude = 1;
	UPROPERTY(EditAnywhere)
	float OscillationFreq = 1;


	UPROPERTY(EditAnywhere)
	float CamMovementSpeed = 100;
	

	virtual void PostInitializeComponents() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void BindedMoveForward(float val);
	void BindedMoveRight(float val);
	void BindedRotationX(float val);
	void BindedRotationY(float val);
	void OnRightMousePressed();

	void OnRightMouseReleased();
	void AdjustCameraDistance(float val);
	void OnFocusEnter();
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    virtual void Update(UCSOAObserverArgs* p_args) override;
	virtual void OnControllerChanged();
	bool FocusCamera(AActor* p_actor);
private:
	UCameraComponent* BoatCam;
	bool IsForward;
	bool IsBackward;
	bool IsLeft;
	bool IsRight;
	bool IsUp;
	bool IsDown;
	bool bIsRightMousePressed;
	float TimeSec;
	void Oscillate();



	// Inherited via IPlatform
	virtual void SetMaxSpeedMeterPerSec(FLOAT32 speed) override;

	virtual FLOAT32 GetMaxSpeedMeterPerSec() override;
	
};

typedef ACBoatBase APlatformBase;