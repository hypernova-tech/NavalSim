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
		TArray<UImage*> SensorSlots;

	UPROPERTY(EditAnywhere)
		TArray<UTextBlock*> SensorTitles;

	virtual void ComputeFPS(float DeltaTime);

	UGizmoUIController* pGizmoUIController;
	bool IsUIVisible = false;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SetSensorSlotImage(int no, UImage* p_image);

	UFUNCTION(BlueprintCallable)
		UImage* GetSensorSlotImage(int no);

	UFUNCTION(BlueprintCallable)
		int GetSensorSlotCount();


	UFUNCTION(BlueprintCallable)
		void SetSensorTitle(int no, UTextBlock* p_image);

	UFUNCTION(BlueprintCallable)
		UTextBlock* GetSensorTitle(int no);

	void SetSensorTitleText(int no, FString val);

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
