// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/Sensor/GenericSensor/CameraBase.h"
#include <Lib/SystemManager/SystemManagerBase.h>
#include <Lib/Utils/CUtil.h>
#include <Lib/Sensor/GenericCamCommProtocolIF/GenericCamProtocolIF.h>
#include <Kismet/KismetRenderingLibrary.h>
#include <Lib/Math/CMath.h>

void ACameraBase::BeginPlay()
{
	Super::BeginPlay();
}

void ACameraBase::InitSensor()
{
	Super::InitSensor();
	pSceneCaptureComp2D = GetComponentByClass<USceneCaptureComponent2D>();

	UTextureRenderTarget2D* p_render_target = pPointVisualizer->CreateRenderTarget(SensorWidth, SensorHeight, ASystemManagerBase::GetInstance()->GetUIController()->GetSensorSlotImage(SensorSlotIndex));

	pSceneCaptureComp2D->TextureTarget = p_render_target;
	pSceneCaptureComp2D->FOVAngle = FovHorizontalDeg;
	FieldOfViewDeg = pSceneCaptureComp2D->FOVAngle;



	if (PostProcessMaterialBase)
	{
		PostProcessMaterialInstance = UMaterialInstanceDynamic::Create(PostProcessMaterialBase, this);
		FPostProcessSettings& PostProcessSettingsA = pSceneCaptureComp2D->PostProcessSettings;
		pSceneCaptureComp2D->PostProcessSettings.WeightedBlendables.Array.Empty();
		if (PostProcessMaterialInstance)
		{
			pSceneCaptureComp2D->PostProcessSettings.AddBlendable(PostProcessMaterialInstance, 1.0f);
		}
		//PostProcessMaterialInstance->SetTextureParameterValue(FName("RenderTargetTexture"), pSceneCaptureComp2D->TextureTarget);
	}
	else {
		auto pppv = ASystemManagerBase::GetInstance()->GetMainPostProcessVolume();
		pSceneCaptureComp2D->PostProcessBlendWeight = 1;
		pSceneCaptureComp2D->PostProcessSettings = pppv->Settings;
	}



	//bool ret;
	//if (ASystemManagerBase::GetInstance()->GetAnnotationModeEnabled(ret)) {
	//	if (ret) {

	pSceneCaptureComp2D->TextureTarget->TargetGamma = SceneCaptureTextureTargetGamma;
	pSceneCaptureComp2D->bAlwaysPersistRenderingState = true;
	pSceneCaptureComp2D->CaptureSource =  ESceneCaptureSource::SCS_FinalColorLDR;
	pSceneCaptureComp2D->TextureTarget = pPointVisualizer->pRenderTarget;
	pSceneCapturer->SetRenderTarget(pPointVisualizer->pRenderTarget);
	pSceneCapturer->pSceneCaptureComp2D = pSceneCaptureComp2D;
	pSceneCapturer->pMaster = this;

	SSharedMemInitArgs args;
	args.Name = ProtocolConverterSharedMemoryName;
	args.HeaderSize = sizeof(SGenericCamSharedMemBufferHdr);
	args.size = SensorWidth * SensorHeight * sizeof(FColor);
	pCommIF->Init(&args, sizeof(SSharedMemInitArgs));



	


}

void ACameraBase::ResumeSensor()
{
	Super::ResumeSensor();
	if (pSceneCaptureComp2D != nullptr) {
		pSceneCaptureComp2D->Activate();
	}

}

void ACameraBase::Run(float delta_time_sec)
{
	Super::Run(delta_time_sec);
	auto pSys = ASystemManagerBase::GetInstance();
	if (pSys->GetIsSimulationMode()) {

		if (pSys->GetAnnotationModeEnabled()) {

			CaptureScreen();

		}
		else {
			if ((FApp::GetCurrentTime() - LastCaptureTimeSec) >= 1.0 / Frequency){
				pSceneCapturer->Capture();
				OnCaptured();
				LastCaptureTimeSec = FApp::GetCurrentTime();
			}
		
		}
	}
	ApplyCamPostProcessEffects();

}
void ACameraBase::CaptureScreen()
{
	auto pSys = ASystemManagerBase::GetInstance();
	bool save = pSys->CanSaveAnnotationFrame();


	if (save) {
		auto frame_info = pSys->GetAnnotionFrameInfo();
		FString name = GetName() + frame_info + ".png";
		pSceneCapturer->Capture();
		pSceneCapturer->SaveAsPNG(pSceneCaptureComp2D->TextureTarget, name);

	}

}


void ACameraBase::PauseSensor()
{
	Super::PauseSensor();
	if (pSceneCaptureComp2D != nullptr) {
		pSceneCaptureComp2D->Deactivate();
	}
}

void ACameraBase::OnCaptured()
{
	if (GenericSensorOutputEnabled) {
		if (ApplyMetarialToPostProcess) {
			UKismetRenderingLibrary::DrawMaterialToRenderTarget(this, pSceneCaptureComp2D->TextureTarget, PostProcessMaterialInstance);
		}
		

		pSceneCapturer->ReadPixels(); //OnCaptureReady is called after read complete
		
	}
	
}

void ACameraBase::OnCaptureReady(void* p_data)
{
	TArray<FColor>* p_color_arr = (TArray<FColor> *)p_data;

	SGenericCamSharedMemBufferHdr hdr;
	hdr.Width = SensorWidth;
	hdr.Height = SensorHeight;
	hdr.DataSize = sizeof(FColor) * p_color_arr->Num();
	hdr.IsUpdated = true;

	//pSharedMemory->SetDataDimension(SensorWidth, SensorHeight, SensorType == ESensorType::CameraIR, IsWhiteHot, IsDefogEnabled, DefogLevel, IsICREnabled);
	pCommIF->SendData((INT8U*)&hdr, sizeof(SGenericCamSharedMemBufferHdr), (INT8U*)p_color_arr->GetData(), hdr.DataSize);
	
}

void ACameraBase::ApplyCamPostProcessEffects()
{

	FPostProcessSettings& PostProcessSettingsA = pSceneCaptureComp2D->PostProcessSettings;
	PostProcessSettingsA.FilmGrainIntensity = MeasurementErrorMean;
	

	PostProcessSettingsA.DepthOfFieldFocalDistance = BlurPercent==0?0:CMath::Remap(BlurPercent, 0, 100, 20, 1);

	if (PostProcessMaterialInstance) {
		PostProcessMaterialInstance->SetScalarParameterValue("LensDistortionIntensity", LensDistortionPercent * 0.01);
	}
}

void ACameraBase::SetFovDeg(double fov_deg)
{
	FieldOfViewDeg = fov_deg;
	if (pSceneCaptureComp2D != nullptr) {
		pSceneCaptureComp2D->FOVAngle = fov_deg;
	}
}
void ACameraBase::UpdateFov()
{
	if (pSceneCaptureComp2D != nullptr && FieldOfViewDeg != 0) {
		pSceneCaptureComp2D->FOVAngle = FieldOfViewDeg;
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
