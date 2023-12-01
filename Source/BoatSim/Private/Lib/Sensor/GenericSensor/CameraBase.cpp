// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/Sensor/GenericSensor/CameraBase.h"
#include <Lib/SystemManager/SystemManagerBase.h>

void ACameraBase::InitSensor()
{
	Super::InitSensor();
	pSceneCapture = GetComponentByClass<USceneCaptureComponent2D>();

	UTextureRenderTarget2D *p_render_target = pPointVisualizer->CreateRenderTarget(SensorWidth, SensorHeight, ASystemManagerBase::GetInstance()->GetUIController()->GetSensorSlotImage(SensorSlotIndex));
	pSceneCapture->TextureTarget = p_render_target;

	pSceneCapture->FOVAngle = FovHorizontalDeg;
	
}

void ACameraBase::ResumeSensor()
{
	Super::ResumeSensor();
	if (pSceneCapture != nullptr) {
		pSceneCapture->Activate();
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
