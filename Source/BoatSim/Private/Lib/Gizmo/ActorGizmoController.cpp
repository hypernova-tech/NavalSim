// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/Gizmo/ActorGizmoController.h"

// Sets default values
AActorGizmoController::AActorGizmoController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AActorGizmoController::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AActorGizmoController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

