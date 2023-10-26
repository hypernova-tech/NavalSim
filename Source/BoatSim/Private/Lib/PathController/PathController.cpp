// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/PathController/PathController.h"
#include <Lib/Utils/CUtil.h>
#include <Lib/SystemManager/SystemManagerBase.h>

APathController::APathController()
{
	pSplineMeshComponent = CreateDefaultSubobject<USplineMeshComponent>(TEXT("SplineMeshComponent"));
}

void APathController::BeginPlay()
{
	Super::BeginPlay();
	pSplineFollower  = FindComponentByClass<USplineFollower>();
	
	RootComponent = pSplineMeshComponent;
}

void APathController::OnStep(float DeltaTime)
{
	Super::OnStep(DeltaTime);
	pSplineFollower->OnStep(DeltaTime);

}

void APathController::AddOrModifyWaypoint(INT32S wp_ind,  FVector wp)
{
	if (wp_ind >= pSplineFollower->GetWaypointCount()) {
		FString name = GetName() + "wp" + CUtil::IntToString(wp_ind);
		auto wp_actor = ASystemManagerBase::GetInstance()->CreateActor("WP", name, TOUE(wp), FVector::ZeroVector, FVector::OneVector);
		AWaypointActor* p_wp = (AWaypointActor*)wp_actor;
		p_wp->SetOwnerPath(this);
		pSplineFollower->AddWaypoint(wp, (AWaypointActor*)wp_actor);
	}
	else {
		pSplineFollower->ModifyWaypoint(wp_ind, wp);
	}

}

bool APathController::GetWaypointPosition(INT32S ind, FVector &ret)
{
	if(ind < pSplineFollower->Waypoints.Num()){
		ret = pSplineFollower->Waypoints[ind]->GetActorLocation();
		return true;
	}
	return false;
}

void APathController::Bake()
{
	if (IsStraightLine) {
		for (int32 i = 0; i < pSplineFollower->GetWaypointCount(); ++i)
		{
			// Calculate the direction from this point to the next
			FVector Direction = FVector::ZeroVector;

			if (i < pSplineFollower->GetWaypointCount() - 1)
			{
				Direction = (pSplineFollower->GetWaypointLocation(i + 1), pSplineFollower->GetWaypointLocation(i)).GetSafeNormal();
			}

			// Set the tangents for this spline point; this will affect the curve segment starting at this point
			pSplineFollower->pSplineComponent->SetTangentAtSplinePoint(i, Direction * 50.0f, ESplineCoordinateSpace::World, true);  // The value 50.0f is arbitrary; you might need a different value
		}

		pSplineFollower->pSplineComponent->UpdateSpline();
	}

	pSplineFollower->TurnRateDegPerSec = TurnRateDegPerSec;
	pSplineFollower->BuildPath(IsClosed);

	GenerateDrawablePathSegments();
	UpdatePathColor();

}

void APathController::GenerateDrawablePathSegments()
{
	for (auto segment : PathSegments) {
		segment->DestroyComponent();
	}
	PathSegments.Reset();
	auto pspline_comp = pSplineFollower->pSplineComponent;
	// Get the total length of the spline
	float totalLength = pspline_comp->GetSplineLength();
	float step_distance_meter = 5;
	float  step_distance = totalLength / NumSegments;  // This ensures even spacing of segments.

	// Calculate the number of segments based on your step distance
	int32 numSegments = FMath::CeilToInt(totalLength / step_distance);

	// For each segment, we're going to find the start and end points based on the step distance
	for (int32 i = 0; i < numSegments; ++i)
	{
		// Calculate the distance along the spline for the start and end of this segment
		float startDist = i * step_distance;
		float endDist = FMath::Fmod((i + 1) * step_distance, totalLength);

		// Make sure we're not exceeding the spline's total length
		endDist = FMath::Min(endDist, totalLength);

		// Get the location and tangent at the start and end distances
		FVector StartLocation = pspline_comp->GetLocationAtDistanceAlongSpline(startDist, ESplineCoordinateSpace::Local);
		FVector StartTangent = pspline_comp->GetTangentAtDistanceAlongSpline(startDist, ESplineCoordinateSpace::Local);
		FVector EndLocation = pspline_comp->GetLocationAtDistanceAlongSpline(endDist, ESplineCoordinateSpace::Local);
		FVector EndTangent = pspline_comp->GetTangentAtDistanceAlongSpline(endDist, ESplineCoordinateSpace::Local);

		// ... (the rest of your loop creating the USplineMeshComponent instances remains unchanged)
		USplineMeshComponent* SplineMesh = NewObject<USplineMeshComponent>(this);

		SplineMesh->SetupAttachment(RootComponent);  // Assuming you're in an AActor subclass

		// Set the mesh you want to deform
		SplineMesh->SetStaticMesh(pMesh);  // Replace with your mesh
		SplineMesh->SetMaterial(0, pSplineMaterial);
		// Set the start and end points for this mesh segment
		SplineMesh->SetStartAndEnd(
			StartLocation, StartTangent * step_distance,  // You might need to adjust the tangent's length based on your step_distance
			EndLocation, EndTangent * step_distance,
			true);
		SplineMesh->RegisterComponent();

		PathSegments.Add(SplineMesh);
		// ...
	}
}

void APathController::SetIsClosed(bool val)
{
	IsClosed = val;
}

bool APathController::GetIsClosed()
{
	return IsClosed;
}

void APathController::SetPathSpeed(FLOAT64 val)
{
	pSplineFollower->MoveSpeedMetersPerSec = val;
}
FLOAT64 APathController::GetPathSpeed()
{
	return pSplineFollower->MoveSpeedMetersPerSec;
}

void APathController::AddAttachedActor(AActor* p_actor)
{
	pSplineFollower->pAttachedObject = p_actor;
}

AActor* APathController::GetAttachedActor()
{
	return pSplineFollower->pAttachedObject;
}

void APathController::UpdatePathColor()
{
	UMaterialInterface* OriginalMaterial = PathSegments[0]->GetStaticMesh()->GetMaterial(0); // Get the original material

	// Create a dynamic material instance (a copy of the material that you can safely modify)
	UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(OriginalMaterial, this);
	FColor MyColor = PathColor; // For instance, red.
	FLinearColor MyLinearColor = MyColor.ReinterpretAsLinear();

	// Set the color parameter. "EmissiveColor" should match the name used in your material parameter.
	DynamicMaterial->SetVectorParameterValue(FName("EmissiveColor"), MyLinearColor);

	for (auto segment : PathSegments) {
		auto StaticMeshComp = segment->GetStaticMesh();
		// Now, set this material instance to the mesh component
		StaticMeshComp->SetMaterial(0, DynamicMaterial);

		// Convert FColor to FLinearColor. Note that FColor is in sRGB space, so we need to convert it properly.
		
	}


}

void APathController::NotifyWaypointMoved(AWaypointActor* p_wp)
{
	pSplineFollower->UpdateWaypointsPositions();
	Bake();
}

void APathController::NotifyWaypointDestroy(AWaypointActor* p_wp)
{
	pSplineFollower->RemoveWaypoint(p_wp);
	Bake();
}

void APathController::Save(ISaveLoader* p_save_loader)
{
	Super::Save(p_save_loader);

	
	FString line;


	for (int i = 0; i < pSplineFollower->GetWaypointCount(); i++) {
		line = p_save_loader->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
		p_save_loader->AppendOption(line, CCLICommandManager::Wp, i);
		p_save_loader->AppendOption(line, CCLICommandManager::WpPos, TOW(pSplineFollower->Waypoints[i]->GetActorLocation()));
		p_save_loader->AddLine(line);
	}

	line = p_save_loader->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
	p_save_loader->AppendOption(line, CCLICommandManager::Speed, pSplineFollower->MoveSpeedMetersPerSec);
	p_save_loader->AddLine(line);

	line = p_save_loader->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
	p_save_loader->AppendOption(line, CCLICommandManager::Closed, GetIsClosed());
	p_save_loader->AddLine(line);

	line = p_save_loader->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
	FString attached = GetAttachedActor()!= nullptr ? GetAttachedActor()->GetName() : "";
	p_save_loader->AppendOption(line, CCLICommandManager::Attach, attached);
	p_save_loader->AddLine(line);
}

