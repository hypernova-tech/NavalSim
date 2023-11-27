// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/Image.h>
#include <Components/TextBlock.h>
#include <Lib/Types/Primitives.h>
#include "GizmoUIController.h"
#include "UIControllerBase.generated.h"





UCLASS()
class AUIControllerBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUIControllerBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	UPROPERTY(EditAnywhere)
		UTextBlock* ConsoleText;

	UPROPERTY(EditAnywhere)
		AActor* GizmoActor;

	UPROPERTY(EditAnywhere)
		TArray<UImage*> LidarSlots;


	UPROPERTY(EditAnywhere)
		TArray<UImage*> CameraSlots;

	virtual void ComputeFPS(float DeltaTime);

	UGizmoUIController* pGizmoUIController;
	bool IsUIVisible = false;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SetLidarSlotImage(int no, UImage* p_image);

	UFUNCTION(BlueprintCallable)
		UImage* GetLidarSlotImage(int no);

	UFUNCTION(BlueprintCallable)
		int GetLidarSlotCount();

	UFUNCTION(BlueprintCallable)
		void SetCameraSlotImage(int no, UImage* p_image);

	UFUNCTION(BlueprintCallable)
		UImage* GetCameraSlotImage(int no);

	UFUNCTION(BlueprintCallable)
		int GetCameraSlotCount();

	UFUNCTION(BlueprintCallable)
		void SetConsoleOutputTextWidget(UTextBlock* pwidget);

	UFUNCTION(BlueprintCallable)
		void OnMouseLeftButtonDown(int locationX, int locationY);

	UFUNCTION(BlueprintCallable)
		void OnMouseLeftButtonUp(int locationX, int locationY);

	UFUNCTION(BlueprintCallable)
		float GetAverageFPS();

	UFUNCTION(BlueprintCallable)
		bool GetIsUIVisible();

	UFUNCTION(BlueprintCallable)
		void SetIsUIVisible(bool val);

	void FindActorAtClickPosition(int locationX, int locationY);


	void SetConsoleOutputText(FString text);
	void SelectActor(AActor* p_actor);

	UGizmoUIController* GetGizmoController();



private:

	FLOAT32 AverageFPS;
	TArray< FLOAT32> FPSMeasurments;
	AActor* pSelectedActor;
	INT64U TotalFrameCount = 0;
	FLOAT64 LastFrameTime;
};
