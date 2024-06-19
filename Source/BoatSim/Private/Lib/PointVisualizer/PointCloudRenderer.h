// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraDataInterfaceTexture.h"
#include "Engine/Texture2D.h"
#include "PointCloudRenderer.generated.h"

UCLASS()
class APointCloudRenderer : public AActor
{
	GENERATED_BODY()


public:
	// Sets default values for this actor's properties
	APointCloudRenderer();
	~APointCloudRenderer();

	// The niagara system which should be used for rendering:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX")
		UNiagaraSystem* pointCloudRenderer;

private:
	// The instance of the niagara system:
	UNiagaraComponent* rendererInstance;

	// Textures to transfer the data to the niagara system:
	UTexture2D* positionTexture;
	UTexture2D* colorTexture;

	// further data:
	uint32_t textureWidth;
	uint32_t textureHeight;
	uint32_t pointCount;

	// The data which has to be written into the textures:
	float* positions;
	uint8_t* colors;

	// precalculated part of the sine function:
	float* precalc;

	// Runtime of the game:
	float runtime;

	// Definition of the region which should be updated:
	FUpdateTextureRegion2D region;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void SetPoints(FVector center, const TArray<FVector>& vec);

};

