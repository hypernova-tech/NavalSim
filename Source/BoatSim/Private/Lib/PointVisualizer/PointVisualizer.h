// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Engine/TextureRenderTarget2D.h"
#include <Lib/Utils/CScanResult.h>
#include <Components/Image.h>
#include "PointCloudRenderer.h"
#include "PointVisualizer.generated.h"




UCLASS()
class APointVisualizer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APointVisualizer();

	UPROPERTY(EditAnywhere)
		UTextureRenderTarget2D* pRenderTarget;

	UPROPERTY(EditAnywhere)
		UMaterialInterface* pBaseMaterial;
	

	void RenderPointCloud(FVector loc, const TArray<FVector>& pts);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	int RenderCount = 0;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void Visualize(SScanResult *p_scan_result, FVector origin, FVector current_forward, FVector current_right, float max_range_meter, void *p_tracker = nullptr, bool reset= true);
	virtual void Visualize(const TArray<FVector> &pts, EPointCooordSystem coord_sys, FVector origin, FVector current_forward, FVector current_right, float max_range_meter, void* p_tracker = nullptr, bool reset = true);

	void SetPixelValue(UTextureRenderTarget2D* RenderTarget, int32 X, int32 Y, FColor Color);

	UTextureRenderTarget2D* CreateRenderTarget(int width, int height, UImage *p_image);
	UMaterialInstanceDynamic* CreateMaterialInstanceWithTexture(UObject* Outer, UMaterialInterface* BaseMaterial, UTexture* Texture, UImage* p_image);
};
