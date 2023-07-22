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
