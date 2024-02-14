// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Lib/ActorBase/ActorBase.h"
#include <Lib/Sensor/GenericSensor/CameraBase.h>
#include <Products/IDAS/Sensors/Camera/IMartiHostIF.h>
#include "MartiCommIF/MartiCommIF.h"
#include "MartiEOSuite.generated.h"



/**
 * 
 */
UCLASS()
class AMartiEOSuite : public ASensorBase, public IMartiHostIF
{
	GENERATED_BODY()


protected:
	UPROPERTY(BlueprintReadWrite);
		ACameraBase* pDTV;
	
	UPROPERTY(BlueprintReadWrite);
		ACameraBase* pIR;

	UMartiCommIF* pMartiCommIF;
			

	SMartiLosCommandPayload			LastLosCommandReport;
	SMartiLosReportPayload			LastLosReport;

	SMartiSensorCommandPayload		LastSensorCommandPayload;
	SMartiSensorReportPayload		LastSensorReportPayload;

	SDTVDefogCommandPayload			LastDTVDefogCommandPayload;
	SDTVDefogStatus					LastDTVDefogStatus;

	SDTVICRCommandPayload			LastDTVICRCommandPayload;
	SDTVICRReportPayload			LastDTVICRReportPayload;


	virtual void InitSensor() override;
	virtual void OnPreStep(float DeltaTime) override;
	virtual void OnStep(float DeltaTime) override;

	// Inherited via IMartiHostIF
	virtual void OnRecievedMessage(SMartiGenericMessage* p_commands) override;
};
