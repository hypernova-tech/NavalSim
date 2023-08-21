// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/ScreenDepth/SceneCapturer.h"
#include "RenderingThread.h"
#include "RHI.h"
#include "Async/AsyncWork.h"
#include "Async/TaskGraphInterfaces.h"
#include <Lib/Sensor/SensorBase.h>
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