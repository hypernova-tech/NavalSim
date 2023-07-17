// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/PointVisualizer/PointVisualizer.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
APointVisualizer::APointVisualizer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ProceduralMeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralMeshComponent"));
  
	RootComponent = ProceduralMeshComponent;
    pSceneCapture2D = GetComponentByClass<USceneCaptureComponent2D>();
}

// Called when the game starts or when spawned
void APointVisualizer::BeginPlay()
{
	Super::BeginPlay();
    // Get all actors in the level
    TArray<AActor*> AllActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), AllActors);
 
    AllActors.Remove(this);
    if (pSceneCapture2D != nullptr){
        pSceneCapture2D->HiddenActors = AllActors;
    }
    

	
}

// Called every frame
void APointVisualizer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APointVisualizer::Visualize(TArray<FVector> points)
{
    if (ProceduralMeshComponent && points.Num() > 0)
    {
        TArray<FVector> Vertices;
        for (const FVector& Point : points)
        {
            Vertices.Add(Point);
        }

        TArray<int32> Triangles;
        // Populate Triangles array based on your desired triangulation

        TArray<FVector> Normals; // Optional: provide vertex normals if needed
        TArray<FVector2D> UVs; // Optional: provide UV coordinates if needed

        ProceduralMeshComponent->CreateMeshSection(0, Vertices, Triangles, Normals, UVs, TArray<FColor>(), TArray<FProcMeshTangent>(), true);
    }
}