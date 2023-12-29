// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/Agent/Agent.h"

// Sets default values
AAgent::AAgent()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}


// Called when the game starts or when spawned
void AAgent::BeginPlay()
{
	Super::BeginPlay();
}

void AAgent::AddObjectDef(FString name, UObject* p_part)
{
	AgentObjectDefContainer.Add(name, p_part);
}

// Called every frame
void AAgent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FAgentObjectDefEntry AAgent::FindDef(FString name)
{
	return AgentObjectDefContainer.Find(name);
}


