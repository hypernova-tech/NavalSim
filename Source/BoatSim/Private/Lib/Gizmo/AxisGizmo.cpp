// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/Gizmo/AxisGizmo.h"

// Sets default values
AAxisGizmo::AAxisGizmo()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAxisGizmo::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAxisGizmo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

