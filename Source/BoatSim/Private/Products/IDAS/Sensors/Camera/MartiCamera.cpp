// Fill out your copyright notice in the Description page of Project Settings.


#include "Products/IDAS/Sensors/Camera/MartiCamera.h"

void AMartiCamera::BeginPlay()
{
	Super::BeginPlay();
	pSharedMemory = GetComponentByClass<USharedMemory>();
}

void AMartiCamera::OnCaptured()
{
	Super::OnCaptured();
	pSceneCapturer->ReadPixels(); //OnCaptureReady is called after read complete
}

void AMartiCamera::OnCaptureReady(void* p_data)
{
	Super::OnCaptureReady(p_data);
	TArray<FColor>* p_color_arr = (TArray<FColor> *)p_data;
	SMartiSharedMemBufferHdr hdr;
	hdr.Width = SensorWidth;
	hdr.Height = SensorHeight;
	hdr.BrightnessLevel = BrightnessLevel;
	hdr.ContrastLevel = ContrastLevel;
	hdr.IsUpdated = true;
	hdr.ImageInfo.DefogLevel = DefogLevel;
	hdr.ImageInfo.EnableDefog = IsDefogEnabled;
	hdr.ImageInfo.IsICREnabled = IsICREnabled;
	hdr.ImageInfo.IsIr = SensorType == ESensorType::CameraIR;
	hdr.ImageInfo.IsWhiteHot = IsWhiteHot;
	hdr.DataSize = sizeof(FColor) * p_color_arr->Num();
	
	//pSharedMemory->SetDataDimension(SensorWidth, SensorHeight, SensorType == ESensorType::CameraIR, IsWhiteHot, IsDefogEnabled, DefogLevel, IsICREnabled);
	pSharedMemory->SendData((INT8U*) & hdr, sizeof(SMartiSharedMemBufferHdr), (INT8U*)p_color_arr->GetData(), hdr.DataSize);

}

void AMartiCamera::InitSensor()
{
	Super::InitSensor();
	
	SSharedMemInitArgs args;
	args.Name = pSharedMemory->SharedMemoryName;
	args.size = SensorWidth * SensorHeight * 4;
	args.HeaderSize = sizeof(SMartiSharedMemBufferHdr);
	pSharedMemory->InitConnection(&args);
}

void AMartiCamera::SetSharedMemoryName(FString val)
{
	pSharedMemory->SharedMemoryName = val;
}

FString AMartiCamera::GetSharedMemoryName()
{
	return pSharedMemory->SharedMemoryName;
}
