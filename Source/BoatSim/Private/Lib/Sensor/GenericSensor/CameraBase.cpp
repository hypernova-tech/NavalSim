// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/Sensor/GenericSensor/CameraBase.h"
#include <Lib/SystemManager/SystemManagerBase.h>
#include <Lib/Utils/CUtil.h>

void ACameraBase::BeginPlay()
{
	Super::BeginPlay();
}

void ACameraBase::InitSensor()
{
	Super::InitSensor();
	pSceneCapture = GetComponentByClass<USceneCaptureComponent2D>();

	UTextureRenderTarget2D* p_render_target = pPointVisualizer->CreateRenderTarget(SensorWidth, SensorHeight, ASystemManagerBase::GetInstance()->GetUIController()->GetSensorSlotImage(SensorSlotIndex));
	pSceneCapture->TextureTarget = p_render_target;
	pSceneCapture->FOVAngle = FovHorizontalDeg;
	FieldOfViewDeg = pSceneCapture->FOVAngle;

	auto pppv = ASystemManagerBase::GetInstance()->GetMainPostProcessVolume();
	pSceneCapture->PostProcessBlendWeight = 1;
	pSceneCapture->PostProcessSettings = pppv->Settings;
	//bool ret;
	//if (ASystemManagerBase::GetInstance()->GetAnnotationModeEnabled(ret)) {
	//	if (ret) {
			pSceneCapture->bAlwaysPersistRenderingState = true;
			pSceneCapture->CaptureSource = ESceneCaptureSource::SCS_FinalColorLDR;
			pSceneCapturer->SetRenderTarget(pPointVisualizer->pRenderTarget);
			pSceneCapturer->pMaster = this;

	//	}
		
	//}

}

void ACameraBase::ResumeSensor()
{
	Super::ResumeSensor();
	if (pSceneCapture != nullptr) {
		pSceneCapture->Activate();
	}

}

void ACameraBase::Run(float delta_time_sec)
{
	Super::Run(delta_time_sec);
	auto pSys = ASystemManagerBase::GetInstance();
	if (pSys->GetIsSimulationMode()) {
	
		if(pSys->GetAnnotationModeEnabled()){
			
			CaptureScreen();
			
		}
		else {
			pSceneCapturer->Capture();
			OnCaptured();
		}
	}


}
void ACameraBase::CaptureScreen()
{
	auto pSys = ASystemManagerBase::GetInstance();
	bool save = pSys->CanSaveAnnotationFrame();


	if (save) {
		auto frame_info = pSys->GetAnnotionFrameInfo();
		FString name = GetName() + frame_info + ".png";
		pSceneCapturer->Capture();
		pSceneCapturer->SaveAsPNG(pSceneCapture->TextureTarget, name);

	}

}


void ACameraBase::PauseSensor()
{
	Super::PauseSensor();
	if (pSceneCapture != nullptr) {
		pSceneCapture->Deactivate();
	}
}

void ACameraBase::OnCaptured()
{
}

void ACameraBase::OnCaptureReady(void* p_data)
{

}

void ACameraBase::SetFovDeg(double fov_deg)
{
	FieldOfViewDeg = fov_deg;
	if (pSceneCapture != nullptr) {
		pSceneCapture->FOVAngle = fov_deg;
	}
}
void ACameraBase::UpdateFov()
{
	if (pSceneCapture != nullptr && FieldOfViewDeg != 0) {
		pSceneCapture->FOVAngle = FieldOfViewDeg;
	}
	
}
void ACameraBase::Save(ISaveLoader* p_save_loader)
{
	Super::Save(p_save_loader);

	FString line;

	line = p_save_loader->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
	p_save_loader->AppendOption(line, CCLICommandManager::CamWidthPx, SensorWidth);
	p_save_loader->AddLine(line);

	line = p_save_loader->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
	p_save_loader->AppendOption(line, CCLICommandManager::CamHeightPx, SensorHeight);
	p_save_loader->AddLine(line);
}

void ACameraBase::SaveJSON(CJsonDataContainer& data)
{
	Super::SaveJSON(data);
	data.Add(CCLICommandManager::CamWidthPx, SensorWidth);
	data.Add(CCLICommandManager::CamHeightPx, SensorHeight);
}
