// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/Waypoint/WaypointActor.h"
#include <Lib/PathController/PathController.h>

// Sets default values
AWaypointActor::AWaypointActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWaypointActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWaypointActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

AActor* AWaypointActor::GetOwnerPath()
{
	return pOwnerPath;
}

void AWaypointActor::SetOwnerPath(AActor* p_path)
{
	pOwnerPath = p_path;
}

void AWaypointActor::OnActorPredestroy()
{

	((APathController*)pOwnerPath)->NotifyWaypointDestroy(this);
}

