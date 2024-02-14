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
	auto cams = CUtil::FindChildActors<AMartiCamera>(this);

	for (auto cam : cams) {
		if (cam->SensorType == ESensorType::CameraVisible) {
			pDTV = cam;
		}else if (cam->SensorType == ESensorType::CameraIR) {
			pIR = cam;
		}
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
		LastLosCommandReport = *(SMartiLosCommandPayload*)p_commands->GetPayload();
		break;

	case EMartiCommandReportId::SensorCommand:
		break;

	case EMartiCommandReportId::DTVDefogCommand:
		break;

	case EMartiCommandReportId::DTVICRCommand:
		break;
	}
}
