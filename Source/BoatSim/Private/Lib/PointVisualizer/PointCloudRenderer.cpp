// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/PointVisualizer/PointCloudRenderer.h"
#include <Lib/Types/Primitives.h>
 // Sets default values
APointCloudRenderer::APointCloudRenderer()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Just define how many points you want. You can define pointCount arbitrary as long as 
	// textureWidth * textureHeight is greater or equals pointCount:
	textureWidth = 512;
	textureHeight = 512;
	pointCount = textureWidth * textureHeight;

	// Initialize data arrays:
	positions = new float[textureWidth * textureHeight * 4];
	colors = new uint8_t[textureWidth * textureHeight * 4];

	memset(positions, 0, textureWidth * textureHeight * sizeof(float) * 4);
	memset(colors, 0, textureWidth * textureHeight * sizeof(uint8_t) * 4);

#if false
	// Now, we have to update the point positions & colors:
	for (uint32_t x = 0; x < textureWidth; ++x) {
		for (uint32_t y = 0; y < textureHeight; ++y) {
			int id = x + y * textureWidth;

			// Write some test data in the arrays:
			positions[id * 4 + 0] = float(x) / textureWidth * 5000 - 500;  // < x
			positions[id * 4 + 1] = float(y) / textureHeight * 5000 - 500; // < y
			positions[id * 4 + 2] = 0;
			positions[id * 4 + 3] = 1; // < could be removed, currently not used.

			colors[id * 4 + 0] = 0;  // < b
			colors[id * 4 + 1] = 0;// uint8_t(float(y) / textureHeight * 255); // < g
			colors[id * 4 + 2] = 255; uint8_t(float(x) / textureWidth * 255);									      // < r
			colors[id * 4 + 3] = 255;									  // < a
		}
	}

	// Precalculate a part of the sine function to avoid these costly calls in the tick function 
	// for every point:
	precalc = new float[textureWidth];
	for (uint32 i = 0; i < textureWidth; ++i) {
		precalc[i] = sin(i / float(textureWidth) * 3.14159f * 2);
	}
#endif

	region = FUpdateTextureRegion2D(0, 0, 0, 0, textureWidth, textureHeight);
}

// Just the destructor:
APointCloudRenderer::~APointCloudRenderer()
{
	delete[] positions;
	delete[] colors;
	positions = nullptr;
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




// Called when the game starts or when spawned
void APointCloudRenderer::BeginPlay()
{
	Super::BeginPlay();

	// Initialize the Niagara System:
	rendererInstance = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
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
	rendererInstance->SetCastShadow(true);

	// Create dynamic texture for position:
	positionTexture = UTexture2D::CreateTransient(textureWidth, textureHeight, PF_A32B32G32R32F, "PositionTexture");
	positionTexture->Filter = TF_Nearest;
	positionTexture->UpdateResource();

	// Create dynamic texture for color:
	colorTexture = UTexture2D::CreateTransient(textureWidth, textureHeight, PF_B8G8R8A8, "ColorTexture");
	colorTexture->Filter = TF_Nearest;
	colorTexture->UpdateResource();

	// Set the niagara system user variables:
	SetNiagaraVariableTexture(rendererInstance, "User.PositionTexture", positionTexture);
	SetNiagaraVariableTexture(rendererInstance, "User.ColorTexture", colorTexture);

	rendererInstance->SetVariableInt("User.TextureWidth", textureWidth);
	rendererInstance->SetVariableInt("User.TextureHeight", textureHeight);
	rendererInstance->SetVariableInt("User.PointCount", pointCount);

	runtime = 0;
}

// Called every frame
void APointCloudRenderer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

#if false
	runtime += DeltaTime;

	// Now, we have to update the point positions & colors:
	for (uint32_t x = 0; x < textureWidth; ++x) {
		for (uint32_t y = 0; y < textureHeight; ++y) {
			int id = x + y * textureWidth;

			int pIndexX = int(x + runtime * 100) % textureWidth;
			int pIndexY = int(y + runtime * 100) % textureHeight;

			positions[id * 4 + 2] = precalc[pIndexX] * precalc[pIndexY] * 100;


		
		}
	}

	// Now, bring the data into the texture:
	positionTexture->UpdateTextureRegions(0, 1, &region, textureWidth * 16, 16, (uint8*)positions);
	colorTexture->UpdateTextureRegions(0, 1, &region, textureWidth * 4, 4, (uint8*)colors);
#endif
}

void APointCloudRenderer::SetPoints(FVector center, const TArray<FVector>& pts)
{
	if (positions == nullptr) {
		return;
	}
	memset(positions, 0, textureWidth * textureHeight * sizeof(float) * 4);
	memset(colors, 0, textureWidth * textureHeight * sizeof(uint8_t) * 4);
	
	for (int i = 0; i < pts.Num() && i< pointCount; i++) {
		FVector vec = TOUE(pts[i]) + center;
		int offset = 4 * i;

		
		
		positions[offset + 0] = vec.X;
		positions[offset + 1] = vec.Y;
		positions[offset + 2] = vec.Z;
		positions[offset + 3] = 1;

		colors[offset + 2] = 255; 					      // < r
		colors[offset + 3] = 255; // a;
	}
	
	positionTexture->UpdateTextureRegions(0, 1, &region, textureWidth * 16, 16, (uint8*)positions);
	colorTexture->UpdateTextureRegions(0, 1, &region, textureWidth * 4, 4, (uint8*)colors);
}

