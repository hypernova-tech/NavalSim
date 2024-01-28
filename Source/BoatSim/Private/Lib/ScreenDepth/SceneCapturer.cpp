// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/ScreenDepth/SceneCapturer.h"
#include "RenderingThread.h"
#include "RHI.h"
#include "Async/AsyncWork.h"
#include "Async/TaskGraphInterfaces.h"
#include <Lib/Sensor/SensorBase.h>
#include <ImageUtils.h>
#include <IImageWrapper.h>
#include "IImageWrapperModule.h"
#include <Kismet/KismetRenderingLibrary.h>
// Sets default values
ASceneCapturer::ASceneCapturer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASceneCapturer::BeginPlay()
{
	Super::BeginPlay();
	pSceneCapture = GetComponentByClass< USceneCaptureComponent2D>();


}

// Called every frame
void ASceneCapturer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASceneCapturer::CreateRenderTexture(void *owner, INT32U width_px, INT32U height_px, EPixelFormat pixel_format)
{
	pOwner = owner;
    pRenderTarget = NewObject<UTextureRenderTarget2D>(this);
    pRenderTarget->InitAutoFormat(width_px, width_px);
    pRenderTarget->OverrideFormat = (pixel_format);
    pRenderTarget->UpdateResource();

    pSceneCapture->TextureTarget = pRenderTarget;

    if (pRenderTarget)
    {
       pRenderTargetResource = pRenderTarget->GameThread_GetRenderTargetResource();

       
       OutBMP.Reserve(pRenderTarget->SizeX * pRenderTarget->SizeY);
     
    }
}

void ASceneCapturer::Capture()
{
	pSceneCapture->CaptureScene();

    
}

void ASceneCapturer::ReadPixels()
{
	StartReadPixelsAsync();
	return;
    pRenderTargetResource = pRenderTarget->GameThread_GetRenderTargetResource();

    if (pRenderTargetResource != nullptr) {
        auto is_init = pRenderTargetResource->IsInitialized();

        FReadSurfaceDataFlags ReadPixelFlags;
        ReadPixelFlags.SetLinearToGamma(false); // This depends on what you want to do with the data. Usually, set to false if you're sending this data to the GPU.
        pRenderTargetResource->ReadPixels(OutBMP, ReadPixelFlags);
    }
   
}
void ASceneCapturer::StartReadPixelsAsync()
{
	if (pRenderTarget && pRenderTarget->Resource)
	{
		pRenderTargetResource = pRenderTarget->GameThread_GetRenderTargetResource();
		FTexture2DRHIRef Texture2DRHI = pRenderTargetResource->GetTexture2DRHI();
		if (Texture2DRHI.IsValid())
		{
			ENQUEUE_RENDER_COMMAND(ReadPixelsCommand)(
				[this, Texture2DRHI](FRHICommandListImmediate& RHICmdList)
				{
					FIntRect Rect(0, 0, Texture2DRHI->GetSizeX(), Texture2DRHI->GetSizeY());
					RHICmdList.ReadSurfaceData(Texture2DRHI, Rect, OutBMP, FReadSurfaceDataFlags());
					
					// Schedule a game-thread task for when the readback is complete
					FFunctionGraphTask::CreateAndDispatchWhenReady(

						[this]()
						{
							OnReadbackComplete();
						},
						TStatId(), nullptr, ENamedThreads::GameThread
					);
					
				}
			);
		}
	}
}

void ASceneCapturer::OnReadbackComplete()
{
	ASensorBase* p_sensor = (ASensorBase*)pOwner;
	
	p_sensor->OnCaptureReady((void*)&OutBMP);
}

void ASceneCapturer::SaveAsPNG(UTextureRenderTarget2D* RenderTarget, FString image_name)
{
    if (!RenderTarget || !RenderTarget->Resource)
    {
        UE_LOG(LogTemp, Warning, TEXT("RenderTarget is null or has no resource."));
        return;
    }
    //ExportAsPNG(RenderTarget, image_name);

    FTextureRenderTargetResource* RenderTargetResource = pRenderTarget->GameThread_GetRenderTargetResource();
    // Enqueue command to capture the image on the render thread

            
            if (!RenderTargetResource)
            {
                UE_LOG(LogTemp, Warning, TEXT("RenderTargetResource is null."));
                return;
            }

            
            TArray<FColor> Bitmap;
            FReadSurfaceDataFlags ReadPixelFlags;
            ReadPixelFlags.SetLinearToGamma(true);
           

            // Read pixels from the render target resource
            RenderTargetResource->ReadPixels(Bitmap, ReadPixelFlags);

#if true
            for(int i = 0; i<Bitmap.Num(); i++){
                FColor &color = Bitmap[i];
                color.A = 255;
                
            }
#endif
            
            // Pass the data to the game thread to save it
         
            // Convert to PNG
            IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
            TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG);

            if (!ImageWrapper.IsValid())
            {
                UE_LOG(LogTemp, Warning, TEXT("Failed to create Image Wrapper."));
                return;
            }

            ImageWrapper->SetRaw(Bitmap.GetData(), Bitmap.GetAllocatedSize(), pRenderTarget->SizeX, pRenderTarget->SizeY, ERGBFormat::BGRA, 8);
            const TArray64<uint8>& PNGData = ImageWrapper->GetCompressed(100);

            if (PNGData.Num() == 0)
            {
                UE_LOG(LogTemp, Warning, TEXT("Failed to compress image data."));
                return;
            }

            // Save to file
            FString SavePath = FPaths::ProjectSavedDir() / TEXT("SavedImages/") + image_name;
            if (!FFileHelper::SaveArrayToFile(PNGData, *SavePath))
            {
                UE_LOG(LogTemp, Warning, TEXT("Failed to save PNG to %s"), *SavePath);
                return;
            }

            UE_LOG(LogTemp, Log, TEXT("Saved PNG to %s"), *SavePath);
            
       

}

void ASceneCapturer::SetRenderTarget(UTextureRenderTarget2D* p_tgt)
{
    pRenderTarget = p_tgt;
    auto curr_format = pRenderTarget->GetFormat();
    pRenderTarget->OverrideFormat = (EPixelFormat::PF_B8G8R8A8);
    pRenderTarget->UpdateResource();
    pRenderTarget->ClearColor = FLinearColor(0, 0, 0, 1);
    pRenderTargetResource = pRenderTarget->GameThread_GetRenderTargetResource();
    auto RenderTargetResource = pRenderTargetResource;

    UKismetRenderingLibrary::ClearRenderTarget2D(this, pRenderTarget, FLinearColor(0,0,0,1));
}
void ASceneCapturer:: ExportAsPNG(UTextureRenderTarget2D* RenderTarget, FString file_name)
{
    


    pRenderTargetResource = pRenderTarget->GameThread_GetRenderTargetResource();
    if (RenderTarget)
    {
        // File path where you want to save the PNG
        FString FilePath = FPaths::ProjectDir() / TEXT("SavedImages/"+ file_name);
        //FScreenshotRequest::RequestScreenshot(FilePath, false, false, false);
        //return;
        // Create file archive to save the PNG
        FArchive* FileAr = IFileManager::Get().CreateFileWriter(*FilePath);
        if (FileAr)
        {
            // Export the render target as PNG
            FImage im;
            FImageUtils::GetRenderTargetImage(RenderTarget, im);
            bool bSuccess = FImageUtils:: ExportRenderTarget2DAsPNG(RenderTarget, *FileAr);
           
            // Close the file archive
            FileAr->Close();
            delete FileAr;

            if (bSuccess)
            {
                UE_LOG(LogTemp, Log, TEXT("Render target exported successfully to %s"), *FilePath);
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Failed to export render target."));
            }
        }
    }
}