// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/TestA/TestA.h"

// Sets default values
ATestA::ATestA()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATestA::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATestA::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

