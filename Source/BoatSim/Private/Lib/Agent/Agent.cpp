// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/Agent/Agent.h"
#include <Lib/SystemManager/SystemManagerBase.h>
#include <Lib/Utils/CUtil.h>

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

void AAgent::PostInitializeComponents()
{
    Super::PostInitializeComponents();
   
}

void AAgent::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);
  
}

void AAgent::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (EncapsulateStaticMeshAsActor && !BuiltOnce) {
        ProcessActor(this, GetWorld());
        BuiltOnce = true;
    }
}

void AAgent::AddObjectDef(FString name, UObject* p_part)
{
	AgentObjectDefContainer.Add(name, p_part);
}


FAgentObjectDefEntry AAgent::FindDef(FString name)
{
	return AgentObjectDefContainer.Find(name);
}


void AAgent::CreateActorsForStaticMeshes(USceneComponent* Component, TMap<USceneComponent*, AActor*>& MapComponentToActor, UWorld* World)
{
    if (!Component || !World)
    {
        return;
    }

    TArray<USceneComponent*> children;
    TArray<AActor*> actors;
    TArray<USceneComponent*> meshes;
    Component->GetChildrenComponents(true, children);


    for (USceneComponent* Child : children)
    {
        if (UStaticMeshComponent* MeshComponent = Cast<UStaticMeshComponent>(Child))
        {
            // Create a new actor for this mesh
            AActor* NewActor = ASystemManagerBase::GetInstance()->CreateActor("EMPTY", GetName() +"_"+ MeshComponent->GetName(), MeshComponent->GetComponentLocation(), MeshComponent->GetComponentRotation().Euler(), FVector::OneVector, 0);
            //NewActor->IsSaveLoadCreateEnabled = false
      
            MeshComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
          

            UStaticMeshComponent* NewStaticMeshComponent = NewObject<UStaticMeshComponent>(NewActor);
            if (NewStaticMeshComponent)
            {
                NewStaticMeshComponent->RegisterComponent();
                NewStaticMeshComponent->AttachToComponent(NewActor->GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);

                NewStaticMeshComponent->SetStaticMesh(MeshComponent->GetStaticMesh());
                NewStaticMeshComponent->SetMaterial(0, MeshComponent->GetMaterial(0));
                NewStaticMeshComponent->SetWorldTransform(MeshComponent->GetComponentTransform());

            }
            MeshComponent->DestroyComponent();


            actors.Add(NewActor);
            CUtil::SetParent(NewActor, Component->GetOwner());


        }
    }


}

void AAgent::ProcessActor(AActor* OriginalActor, UWorld* World)
{
    if (!OriginalActor || !World)
    {
        return;
    }

    TMap<USceneComponent*, AActor*> MapComponentToActor;
    CreateActorsForStaticMeshes(OriginalActor->GetRootComponent(), MapComponentToActor, World);
    //RebuildHierarchy(OriginalActor, MapComponentToActor);

}