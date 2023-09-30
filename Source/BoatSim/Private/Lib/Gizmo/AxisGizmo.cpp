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
    this->GetComponents< UStaticMeshComponent>(AxisComponents);

	
	
}
ECoordAxis AAxisGizmo::GetAxis(UPrimitiveComponent* p_hit)
{


 

    for (USceneComponent* child : AxisComponents)
    {
        // If you specifically want to work with UStaticMeshComponent, you can cast it:
        UStaticMeshComponent* mesh = Cast<UStaticMeshComponent>(child);
        if (mesh == p_hit)
        {
            if (p_hit->GetName().Contains("XAxis")) {
                return ECoordAxis::CoordAxisX;
            }else   if (p_hit->GetName().Contains("YAxis")) {
                return ECoordAxis::CoordAxisY;
            }
            else   if (p_hit->GetName().Contains("ZAxis")) {
                return ECoordAxis::CoordAxisZ;
            }
        }
    }

    return ECoordAxis::CoordAxisUndef;

}
// Called every frame
void AAxisGizmo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

