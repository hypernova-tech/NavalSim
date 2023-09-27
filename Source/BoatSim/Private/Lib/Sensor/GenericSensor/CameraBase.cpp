// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/Sensor/GenericSensor/CameraBase.h"
#include <Lib/SystemManager/SystemManagerBase.h>

void ACameraBase::InitSensor()
{
	Super::InitSensor();
	pSceneCapture = GetComponentByClass<USceneCaptureComponent2D>();

	UTextureRenderTarget2D *p_render_target = pPointVisualizer->CreateRenderTarget(SensorWidth, SensorHeight, ASystemManagerBase::GetInstance()->GetUIController()->GetCameraSlotImage(SensorSlotIndex));
	pSceneCapture->TextureTarget = p_render_target;
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
