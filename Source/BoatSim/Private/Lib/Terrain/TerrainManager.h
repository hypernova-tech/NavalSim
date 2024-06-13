// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Lib/ActorBase/ActorBase.h"
#include <ProceduralMeshComponent.h>
#include "IImageWrapper.h"
#include "PixelFormat.h"
#include <Lib/Origin/MapOrigin.h>
#include "TerrainManager.generated.h"


#define TERRAIN_INVALID_VAL (double)(DBL_MAX);



/**
 * 
 */
UCLASS()
class ATerrainManager : public AActorBase
{
	GENERATED_BODY()
public:
	
		ATerrainManager();

public:
	UPROPERTY(EditAnywhere)
		FString HeightMapPath = "";

	UPROPERTY(EditAnywhere)
		FString DepthMapPath = "";

	UPROPERTY(EditAnywhere)
		FString BaseTexturePath = "";

	UPROPERTY(EditAnywhere)
		double HeightMapMinLevelMeter = TERRAIN_INVALID_VAL;

	UPROPERTY(EditAnywhere)
		double HeightMapMaxLevelMeter = TERRAIN_INVALID_VAL;

	UPROPERTY(EditAnywhere)
		double DepthMapMinLevelMeter = TERRAIN_INVALID_VAL;

	UPROPERTY(EditAnywhere)
		double DepthMapMaxLevelMeter = TERRAIN_INVALID_VAL;

	UPROPERTY(EditAnywhere)
		FVector TerrainTopLeftCornerLLH;

	UPROPERTY(EditAnywhere)
		FVector TerrainBottomRightCornerLLH;
protected:
	virtual void BeginPlay() override;
	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	TArray<FVector> Vertices;
	TArray<FLOAT64> Elevations;
	TArray<int32> Triangles;
	TArray<FVector2D> UVs;
	TArray<FVector> Normals;
	ECoordSystem CoordSystem = ECoordSystem::CoordSystemLLH_WGS84;
	FLOAT64 MapWidthPix;
	FLOAT64 MapHeightPix;
	
	FVector TerrainBottomLeftCornerXYZMeter;
	FVector TerrainTopRightCornerXYZMeter;

	FLOAT64 OneOverXStepDistMeter;
	FLOAT64 OneOverYStepDistMeter;


	UTexture2D* LoadPNGTextureFromFile(const FString& ImagePath, ERGBFormat format, INT32S bit_depth, EPixelFormat px_format);






	UProceduralMeshComponent* ProceduralMesh;
	UMaterialInstanceDynamic* pDynamicMaterial;
	bool IsDoubleUpdated(double val);
	bool IsVectorUpdated(FVector vec);
	
	UTexture2D* LoadPNGAsTexture2D(const FString& ImagePath);
	//void UpdateLandscapeHeightFromTexture(ALandscape* Landscape, UTexture2D* HeightmapTexture, FLOAT64 min_height_meter, FLOAT64 max_height_meter);
	void BuildTerrain(FString png_path, FString depth_map, FLOAT64 min_height, FLOAT64 max_height, FVector top_left, FVector bottom_right);
	//bool SetHeightmapData(ALandscapeProxy* Landscape, const TArray<uint16>& Data);
	//void SetAll(float h);
	//virtual void OnStepScenarioMode(float DeltaTime) override;
	//FIntPoint GetLandscapeHeightmapSize(ALandscape* Landscape);
	//FLOAT64 GetTextureValue(UTexture2D *p_text , FLOAT64 tf_x, FLOAT64 tf_y);
	void GenerateTerrain(const TArray<FLOAT64>& HeightMap, int32 MapWidth, int32 MapHeight, FVector top_left, FVector bottom_right, const TArray<FLOAT64>& depth_map);
	void CalculateVertices(const TArray<FLOAT64>& HeightMap, int32 MapWidth, int32 MapHeight, FVector top_left, FVector bottom_right, const TArray<FLOAT64>& depth_map, TArray<FVector>& OutVertices);
	void CalculateTriangles(int32 MapWidth, int32 MapHeight, TArray<int32>& OutTriangles);
	void CalculateUVs(int32 MapWidth, int32 MapHeight, TArray<FVector2D>& OutUVs);
	void CalculateNormals(const TArray<FVector>& Vertices, int32 Width, int32 Height, TArray<FVector>& Normals);
	void GetDataTextureData(UTexture2D* Texture, FLOAT64 min_height_meter, FLOAT64 max_height_meter, TArray<FLOAT64>& data);

	UTexture2D* LoadTextureFromPath(const FString& Path);
	UMaterialInstanceDynamic* CreateDynamicMaterialInstance(UProceduralMeshComponent* mesh_comp, const FString& mat_path);
	void ApplyTextureToMesh(UProceduralMeshComponent* mesh_comp, UMaterialInstanceDynamic* mat_inst, UTexture2D* texture);
	// Example function that sets up texture and material for the mesh
	void SetupMeshMaterial(const FString& texture_path, const FString& mat_path);
	void SetBaseMap(const FString& text_path);
	FVector GetTerrianCoordUE();
public: 


	UPROPERTY(EditAnywhere)
		UMaterialInterface* pMaterial;



	void SetTerrainTopLeftCornerLLH(FVector val);
	FVector GetTerrainTopLeftCornerLLH();

	void SetTerrainRightBottomCornerLLH(FVector val);
	FVector GetTerrainRightBottomCornerLLH();

	virtual void Bake() override;
	virtual void Save(ISaveLoader* p_save_loader) override;
	virtual void SaveJSON(CJsonDataContainer& data) override;
	virtual void UpdateAnnotation(bool is_enabled) override;
	inline FLOAT64 GetElevation(FLOAT64 x_meter, FLOAT64 y_meter, bool& ret)
	{
		ret = false;
		if (x_meter < TerrainBottomLeftCornerXYZMeter.X) {
			return 0;
		}
		if (x_meter > TerrainTopRightCornerXYZMeter.X) {
			return 0;
		}
		if (y_meter < TerrainBottomLeftCornerXYZMeter.Y) {
			return 0;
		}
		if (y_meter > TerrainTopRightCornerXYZMeter.Y) {
			return 0;
		}

		int ind_x = (int) ((x_meter - TerrainBottomLeftCornerXYZMeter.X) * OneOverXStepDistMeter + 0.5);
		int ind_y = (int) ((y_meter - TerrainBottomLeftCornerXYZMeter.Y) * OneOverYStepDistMeter + 0.5);

		auto elev = Elevations[ind_x * MapWidthPix + ind_y];
		ret = true;
		return elev;
	}



	
};
