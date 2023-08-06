// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/Sensor/SensorBase.h"
#include <Lib/Utils/CUtil.h>

// Sets default values
ASensorBase::ASensorBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	

	

	
}

void ASensorBase::Run(float delta_time_sec)
{
}

void ASensorBase::SensorStateMachine(float delta_time_sec)
{
	ESensorState current_state = SensorState;
	ESensorState  next_state = current_state;

	switch (current_state)
	{
	case Init:
		InitSensor();
		next_state = ESensorState::Running;
		break;
	case Running:
		Run(delta_time_sec);
		break;
	case Finish:
		FinalizeSensor();
		break;
	default:
		break;
	}

	SensorState = next_state;
}

void ASensorBase::InitSensor()
{
	pCommIF = GetComponentByClass<UGenericCommIF>();
	if (SimulatePhysicsEnabled) {

	}
	else {
		this->DisableComponentsSimulatePhysics();

	}
}



void ASensorBase::FinalizeSensor()
{
}

SClutterParams ASensorBase::GetClutterParams()
{
	SClutterParams Params;

	Params.EnableFoamDetect = EnableFoamDetect;
	Params.EnableSubsurfaceScan = EnableSubsurfaceDetect;
	Params.EnableSurfaceDetect = EnableSurfaceDetect;
	Params.SeaSurfaceDetectionProbability = SeaSurfaceDetectionProbability;
	Params.MaxSurfacePenetrationMeter = MaxSurfacePenetrationMeter;
	return Params;
}

// Called when the game starts or when spawned
void ASensorBase::BeginPlay()
{
	CUtil::DebugLog("ASensorBase Beginplay");
	Super::BeginPlay();
	CUtil::DebugLog("TArray<AActor*> child_actors");
	TArray<AActor*> child_actors;
	GetAllChildActors(child_actors, true);

	for (AActor* ChildActor : child_actors)
	{
		CUtil::DebugLog("ChildActor");
		// Check if the child actor is of the desired class
		if (ChildActor->IsA<APointVisualizer>())
		{
			pPointVisualizer = Cast<APointVisualizer>(ChildActor);
			// Perform actions with the found child actor
			break;
		}
	}

}

// Called every frame
void ASensorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (Enabled) {
		SensorStateMachine(DeltaTime);
	}
	
}

void ASensorBase::Visualize(SScanResult* p_scan_result, FVector origin, FVector current_forward, FVector current_right, float max_range_meter)
{
	if (pPointVisualizer != nullptr) {
		pPointVisualizer->Visualize(p_scan_result, origin, current_forward, current_right, max_range_meter);
	}
	
}

UGenericCommIF* ASensorBase::GetCommCommIF()
{
	return pCommIF;
}

