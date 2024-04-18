// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Lib/ActorBase/ActorBase.h"
#include <Lib/Sensor/GenericSensor/CameraBase.h>
#include <Products/IDAS/Sensors/Camera/IMartiHostIF.h>
#include "MartiCommIF/MartiCommIF.h"
#include <Lib/Gimbal/GimbalBase.h>
#include "MartiEOSuite.generated.h"




/**
 * 
 */
UCLASS()
class AMartiEOSuite : public ASensorBase, public IMartiHostIF
{
	GENERATED_BODY()


protected:

	
	ACameraBase* pDTV;
	ACameraBase* pIR;
	ACameraBase* pActiveCamera;
	AGimbalBase* pGimbal;
	UMartiCommIF* pMartiCommIF;
	INT32S StreamerInstanceId;
	void* hDLL;

	SMartiLosCommandPayload			LastLosCommand;
	SMartiLosReportPayload			LastLosReport;

	SMartiSensorCommandPayload		LastSensorCommandPayload;
	SMartiSensorReportPayload		LastSensorReportPayload;

	SDTVDefogCommandPayload			LastDTVDefogCommandPayload;
	SDTVDefogStatus					LastDTVDefogStatus;

	SDTVICRCommandPayload			LastDTVICRCommandPayload;
	SDTVICRReportPayload			LastDTVICRReportPayload;

	virtual void BeginPlay() override;
	virtual void InitSensor() override;
	virtual void Run(float delta_time_sec) override;
	virtual void OnPreStep(float DeltaTime) override;
	virtual void OnStep(float DeltaTime) override;

	// Inherited via IMartiHostIF
	virtual void OnRecievedMessage(SMartiGenericMessage* p_commands) override;
	void HandleLosCommand(SMartiLosCommandPayload* p_cmd);
	void HandleSensorCommand(SMartiSensorCommandPayload *p_cmd);
	void HandleDefogCommand(SDTVDefogCommandPayload* p_cmd);
	void HandleICRCommand(SDTVICRCommandPayload* p_cmd);

	void ChangeActiveCamera(ACameraBase* p_req);
	void UpdateDTVFieldOfView(EDTVFieldOfView fov_cmd);
	void UpdateIRFieldOfView(EThermalFieldOfView fov_cmd);

	void SendLosReportCommand();
	void SendSensorReport();
	void SendDefogReport();
	void SendICRReport();


	UPROPERTY(EditAnywhere)
		FVector RateOverride; //todo sil



};
