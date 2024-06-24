// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/PointVisualizer/PointCloudRenderer.h"
#include <Lib/Types/Primitives.h>
#include <Lib/Utils/CUtil.h>
 // Sets default values
APointCloudRenderer::APointCloudRenderer()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

// Just the destructor:
APointCloudRenderer::~APointCloudRenderer()
{
	for (auto info : RendererInfo) {

		delete[] info->positions;
		delete[] info->colors;
		info->positions = nullptr;
	}
}

/**
 * Just a helper method to set a texture for a user variable because the UNiagaraComponent
 * has no direct way to edit a texture variable compared to float, vectors, ...
 */

void SetNiagaraVariableTexture(class UNiagaraComponent* niagara, FString variableName, UTexture* texture) {
	if (!niagara || !texture)
		return;

	FNiagaraUserRedirectionParameterStore& overrideParameters = niagara->GetOverrideParameters();
	FNiagaraVariable niagaraVariable = FNiagaraVariable(FNiagaraTypeDefinition(UNiagaraDataInterfaceTexture::StaticClass()), *variableName);

	UNiagaraDataInterfaceTexture* dataInterface = (UNiagaraDataInterfaceTexture*)overrideParameters.GetDataInterface(niagaraVariable);
	dataInterface->SetTexture(texture);
}

SRendererInfo* APointCloudRenderer::CreateInstance(int texture_width_px)
{
	SRendererInfo* info = new SRendererInfo();
	
	info->textureWidth = texture_width_px;
	info->textureHeight = texture_width_px;
	info->pointCount = info->textureWidth * info->textureHeight;

	// Initialize data arrays:
	info->positions = new float[info->textureWidth * info->textureHeight * 4];
	info->colors = new uint8_t[info->textureWidth * info->textureHeight * 4];

	memset(info->positions, 0, info->textureWidth * info->textureHeight * sizeof(float) * 4);
	memset(info->colors, 0, info->textureWidth * info->textureHeight * sizeof(uint8_t) * 4);

	info->region = FUpdateTextureRegion2D(0, 0, 0, 0, info->textureWidth, info->textureHeight);
	// Initialize the Niagara System:
	info->rendererInstance = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		this,
		pointCloudRenderer,
		FVector(0, 0, 300),
		FRotator(0),
		FVector(1),
		true,
		true,
		ENCPoolMethod::AutoRelease,
		true
	);
	info->rendererInstance->SetCastShadow(false);


	// Create dynamic texture for position:
	info->positionTexture = UTexture2D::CreateTransient(texture_width_px, texture_width_px, PF_A32B32G32R32F);
	info->positionTexture->Filter = TF_Nearest;
	info->positionTexture->UpdateResource();

	// Create dynamic texture for color:
	info->colorTexture = UTexture2D::CreateTransient(info->textureWidth, info->textureHeight, PF_B8G8R8A8);
	info->colorTexture->Filter = TF_Nearest;
	info->colorTexture->UpdateResource();

	// Set the niagara system user variables:
	SetNiagaraVariableTexture(info->rendererInstance, "User.PositionTexture", info->positionTexture);
	SetNiagaraVariableTexture(info->rendererInstance, "User.ColorTexture", info->colorTexture);

	info->rendererInstance->SetVariableInt("User.TextureWidth", info->textureWidth);
	info->rendererInstance->SetVariableInt("User.TextureHeight", info->textureHeight);
	info->rendererInstance->SetVariableInt("User.PointCount", info->pointCount);

	info->positionTexture->UpdateTextureRegions(0, 1, &info->region, info->textureWidth * 16, 16, (uint8*)info->positions);
	info->colorTexture->UpdateTextureRegions(0, 1, &info->region, info->textureWidth * 4, 4, (uint8*)info->colors);

	return info;
}

void APointCloudRenderer::CreateTextures()
{
	int cnt = TotalPointSize / (EachTextureWidth * EachTextureWidth);

	for (int i = 0; i < cnt; i++) {
		SRendererInfo *info;
		info = CreateInstance(EachTextureWidth);
		RendererInfo.Add(info);
	}

}

void APointCloudRenderer::EnablePointShow(bool val)
{
	for (int ind = 0; ind < RendererInfo.Num(); ind++) {
		SRendererInfo* info = RendererInfo[ind];
		
		if (val) {
			info->rendererInstance->Activate();
		}
		else {
			info->rendererInstance->Deactivate();
		}
		
	}
}


// Called when the game starts or when spawned
void APointCloudRenderer::BeginPlay()
{
	Super::BeginPlay();


	
}

void APointCloudRenderer::Init(int total_point_size)
{
	TotalPointSize = total_point_size;
	CreateTextures();
	EnablePointShow(false);
}

// Called every frame
void APointCloudRenderer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void APointCloudRenderer::RenderPoints(SRendererInfo* info, int from, int cnt, FVector center, const TArray<FVector>& pts, EPointCooordSystem coord_sys, int sprite_size)
{



	if (info->positions == nullptr) {
		return;
	}


	int render_point_count = (cnt > (int)info->pointCount) ? info->pointCount : cnt;


	int update_row_cnt = FMath::CeilToInt(render_point_count / (double)info->textureWidth);
	if (update_row_cnt == 0) {
		update_row_cnt = 1;
	}

	update_row_cnt = FMath::Max(update_row_cnt, info->LastUpdatedRowCount);

	memset(info->positions, 0, info->textureWidth * update_row_cnt * sizeof(float) * 4);
	memset(info->colors, 0, info->textureWidth * update_row_cnt * sizeof(uint8_t) * 4);


	if (coord_sys == EPointCooordSystem::PointCooordSystemRightHand) {
		for (int i = 0; i < render_point_count; i++) {
			FVector pos = pts[from+i];
			pos.Y *= -1;

			pos = TOUE(pos) + center;
			int offset = 4 * i;

			info->positions[offset + 0] = pos.X;
			info->positions[offset + 1] = pos.Y;
			info->positions[offset + 2] = pos.Z;
			info->positions[offset + 3] = 1;

			info->colors[offset + 2] = 255; 					      // < r
			info->colors[offset + 3] = 255; // a;
		}
	}
	else if (coord_sys == EPointCooordSystem::PointCooordSystemLeftHand) {
		for (int i = 0; i < render_point_count; i++) {
			FVector pos = pts[from+i];


			pos = TOUE(pos) + center;
			int offset = 4 * i;

			info->positions[offset + 0] = pos.X;
			info->positions[offset + 1] = pos.Y;
			info->positions[offset + 2] = pos.Z;
			info->positions[offset + 3] = 1;

			info->colors[offset + 2] = 255; 					      // < r
			info->colors[offset + 3] = 255; // a;
		}
	}

	info->region = FUpdateTextureRegion2D(0, 0, 0, 0, info->textureWidth, update_row_cnt);
	//region =  FUpdateTextureRegion2D(0, 0, 0, 0, textureWidth, textureHeight );
	info->rendererInstance->SetVariableInt("User.SpriteSize", sprite_size);
	info->positionTexture->UpdateTextureRegions(0, 1, &info->region, info->textureWidth * 16, 16, (uint8*)info->positions);
	info->colorTexture->UpdateTextureRegions(0, 1, &info->region, info->textureWidth * 4, 4, (uint8*)info->colors);

	info->LastUpdatedRowCount = update_row_cnt;
}

void APointCloudRenderer::SetPoints(FVector center, const TArray<FVector>& pts, EPointCooordSystem coord_sys, int sprite_size)
{
	int each_size = EachTextureWidth * EachTextureWidth;

	int from = 0;

	int prev_last_inddex = LastIndex;

	for(int  ind = 0; ind< RendererInfo.Num(); ind++){
		SRendererInfo *info = RendererInfo[ind];

		if (!info->rendererInstance->IsActive()) {
			info->rendererInstance->SetVisibility(true, true);

			// Method 2: Reactivate the Component
			info->rendererInstance->SetComponentTickEnabled(true);
			info->rendererInstance->SetRenderingEnabled(true);
			info->rendererInstance->Activate(true);
			info->rendererInstance->ReinitializeSystem();
		}
		int cnt = (pts.Num() - from) > each_size ? each_size : (pts.Num() - from);


		RenderPoints(info, from, cnt, center, pts, coord_sys, sprite_size);
		from += cnt;
		LastIndex = ind;
	
		if (from >= pts.Num()) {
			break;
		}

	}


	for (int i = LastIndex+1; i <= (prev_last_inddex); i++){
		ClearInfo(RendererInfo[i]);
	}
	


}

void APointCloudRenderer::ClearInfo(SRendererInfo* info)
{
	memset(info->positions, 0, info->textureWidth * info->textureHeight * sizeof(float) * 4);
	memset(info->colors, 0, info->textureWidth * info->textureHeight * sizeof(uint8_t) * 4);

	info->region = FUpdateTextureRegion2D(0, 0, 0, 0, info->textureWidth, info->textureHeight);

	info->positionTexture->UpdateTextureRegions(0, 1, &info->region, info->textureWidth * 16, 16, (uint8*)info->positions);
	info->colorTexture->UpdateTextureRegions(0, 1, &info->region, info->textureWidth * 4, 4, (uint8*)info->colors);
	info->LastUpdatedRowCount = -1;
	info->rendererInstance->Deactivate();
}

void APointCloudRenderer::ClearAll()
{

	for (auto info : RendererInfo) {
		ClearInfo(info);
	}


}