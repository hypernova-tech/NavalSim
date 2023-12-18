// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/UI/UIControllerBase.h"
#include <Lib/SystemManager/SystemManagerBase.h>
#include <Lib/Utils/CUtil.h>

// Sets default values
AUIControllerBase::AUIControllerBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AUIControllerBase::BeginPlay()
{
	Super::BeginPlay();
	pGizmoUIController = GetComponentByClass<UGizmoUIController>();
	pGizmoUIController->SetGizmoActor(GizmoActor);
}

// Called every frame
void AUIControllerBase::Tick(float DeltaTime)
{
	FLOAT64 elp_time_sec = -1;

	if (TotalFrameCount > 0) {
		elp_time_sec = CUtil::Tock(LastFrameTime);
	}
	Super::Tick(DeltaTime);
	if (elp_time_sec > 0) {
		ComputeFPS(elp_time_sec);
	}
	
	TotalFrameCount++;
	
	LastFrameTime = CUtil::Tick();

}

void AUIControllerBase::SetSensorSlotImage(int no, UImage* p_image)
{
	SensorSlots.Add(p_image) ;
}

UImage* AUIControllerBase::GetSensorSlotImage(int no)
{

	return SensorSlots[no];
}
int AUIControllerBase::GetSensorSlotCount()
{
	return SensorSlots.Num();
}

void AUIControllerBase::SetSensorTitle(int no, UTextBlock* p_val)
{
	SensorTitles.Add(p_val);
}


UTextBlock* AUIControllerBase::GetSensorTitle(int no)
{
	return SensorTitles[no];
}

void AUIControllerBase::SetSensorTitleText(int no, FString val)
{
	auto sensor_title = GetSensorTitle(no);

	if (sensor_title != nullptr) {
		sensor_title->SetText(FText::FromString(val));
	}
	
}

void AUIControllerBase::SetConsoleOutputTextWidget(UTextBlock* pwidget)
{
	ConsoleText = pwidget;
}

void AUIControllerBase::SetCoordTextWidget(UTextBlock* pwidget)
{
	CoordText = pwidget;
}

void AUIControllerBase::OnMouseLeftButtonDown(int locationX, int locationY)
{

	pGizmoUIController->OnMouseLeftButtonDown(locationX, locationY);
	
	if (pGizmoUIController->FindGizmoAtClickPosition(locationX, locationY) == nullptr) {
		FindActorAtClickPosition(locationX, locationY);
	}
	
	
}

void AUIControllerBase::OnMouseLeftButtonUp(int locationX, int locationY)
{

	pGizmoUIController->OnMouseLeftButtonUp(locationX, locationY);
}

void AUIControllerBase::DisplayClickedCoord(const FHitResult& hit_res)
{
	auto p_uicontroller = ASystemManagerBase::GetInstance()->GetUIController();
	auto p_map_origin = ASystemManagerBase::GetInstance()->GetMapOrigin();
	FVector hit_loc = hit_res.Location;
	auto llh = p_map_origin->ConvertUEXYZToLLH(hit_loc);
	FString hit_coord = "LatLonH: " + CUtil::VectorToString(llh) + " UEXYZ: +" + CUtil::VectorToString(hit_loc);
	p_uicontroller->SetCoordTextValue(hit_coord);
}

void AUIControllerBase::FindActorAtClickPosition(int locationX, int locationY)
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	FVector WorldLocation;
	FVector WorldDirection;

	if (PlayerController)
	{
		PlayerController->DeprojectScreenPositionToWorld(
			locationX, locationY,
			WorldLocation,
			WorldDirection
		);
	}

	FHitResult HitResult;

	// Define start and end points of the ray
	FVector Start = WorldLocation;
	FVector End = Start + (WorldDirection * TOUE(10000));  // MaxTraceDistance is up to you, e.g., 10000.0f

	// Perform the line trace
	// The ECC_Visibility channel will typically work, but you can change this if necessary
	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		Start,
		End,
		ECC_Visibility
	);

	if (bHit)
	{
		// HitResult now contains information about what was hit
		DisplayClickedCoord(HitResult);
		AActor* ClickedActor = HitResult.GetActor();
		if (ClickedActor->ActorHasTag("Gizmo")) {
			ClickedActor = CUtil::GetParentActor(ClickedActor);
		}

		if (ClickedActor&& !ClickedActor->ActorHasTag("Gizmo"))
		{
			SelectActor(ClickedActor);
#if 0
			CUtil::DebugLogScreen(ClickedActor->GetName());
			ASystemManagerBase::GetInstance()->GetConsole()->SendToConsole("<clicked>"+ ClickedActor->GetName());
			pSelectedActor = ClickedActor;
			ASystemManagerBase::GetInstance()->SetSelectedActor(pSelectedActor);
			pGizmoUIController->SetTrackedActor(pSelectedActor);
#endif
			
		}
	}
	else {
		pSelectedActor = nullptr;
		ASystemManagerBase::GetInstance()->SetSelectedActor(nullptr);
	}
}

void AUIControllerBase::SetConsoleOutputText(FString text)
{
	if (ConsoleText) {
		ConsoleText->SetText(FText::FromString(text));
	}
	
}
void AUIControllerBase::SelectActor(AActor* p_actor)
{
	CUtil::DebugLogScreen(p_actor->GetName());
	ASystemManagerBase::GetInstance()->GetConsole()->SendToConsole("<clicked>" +p_actor->GetName());
	ASystemManagerBase::GetInstance()->GetConsole()->SendActorTransform(p_actor);
	pSelectedActor = p_actor;
	ASystemManagerBase::GetInstance()->SetSelectedActor(pSelectedActor);
	pGizmoUIController->SetTrackedActor(pSelectedActor);
}

void AUIControllerBase::SetCoordTextValue(FString val)
{
	if (CoordText) {
		CoordText->SetText(FText::FromString(val));
	}
}

UGizmoUIController* AUIControllerBase::GetGizmoController()
{
	return pGizmoUIController;
}
float AUIControllerBase::GetAverageFPS()
{
	return AverageFPS;
}

bool AUIControllerBase::GetIsUIVisible()
{
	return IsUIVisible;
}

void AUIControllerBase::SetIsUIVisible(bool val)
{
	IsUIVisible = val;
}

void AUIControllerBase::ComputeFPS(float DeltaTime)
{
	FPSMeasurments.Add(DeltaTime);

	if (FPSMeasurments.Num() > 100) {
		FPSMeasurments.RemoveAt(0);
	}
	FLOAT32 total_time = 0;
	for (int i = 0; i < FPSMeasurments.Num(); i++) {
		total_time += FPSMeasurments[i];
	}

	AverageFPS = FPSMeasurments.Num()/ total_time;
}