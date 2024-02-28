// Fill out your copyright notice in the Description page of Project Settings.


#include "Products/IDAS/Sensors/Camera/MartiEOSuite.h"
#include <Lib/Utils/CUtil.h>
#include "MartiCamera.h"

void AMartiEOSuite::InitSensor()
{
	Super::InitSensor();
	pMartiCommIF = (UMartiCommIF*)GetCommCommIF();
	pMartiCommIF->SetHostIf(this);
}

void AMartiEOSuite::OnPreStep(float DeltaTime)
{
	TArray<ACameraBase*> cams;

	CUtil::FindChildActorsRecursive<ACameraBase>(this, cams);
	
	for (auto cam : cams) {
		if (cam->SensorType == ESensorType::CameraVisible) {
			pDTV = cam;
		}else if (cam->SensorType == ESensorType::CameraIR) {
			pIR = cam;
		}
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

	pGimbal->EnableAxis_(rpy_en);
	pGimbal->SetCommand_(rpy_cmd);
	pGimbal->SetAxisRateDegPerSec_(rpy_rate);
	SendLosReportCommand();

}

void AMartiEOSuite::HandleSensorCommand(SMartiSensorCommandPayload* p_cmd)
{
	if (p_cmd->CamControl.ActiveVideoSelection == 1) { //ir
		ChangeActiveCamera(pIR);
	}else if (p_cmd->CamControl.ActiveVideoSelection == 2) { //DTV
		ChangeActiveCamera(pDTV);
	}
	UpdateDTVFieldOfView(p_cmd->DTVFieldOfView.Bits);
	UpdateIRFieldOfView(p_cmd->ThermalFieldOfView.Bits);

	if (p_cmd->ModeContol.ThermalPolarity == 1) {
		pActiveCamera->IsWhiteHot = true;
	}
	else {
		pActiveCamera->IsWhiteHot = false;
	}

	pDTV->ContrastLevel = p_cmd->DTVContrast;
	pDTV->BrightnessLevel = p_cmd->DTVBrightness;

	pIR->ContrastLevel = p_cmd->ThermalContrast;
	pIR->BrightnessLevel = p_cmd->ThermalBrightness;
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

	pDTV->FieldOfViewDeg = cur_fov;
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

	pIR->FieldOfViewDeg = cur_fov;
}

void AMartiEOSuite::SendLosReportCommand()
{
	SMartiLosReportPayload los_report;
	memcpy(&los_report, &LastLosCommand, sizeof(los_report));

	los_report.AzimuthRate = pGimbal->GetAxis(EGimbalAxis::Yaw)->AngleSpeedDegPerSec;
	los_report.ElavationRate = pGimbal->GetAxis(EGimbalAxis::Pitch)->AngleSpeedDegPerSec;

	los_report.AzimuthPosition = pGimbal->GetAxisAngleDeg(EGimbalAxis::Yaw);
	los_report.ElevationPosition = pGimbal->GetAxisAngleDeg(EGimbalAxis::Pitch);
	pMartiCommIF->SendMessage(EMartiCommandReportId::LosReport, (INT8U*)&los_report, sizeof(SMartiLosReportPayload));
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

	pMartiCommIF->SendMessage(EMartiCommandReportId::SensorReport, (INT8U*) & report, sizeof(SMartiSensorReportPayload));
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
	
	pMartiCommIF->SendMessage(EMartiCommandReportId::DTVDefogReport, (INT8U*)&status, sizeof(SDTVDefogStatus));
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

	pMartiCommIF->SendMessage(EMartiCommandReportId::DTVICRReport, (INT8U*)&icr, sizeof(SDTVICRReportPayload));
}