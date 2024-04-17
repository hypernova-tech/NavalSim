// Fill out your copyright notice in the Description page of Project Settings.


#include "Products/IDAS/Sensors/Camera/MartiEOSuite.h"
#include <Lib/Utils/CUtil.h>
#include "MartiCamera.h"
#include "Windows/MinWindows.h" // Include this header for Windows-specific functions


typedef int(*EOCreateInstance)(char* sm_sname, char* listner_ip_addr, int listener_port, int width, int height, int frame_rate);


void AMartiEOSuite::BeginPlay()
{
	Super::BeginPlay();
}

void AMartiEOSuite::InitSensor()
{
	Super::InitSensor();
	pMartiCommIF = (UMartiCommIF*)GetCommCommIF();
	pMartiCommIF->SetHostIf(this);



	FString BinariesDir = FPaths::ProjectDir() + TEXT("Binaries/Win64/Marti/");

	FString DLLName = TEXT("GStream.dll");

	// Construct the full path to the DLL
	FString DLLPath = BinariesDir + DLLName;

	// Load the DLL
	HINSTANCE inst;
	inst = LoadLibrary(*DLLPath);
	if (inst != nullptr)
	{
		// Get function pointer
		EOCreateInstance func_ptr = (EOCreateInstance)GetProcAddress(inst, "CreateInstance");
		if (func_ptr != nullptr)
		{
			char sm_name[1024];
			CUtil::FStringToAsciiChar(ProtocolConverterSharedMemoryName, sm_name, 1024);

			auto conns = pMartiCommIF->GetConnectionsInfo();
			char ip_addr[256];
			CUtil::FStringToAsciiChar(GStreamerIP, ip_addr, 256);
			// Call function
			if (GStreamerPort != 0) {
				StreamerInstanceId = func_ptr(sm_name, ip_addr, GStreamerPort, pActiveCamera->SensorWidth, pActiveCamera->SensorHeight, 60);

			}
		}
		hDLL = inst;

		// Unload DLL (optional)
		//FreeLibrary(hDLL);
	}


}

void AMartiEOSuite::OnPreStep(float DeltaTime)
{
	Super::OnPreStep(DeltaTime);

	TArray<ACameraBase*> cams;

	CUtil::FindChildActorsRecursive<ACameraBase>(this, cams);
	
	for (auto cam : cams) {
		if (cam->SensorType == ESensorType::CameraVisible) {
			pDTV = cam;
		}else if (cam->SensorType == ESensorType::CameraIR) {
			pIR = cam;
		}
		AMartiCamera* p_marti_cam = (AMartiCamera*)cam;
		p_marti_cam->SetSharedMemoryName(ProtocolConverterSharedMemoryName);
	}

	pActiveCamera = pDTV;
	if (pIR != nullptr) {
		pIR->SetSuppressed(true);
	}
	TArray<AGimbalBase*> gimbals;

	CUtil::FindChildActorsRecursive<AGimbalBase>(this, gimbals);
	if (gimbals.Num() > 0) {
		pGimbal = gimbals[0];
	}	
	
}

void AMartiEOSuite::OnStep(float DeltaTime)
{
	Super::OnStep(DeltaTime);
}

void AMartiEOSuite::OnRecievedMessage(SMartiGenericMessage* p_commands)
{
	EMartiCommandReportId id = (EMartiCommandReportId)p_commands->GetCommandReportId();

	switch (id) {
	case EMartiCommandReportId::LosCommand:
		LastLosCommand = *(SMartiLosCommandPayload*)p_commands->GetPayload();
		HandleLosCommand(&LastLosCommand);
		break;

	case EMartiCommandReportId::SensorCommand:
	
		LastSensorCommandPayload = *(SMartiSensorCommandPayload*)p_commands->GetPayload();
		HandleSensorCommand(&LastSensorCommandPayload);

		break;

	case EMartiCommandReportId::DTVDefogCommand:
		LastDTVDefogCommandPayload = *(SDTVDefogCommandPayload*)p_commands->GetPayload();
		HandleDefogCommand(&LastDTVDefogCommandPayload);
		break;

	case EMartiCommandReportId::DTVICRCommand:
		LastDTVICRCommandPayload = *(SDTVICRCommandPayload*)p_commands->GetPayload();
		HandleICRCommand(&LastDTVICRCommandPayload);
		break;
	}
}

void AMartiEOSuite::HandleLosCommand(SMartiLosCommandPayload* p_cmd)
{
	FVector rpy_en		= FVector::ZeroVector;
	FVector rpy_cmd		= FVector::ZeroVector;
	FVector rpy_rate	= FVector::ZeroVector;

	ELosModOfOperation mode_op = (ELosModOfOperation)p_cmd->LosModeOfOperation;
	bool is_forward = false;
	bool is_slave_rate = false;
	bool is_slave_position = false;
	bool is_chicken_head = false;
	bool is_idle = false;
	bool is_stabilization = false;
	bool is_park = false;
	bool is_track = false;
	bool is_scan = false;
	bool is_drift_comp = false;

	switch (mode_op) {
	case LosModOfOperationNoChange:

		break;
	case LosModOfOperationPark:// + aşağı baktırıyor
		is_park = true;
		break;
	case LosModOfOperationSleveRate:// +dön
		is_slave_rate = true;
		break;
	case LosModOfOperationSlevePosition:// + bak
		is_slave_position = true;
		break;
	case LosModOfOperationForward: // + kullanıyor,sıfırda kalıyor
		is_forward = true;
		break;  
	case LosModOfOperationIdle:
		is_idle = false;
		break;
	case LosModOfOperationPoint:
		break;
	case LosModOfOperationCuedPoint:
		break;
	case LosModOfOperationScan:
		is_scan = true;
		break;
	case LosModOfOperationTargetTrack:
		is_track = true;
		break;
	case LosModOfOperationDriftCompensation:// motor drift compensation offset girme
		is_drift_comp = false;
		break;
	case LosModOfOperationStow:
		break;
	case LosModOfOperationReserved:
		break;
	case LosModOfOperationStabilizasyon:
		is_stabilization = false;
		break;// ?
	case LosModOfOperationChickenHead:// aynı noktaya bakırım
		is_chicken_head = true;
		break; 

	}

	if (p_cmd->CommandValidty.AzimuthPositionCommandValid) {
		rpy_en.Z = 1;
		rpy_cmd.Z = p_cmd->AzimuthPositionCommand;
	}
	
	if (p_cmd->CommandValidty.AzimuthRateCommandValid) {
		rpy_en.Z = 1;
		rpy_rate.Z = p_cmd->AzimuthRateCommand;
	}
	
	if (p_cmd->CommandValidty.ElavationPositionCommandValid) {
		rpy_en.Y = 1;
		rpy_cmd.Y = p_cmd->ElevationPositionCommand;
	}
	
	if (p_cmd->CommandValidty.ElavationRateCommandValid) {
		rpy_en.Y = 1;
		rpy_rate.Y = p_cmd->ElavationRateCommand;
	}

	if (is_park) {
		rpy_cmd.X = 0;
		rpy_cmd.Y = 45;
		rpy_cmd.Z = 0;
		pGimbal->SetGimbalControlMode(EGimbalControlMode::PositionWithoutRate);
	}
	else if (is_forward) {
		rpy_cmd.X = 0;
		rpy_cmd.Y = 0;
		rpy_cmd.Z = 0;
		pGimbal->SetGimbalControlMode(EGimbalControlMode::PositionWithoutRate);
	}
	else if (is_slave_rate) {
		pGimbal->SetGimbalControlMode(EGimbalControlMode::OnlyRate);
	}
	else if (is_slave_position) {
		pGimbal->SetGimbalControlMode(EGimbalControlMode::PositionWithoutRate);
	}
	else if (is_idle) {
		rpy_en = (FVector::ZeroVector);
	}
	else if (is_chicken_head) {

	}



	pGimbal->EnableAxis_(rpy_en);
	pGimbal->SetCommand_(rpy_cmd);
	pGimbal->SetAxisRateDegPerSec_(rpy_rate);
	SendLosReportCommand();

}

void AMartiEOSuite::HandleSensorCommand(SMartiSensorCommandPayload* p_cmd)
{
	if (p_cmd->CamControl1.ActiveVideoSelection == 1) { //ir
		ChangeActiveCamera(pIR);
	}else if (p_cmd->CamControl1.ActiveVideoSelection == 2) { //DTV
		ChangeActiveCamera(pDTV);
	}
	UpdateDTVFieldOfView(p_cmd->DTVFieldOfView.Bits);
	UpdateIRFieldOfView(p_cmd->ThermalFieldOfView.Bits);

	if (p_cmd->ModeContol.ThermalPolarity == 1) {
		pActiveCamera->IsWhiteHot = true;
	}
	else if (p_cmd->ModeContol.ThermalPolarity == 2) {
		pActiveCamera->IsWhiteHot = false;
	}
	else {
		// no change
	}
	if (p_cmd->DTVContrast <= 100) {
		pDTV->ContrastLevel = FMath::Clamp(p_cmd->DTVContrast, 0, 100);
	}
	if (pDTV->BrightnessLevel <= 100) {
		pDTV->BrightnessLevel = FMath::Clamp(p_cmd->DTVBrightness, 0, 100);
	}
	
	if (pIR->ContrastLevel <= 100) {
		pIR->ContrastLevel = FMath::Clamp(p_cmd->ThermalContrast, 0, 100);
	}
	
	if (pIR->BrightnessLevel <= 100) {
		pIR->BrightnessLevel = FMath::Clamp(p_cmd->ThermalBrightness, 0, 100);
	}
	

	pActiveCamera->UpdateFov();
}

void AMartiEOSuite::HandleDefogCommand(SDTVDefogCommandPayload* p_cmd)
{
	bool en =	p_cmd->DTVDefogCommand.Bits == EDefogCommand::SetDefogLevel1 ||
				p_cmd->DTVDefogCommand.Bits == EDefogCommand::SetDefogLevel2 ||
				p_cmd->DTVDefogCommand.Bits == EDefogCommand::SetDefogLevel3;

		pDTV->IsDefogEnabled = en;
		pDTV->DefogLevel = (int)p_cmd->DTVDefogCommand.Bits;
		
		SendDefogReport();
}

void AMartiEOSuite::HandleICRCommand(SDTVICRCommandPayload* p_cmd)
{
	pDTV->IsICREnabled = p_cmd->DTVICRCommand == 1;
	SendICRReport();
}



void AMartiEOSuite::ChangeActiveCamera(ACameraBase* p_req)
{
	if (pActiveCamera != p_req) {
		pActiveCamera->SetSuppressed(true);
		pActiveCamera = p_req;
		pActiveCamera->SetSuppressed(false);
	}
}

void AMartiEOSuite::UpdateDTVFieldOfView(EDTVFieldOfView fov_cmd)
{
	FLOAT64 cur_fov = pDTV->FieldOfViewDeg;

	switch (fov_cmd)
	{
	case DTVFieldOfViewNoChange:
		break;
	case DTVFieldOfViewNarrow:
		cur_fov = 10;
		break;
	case DTVFieldOfViewMiddle:
		cur_fov = 60;
		break;
	case DTVFieldOfViewWide:
		cur_fov = 90;
		break;
	case DTVFieldOfViewSuperWide:
		cur_fov = 135;
		break;
	default:
		break;
	}

	pDTV->SetFovDeg(cur_fov);
}

void AMartiEOSuite::UpdateIRFieldOfView(EThermalFieldOfView fov_cmd)
{
	FLOAT64 cur_fov = pIR->FieldOfViewDeg;

	switch (fov_cmd)
	{
	case EThermalFieldOfView::ThermalFieldOfViewNoChange:
		break;
	case EThermalFieldOfView::ThermalFieldOfViewNarrow:
		cur_fov = 10;
		break;
	case EThermalFieldOfView::ThermalFieldOfViewMiddle:
		cur_fov = 60;
		break;
	case EThermalFieldOfView::ThermalFieldOfViewWide:
		cur_fov = 90;
		break;

	default:
		break;
	}

	pIR->SetFovDeg(cur_fov);
}

void AMartiEOSuite::SendLosReportCommand()
{
	SMartiLosReportPayload los_report;
	memcpy(&los_report, &LastLosCommand, sizeof(los_report));

	los_report.AzimuthRate = pGimbal->GetAxis(EGimbalAxis::Yaw)->AngleSpeedDegPerSec;
	los_report.ElavationRate = pGimbal->GetAxis(EGimbalAxis::Pitch)->AngleSpeedDegPerSec;

	los_report.AzimuthPosition = pGimbal->GetAxisAngleDeg(EGimbalAxis::Yaw);
	los_report.ElevationPosition = pGimbal->GetAxisAngleDeg(EGimbalAxis::Pitch);
	pMartiCommIF->SendMessageWithId(EMartiCommandReportId::LosReport, (INT8U*)&los_report, sizeof(SMartiLosReportPayload));
}

void AMartiEOSuite::SendSensorReport()
{
	SMartiSensorReportPayload report;
	memset(&report, 0, sizeof(SMartiSensorReportPayload));

	report.DTVFieldOfView.Val = LastSensorCommandPayload.DTVFieldOfView.Command;
	report.ThermalFieldOfView.Val = LastSensorCommandPayload.ThermalFieldOfView.Command;

	report.Video.ActiveVideoStatus = pActiveCamera == pIR ? 1 : 2;

	report.ModeContolReturn.ThermalPolarity = pActiveCamera->IsWhiteHot ? 1 : 2;

	report.DTVBrightness = pDTV->BrightnessLevel;
	report.DTVContrast = pDTV->ContrastLevel;

	report.ThermalBrightness = pIR->BrightnessLevel;
	report.ThermalContrast = pIR->ContrastLevel;

	pMartiCommIF->SendMessageWithId(EMartiCommandReportId::SensorReport, (INT8U*) & report, sizeof(SMartiSensorReportPayload));
}

void AMartiEOSuite::SendDefogReport()
{
	SDTVDefogStatus status;
	memset(&status, 0, sizeof(SDTVDefogStatus));
	
	if (!pActiveCamera->IsDefogEnabled) {
		status.DTVDefogStatus.Bits = EDefogStatus::DefogOff;
	}
	else {
		status.DTVDefogStatus.Bits = (EDefogStatus)pActiveCamera->DefogLevel;
	}
	
	pMartiCommIF->SendMessageWithId(EMartiCommandReportId::DTVDefogReport, (INT8U*)&status, sizeof(SDTVDefogStatus));
}
void AMartiEOSuite::SendICRReport()
{
	SDTVICRReportPayload icr;
	memset(&icr, 0, sizeof(SDTVICRReportPayload));

	if (!pActiveCamera->IsICREnabled) {
		icr.DTVICRStatus = 1;
	}
	else {
		icr.DTVICRStatus = 2;
	}

	pMartiCommIF->SendMessageWithId(EMartiCommandReportId::DTVICRReport, (INT8U*)&icr, sizeof(SDTVICRReportPayload));
}