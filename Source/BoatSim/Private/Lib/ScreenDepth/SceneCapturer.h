// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Lib/Types/Primitives.h"
#include "SceneCapturer.generated.h"


UCLASS()
class ASceneCapturer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASceneCapturer();



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	USceneCaptureComponent2D* pSceneCapture;
	UTextureRenderTarget2D* pRenderTarget;
	FTextureRenderTargetResource* pRenderTargetResource;
	TArray<FColor> OutBMP;
	void* pOwner;

	void StartReadPixelsAsync();
	void OnReadbackComplete();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void CreateRenderTexture(void *ownersensor, INT32U width_px, INT32U height_px, EPixelFormat pixel_format);
	virtual void Capture();
	virtual void ReadPixels();

};
