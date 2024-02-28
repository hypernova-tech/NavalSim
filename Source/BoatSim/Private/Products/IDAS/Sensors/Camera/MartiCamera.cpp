// Fill out your copyright notice in the Description page of Project Settings.


#include "Products/IDAS/Sensors/Camera/MartiCamera.h"

void AMartiCamera::OnCaptured()
{
	Super::OnCaptured();
	pSceneCapturer->ReadPixels(); //OnCaptureReady is called after read complete
}

void AMartiCamera::OnCaptureReady(void* p_data)
{
	Super::OnCaptureReady(p_data);
	TArray<FColor>* p_color_arr = (TArray<FColor> *)p_data;
	pSharedMemory->SetDataDimension(SensorWidth, SensorHeight, SensorType == ESensorType::CameraIR, IsWhiteHot, IsDefogEnabled, DefogLevel, IsICREnabled);
	pSharedMemory->SendData((INT8U*)p_color_arr->GetData(), sizeof(FColor) * p_color_arr->Num());

}

void AMartiCamera::InitSensor()
{
	Super::InitSensor();
	pSharedMemory  = GetComponentByClass<USharedMemory>();
	SSharedMemInitArgs args;
	args.Name = pSharedMemory->SharedMemoryName;
	args.size = SensorWidth * SensorHeight * 4;
	pSharedMemory->InitConnection(&args);
}
