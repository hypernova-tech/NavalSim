// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/Sensor/GenericSensor/CameraBase.h"
#include <Lib/SystemManager/SystemManagerBase.h>
#include <Lib/Utils/CUtil.h>

void ACameraBase::InitSensor()
{
	Super::InitSensor();
	pSceneCapture = GetComponentByClass<USceneCaptureComponent2D>();

	UTextureRenderTarget2D* p_render_target = pPointVisualizer->CreateRenderTarget(SensorWidth, SensorHeight, ASystemManagerBase::GetInstance()->GetUIController()->GetSensorSlotImage(SensorSlotIndex));
	pSceneCapture->TextureTarget = p_render_target;
	pSceneCapture->FOVAngle = FovHorizontalDeg;

	auto pppv = ASystemManagerBase::GetInstance()->GetMainPostProcessVolume();
	pSceneCapture->PostProcessBlendWeight = 1;
	pSceneCapture->PostProcessSettings = pppv->Settings;
	//bool ret;
	//if (ASystemManagerBase::GetInstance()->GetAnnotationModeEnabled(ret)) {
	//	if (ret) {
			pSceneCapture->bAlwaysPersistRenderingState = true;
			pSceneCapture->CaptureSource = ESceneCaptureSource::SCS_FinalColorLDR;
			pSceneCapturer->SetRenderTarget(pPointVisualizer->pRenderTarget);
			

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

void ACameraBase::SetFovDeg(double fov_deg)
{
	FieldOfViewDeg = fov_deg;
	pSceneCapture->FOVAngle = fov_deg;
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
