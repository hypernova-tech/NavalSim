// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/PointVisualizer/PointVisualizer.h"
#include <Kismet/GameplayStatics.h>
#include <CanvasItem.h>


// Sets default values
APointVisualizer::APointVisualizer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APointVisualizer::BeginPlay()
{
	Super::BeginPlay();
    // Get all actors in the level
    

	
}

// Called every frame
void APointVisualizer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APointVisualizer::Visualize(SScanResult *p_scan_result)
{
    //SetPixelValue(pRenderTarget, 50, 50, FColor::Green);


    if (!pRenderTarget)
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid render target."));
        return;
    }



    FRenderTarget* RenderTargetResource = pRenderTarget->GameThread_GetRenderTargetResource();




    FCanvas Canvas(RenderTargetResource, nullptr, 0, 0, 0, ERHIFeatureLevel::SM5);

    // Clear the canvas with the desired color to avoid artifacts from previous frames
    Canvas.Clear(FColor::Black);
    Canvas.SetAllowedModes(FCanvas::ECanvasAllowModes::Allow_Flush);


    FIntPoint rt_size =  RenderTargetResource->GetSizeXY();

   

    for (FVector2D vec : p_scan_result->Point2DScreen) {
        
        // Draw a single pixel at the specified location with the desired color
               
        FVector2D size(10, 10);
        FCanvasTileItem TileItem(FVector2D(vec.X * rt_size.X, vec.Y * rt_size.Y), size, FLinearColor::Red);

        TileItem.BlendMode = SE_BLEND_Opaque;
        Canvas.DrawItem(TileItem);

            
        
    }

    Canvas.Flush_GameThread();

}


void APointVisualizer::SetPixelValue(UTextureRenderTarget2D* RenderTarget, int32 X, int32 Y, FColor Color)
{

    if (!RenderTarget)
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid render target."));
        return;
    }

#if true

    FRenderTarget* RenderTargetResource = RenderTarget->GameThread_GetRenderTargetResource();

  
 
    
    FCanvas Canvas(RenderTargetResource, nullptr, 0,0,0, ERHIFeatureLevel::SM5);

    // Clear the canvas with the desired color to avoid artifacts from previous frames
    Canvas.Clear(FColor::Black);
    Canvas.SetAllowedModes(FCanvas::ECanvasAllowModes::Allow_Flush);

    for (int i = X; i < X + 50; i++) {
        for (int j = Y; j < Y + 50; j++) {
            // Draw a single pixel at the specified location with the desired color
            FVector2D start(i, j );
            FVector2D size(1 , 1 );
            FCanvasTileItem TileItem(start, size, FLinearColor::Red);

            TileItem.BlendMode = SE_BLEND_Opaque;
            Canvas.DrawItem(TileItem);
           
        }
    }

 
    Canvas.Flush_GameThread();
   

    UE_LOG(LogTemp, Error, TEXT("Canvas Drawn"));

#endif

#if false

    if (!RenderTarget)
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid render target."));
        return;
    }

    UCanvasRenderTarget2D* CanvasRenderTarget = UCanvasRenderTarget2D::CreateCanvasRenderTarget2D(this, UCanvasRenderTarget2D::StaticClass(), RenderTarget->SizeX, RenderTarget->SizeY);
    if (!CanvasRenderTarget)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create CanvasRenderTarget2D."));
        return;
    }

    // Draw on the CanvasRenderTarget2D to set the pixel color
    UCanvas* Canvas = nullptr;
    if (!CanvasRenderTarget->GetCanvas(Canvas, 0, 0))
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to get canvas."));
        return;
    }

    FCanvasTileItem TileItem(FVector2D(X, Y), FVector2D(1, 1), FLinearColor(Color));
    TileItem.BlendMode = SE_BLEND_Translucent;
    Canvas->DrawItem(TileItem);
   

    // Copy the pixel data from the CanvasRenderTarget2D to the TextureRenderTarget2D
    FTextureRenderTargetResource* CanvasResource = CanvasRenderTarget->GameThread_GetRenderTargetResource();
    FTextureRenderTargetResource* RenderTargetResource = RenderTarget->GameThread_GetRenderTargetResource();
    if (!CanvasResource || !RenderTargetResource)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to get render target resources."));
        return;
    }

    TArray<FColor> PixelData;
    PixelData.SetNum(CanvasRenderTarget->SizeX * CanvasRenderTarget->SizeY);
    CanvasResource->ReadPixels(PixelData);

    RenderTargetResource->UpdateTextureRegions(0, 1, RenderTarget->GetTexture2D(), 0, 4 * CanvasRenderTarget->SizeX, 4, (uint8*)PixelData.GetData(), nullptr);

    // Release the CanvasRenderTarget2D
    CanvasRenderTarget->ConditionalBeginDestroy();

#endif
}
