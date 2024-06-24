// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/PointVisualizer/PointVisualizer.h"
#include <Kismet/GameplayStatics.h>
#include <CanvasItem.h>
#include <Lib/Utils/CUtil.h>
#include <Lib/Tracker/TrackerBase.h>

const FLinearColor FCommonColors::White = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);
const FLinearColor FCommonColors::Black = FLinearColor(0.0f, 0.0f, 0.0f, 1.0f);
const FLinearColor FCommonColors::Red = FLinearColor(1.0f, 0.0f, 0.0f, 1.0f);
const FLinearColor FCommonColors::Green = FLinearColor(0.0f, 1.0f, 0.0f, 1.0f);
const FLinearColor FCommonColors::Blue = FLinearColor(0.0f, 0.0f, 1.0f, 1.0f);
const FLinearColor FCommonColors::Yellow = FLinearColor(1.0f, 1.0f, 0.0f, 1.0f);
const FLinearColor FCommonColors::Cyan = FLinearColor(0.0f, 1.0f, 1.0f, 1.0f);
const FLinearColor FCommonColors::Magenta = FLinearColor(1.0f, 0.0f, 1.0f, 1.0f);

const FLinearColor FCommonColors::LightGray = FLinearColor(0.75f, 0.75f, 0.75f, 1.0f);
const FLinearColor FCommonColors::Gray = FLinearColor(0.5f, 0.5f, 0.5f, 1.0f);
const FLinearColor FCommonColors::DarkGray = FLinearColor(0.25f, 0.25f, 0.25f, 1.0f);

const FLinearColor FCommonColors::Orange = FLinearColor(1.0f, 0.5f, 0.0f, 1.0f);
const FLinearColor FCommonColors::Purple = FLinearColor(0.5f, 0.0f, 0.5f, 1.0f);
const FLinearColor FCommonColors::Pink = FLinearColor(1.0f, 0.75f, 0.8f, 1.0f);
const FLinearColor FCommonColors::Brown = FLinearColor(0.6f, 0.3f, 0.0f, 1.0f);
const FLinearColor FCommonColors::Lime = FLinearColor(0.75f, 1.0f, 0.0f, 1.0f);
const FLinearColor FCommonColors::Turquoise = FLinearColor(0.0f, 0.75f, 0.75f, 1.0f);
const FLinearColor FCommonColors::SkyBlue = FLinearColor(0.53f, 0.81f, 0.92f, 1.0f);
const FLinearColor FCommonColors::NavyBlue = FLinearColor(0.0f, 0.0f, 0.5f, 1.0f);
const FLinearColor FCommonColors::Olive = FLinearColor(0.5f, 0.5f, 0.0f, 1.0f);
const FLinearColor FCommonColors::Maroon = FLinearColor(0.5f, 0.0f, 0.0f, 1.0f);
const FLinearColor FCommonColors::Gold = FLinearColor(1.0f, 0.84f, 0.0f, 1.0f);
const FLinearColor FCommonColors::Silver = FLinearColor(0.75f, 0.75f, 0.75f, 1.0f);


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



void APointVisualizer::Visualize(SScanResult *p_scan_result, FVector origin, FVector current_forward, FVector current_right, float max_range_meter, void *p_tracker, bool reset)
{
        //SetPixelValue(pRenderTarget, 50, 50, FColor::Green);
    


        if (!pRenderTarget)
        {
            UE_LOG(LogTemp, Error, TEXT("Invalid render target."));
            return;
        }


        FRenderTarget* RenderTargetResource = pRenderTarget->GameThread_GetRenderTargetResource();

        if (RenderTargetResource == nullptr) {
            return;
        }
        RenderCount = 0;

        double one_over_max_range = 1.0 / WORLD_TO_UNREAL(max_range_meter);
        FCanvas Canvas(RenderTargetResource, nullptr, 0, 0, 0, ERHIFeatureLevel::SM5);

        // Clear the canvas with the desired color to avoid artifacts from previous frames
        //Canvas.SetAllowedModes(FCanvas::ECanvasAllowModes::Allow_Flush);
        //Canvas.SetAllowedModes(FCanvas::ECanvasAllowModes::Allow_DeleteOnRender);
        if (reset) {
            Canvas.Clear(FColor::Black);
        }
        
        FIntPoint rt_size = RenderTargetResource->GetSizeXY();

        SSectorContainer* p_sector_container = p_scan_result->GetSectorContainer();

        current_forward.Z = 0;
        current_forward.Normalize();

        current_right.Z = 0;
        current_right.Normalize();

        for (int sector = 0; sector < p_scan_result->SectorCount; sector++) {
            SSectorInfo* p_sector_info = p_sector_container->GetSector(sector);

            for (FVector &pos : p_sector_info->SectorData) {
                FVector diff = pos - origin;
                float local_forward = FVector::DotProduct(diff, current_forward);
                float local_right = FVector::DotProduct(diff, current_right);

                local_forward *= one_over_max_range;
                local_right *= one_over_max_range;

                float X = (1 + local_right) * 0.5;
                float Y = (1 - local_forward) * 0.5;

                FVector2D size(20, 20);
                auto color = FLinearColor::Red;
                color.A = 0;
                FCanvasTileItem TileItem(FVector2D(X * rt_size.X - size.X * 0.5, Y * rt_size.Y - size.Y * 0.5), size, color);

                TileItem.BlendMode = SE_BLEND_Opaque;
                Canvas.DrawItem(TileItem);
                RenderCount++;
            }


        }


        if (p_tracker != nullptr) {
            TArray< STrackedObjectInfo*>* p_track = ((CTrackerBase*)p_tracker)->GetTrackedObjects();

            for (auto p_tracked_object : *(p_track)) {
                if (p_tracked_object->pActor == nullptr) {
                    continue;
                }

                if (!p_tracked_object->IsAcquired()) {
                    continue;
                }

                FVector diff = p_tracked_object->pActor->GetActorLocation() - origin;
                float local_forward = FVector::DotProduct(diff, current_forward);
                float local_right = FVector::DotProduct(diff, current_right);

                local_forward *= one_over_max_range;
                local_right *= one_over_max_range;

                float X = (1 + local_right) * 0.5;
                float Y = (1 - local_forward) * 0.5;

                FVector2D size(15, 15);
                auto color = FCommonColors::SkyBlue;
                color.A = 0;
                FCanvasTileItem TileItem(FVector2D(X * rt_size.X - size.X * 0.5, Y * rt_size.Y - size.Y * 0.5), size, color);

                TileItem.BlendMode = SE_BLEND_Opaque;
                Canvas.DrawItem(TileItem);
                RenderCount++;
            }
        }
        /*
        FVector2D pos_text = FVector2D(100, 100);
        FCanvasTextItem TextItem(pos_text, FText::FromString(TEXT("Your Text Here")), GEngine->GetMediumFont(), FLinearColor::Red);
        Canvas.DrawItem(TextItem);
        */
        if (RenderCount > 0) {
            Canvas.Flush_GameThread();
        }
       
        
 

}

void APointVisualizer::Visualize(const TArray<FVector> &points, EPointCooordSystem coord_sys, FVector origin, FVector current_forward, FVector current_right, float max_range_meter, void* p_tracker, bool reset)
{
    //SetPixelValue(pRenderTarget, 50, 50, FColor::Green);

    if (!pRenderTarget)
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid render target."));
        return;
    }


    FRenderTarget* RenderTargetResource = pRenderTarget->GameThread_GetRenderTargetResource();

    if (RenderTargetResource == nullptr) {
        return;
    }
    RenderCount = 0;

    double one_over_max_range = 1.0 / WORLD_TO_UNREAL(max_range_meter);
    FCanvas Canvas(RenderTargetResource, nullptr, 0, 0, 0, ERHIFeatureLevel::SM5);

    // Clear the canvas with the desired color to avoid artifacts from previous frames
    //Canvas.SetAllowedModes(FCanvas::ECanvasAllowModes::Allow_Flush);
    //Canvas.SetAllowedModes(FCanvas::ECanvasAllowModes::Allow_DeleteOnRender);
    if (reset) {
        Canvas.Clear(FColor::Black);
    }

    FIntPoint rt_size = RenderTargetResource->GetSizeXY();

    current_forward.Z = 0;
    current_forward.Normalize();

    current_right.Z = 0;
    current_right.Normalize();

    for (int i = 0; i < points.Num(); i++) {
            
        const FVector &pos = points[i];

        FVector diff = pos - origin;
        float local_forward = FVector::DotProduct(diff, current_forward);
        float local_right = FVector::DotProduct(diff, current_right);

        local_forward *= one_over_max_range;
        local_right *= one_over_max_range;

        float X = (1 + local_right) * 0.5;
        float Y = (1 - local_forward) * 0.5;

        FVector2D size(20, 20);
        FCanvasTileItem TileItem(FVector2D(X * rt_size.X - size.X * 0.5, Y * rt_size.Y - size.Y * 0.5), size, FLinearColor::Red);

        TileItem.BlendMode = SE_BLEND_Translucent;
        Canvas.DrawItem(TileItem);
        RenderCount++;
    }


    if (p_tracker != nullptr) {
        TArray< STrackedObjectInfo*>* p_track = ((CTrackerBase*)p_tracker)->GetTrackedObjects();

        for (auto p_tracked_object : *(p_track)) {
            if (p_tracked_object->pActor == nullptr) {
                continue;
            }

            if (!p_tracked_object->IsAcquired()) {
                continue;
            }

            FVector diff = p_tracked_object->pActor->GetActorLocation() - origin;
            float local_forward = FVector::DotProduct(diff, current_forward);
            float local_right = FVector::DotProduct(diff, current_right);

            local_forward *= one_over_max_range;
            local_right *= one_over_max_range;

            float X = (1 + local_right) * 0.5;
            float Y = (1 - local_forward) * 0.5;

            FVector2D size(20, 20);

            FCanvasTileItem TileItem(FVector2D(X * rt_size.X - size.X * 0.5, Y * rt_size.Y - size.Y * 0.5), size, FLinearColor::Green);

            TileItem.BlendMode = SE_BLEND_Translucent;
            Canvas.DrawItem(TileItem);
            RenderCount++;
        }
    }
    /*
    FVector2D pos_text = FVector2D(100, 100);
    FCanvasTextItem TextItem(pos_text, FText::FromString(TEXT("Your Text Here")), GEngine->GetMediumFont(), FLinearColor::Red);
    Canvas.DrawItem(TextItem);
    */
    if (RenderCount > 0) {
        Canvas.Flush_GameThread();
    }




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

}

UTextureRenderTarget2D* APointVisualizer::CreateRenderTarget(int width, int height, UImage* p_image)
{

    pRenderTarget = NewObject<UTextureRenderTarget2D>(this);
    pRenderTarget->InitAutoFormat(width, height);
   
    //pRenderTarget->InitCustomFormat(width, height,EPixelFormat::PF_R8G8B8A8, true);
 
    CreateMaterialInstanceWithTexture(this, pBaseMaterial, pRenderTarget, p_image);
    return pRenderTarget;
}
UMaterialInstanceDynamic* APointVisualizer::CreateMaterialInstanceWithTexture(UObject* Outer, UMaterialInterface* BaseMaterial, UTexture* Texture, UImage* p_image)
{
    if (!BaseMaterial)
    {
        // Return null if the base material is not valid
        return nullptr;
    }

    // Create a new material instance dynamically
    UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(BaseMaterial, Outer);

    if (!DynamicMaterial)
    {
        // Return null if the dynamic material creation fails
        return nullptr;
    }

    if (!Texture)
    {
        // Return the dynamic material without setting the texture if the texture is not valid
        return DynamicMaterial;
    }

    // Set the texture parameter for the dynamic material

    FName TextureParameterName = "MainTexture"; // Change this to the actual name of the texture parameter in your material
    DynamicMaterial->SetTextureParameterValue(TextureParameterName, Texture);

    FSlateBrush NewBrush;
    NewBrush.SetResourceObject(DynamicMaterial);
    p_image->SetBrush(NewBrush);
    return DynamicMaterial;
}

void APointVisualizer::RenderPointCloud(FVector loc, const TArray<FVector>& pts)
{
    int cnt = 0;
    for (auto pt : pts) {
        if (!pt.IsZero()) {
            cnt++;
            //if (cnt < 1000) {
            CUtil::DebugBox(GetWorld(), loc + TOUE(pt), 10, FColor::Red, 0.2);
            //}
        }
    }
}