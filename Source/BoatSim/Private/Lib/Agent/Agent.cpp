// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/Agent/Agent.h"

// Sets default values
AAgent::AAgent()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//if (pStaticMesh) {
		MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
		RootComponent = MeshComponent; // Set the mesh component as the root component
	//}
	



}

void AAgent::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	// Check if the mesh is set in the property. If so, apply it to the mesh component.
	if (pStaticMesh)
	{
		MeshComponent->SetStaticMesh(pStaticMesh);
	}
}

// Called when the game starts or when spawned
void AAgent::BeginPlay()
{
	Super::BeginPlay();

	
	
}

// Called every frame
void AAgent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


