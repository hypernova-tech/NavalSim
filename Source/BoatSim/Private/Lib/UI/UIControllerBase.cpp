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
	
}

// Called every frame
void AUIControllerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ComputeFPS(DeltaTime);

}

void AUIControllerBase::SetLidarSlotImage(int no, UImage* p_image)
{
	LidarSlots.Add(p_image) ;
}

UImage* AUIControllerBase::GetLidarSlotImage(int no)
{

	return LidarSlots[no];
}
int AUIControllerBase::GetLidarSlotCount()
{
	return LidarSlots.Num();
}




void AUIControllerBase::SetCameraSlotImage(int no, UImage* p_image)
{
	CameraSlots.Add(p_image);
}

UImage* AUIControllerBase::GetCameraSlotImage(int no)
{

	return CameraSlots[no];
}
int AUIControllerBase::GetCameraSlotCount()
{
	return CameraSlots.Num();
}

void AUIControllerBase::SetConsoleOutputTextWidget(UTextBlock* pwidget)
{
	ConsoleText = pwidget;
}

void AUIControllerBase::OnMouseLeftButtonDown(int locationX, int locationY)
{

	CUtil::DebugLog("OnMouseLeftButtonDown");
	FindGizmoAtClickPosition( locationX,  locationY);
	FindActorAtClickPosition(locationX, locationY);
}

void AUIControllerBase::OnMouseLeftButtonUp(int locationX, int locationY)
{
	CUtil::DebugLog("OnMouseLeftButtonUp");
}

void AUIControllerBase::FindGizmoAtClickPosition(int locationX, int locationY)
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
	FVector End = Start + (WorldDirection * TOUE(1000));  // MaxTraceDistance is up to you, e.g., 10000.0f
	
	CUtil::DrawDebugRay(GetWorld(), Start, End, FColor::Red, 10, 0.75);

	// Perform the line trace
	// The ECC_Visibility channel will typically work, but you can change this if necessary
	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		Start,
		End,
		ECC_GameTraceChannel1
	);

	if (bHit)
	{
		// HitResult now contains information about what was hit
		AActor* ClickedActor = HitResult.GetActor();
		
		if (ClickedActor)
		{
			CUtil::DebugLogScreen(ClickedActor->GetName());
			

		}
	}
	else {
	
	}
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
		AActor* ClickedActor = HitResult.GetActor();
		if (ClickedActor)
		{
			CUtil::DebugLogScreen(ClickedActor->GetActorLabel()/*ClickedActor->GetName()*/);
			ASystemManagerBase::GetInstance()->GetConsole()->SendToConsole("clicked: "+ ClickedActor->GetName());
			pSelectedActor = ClickedActor;
			ASystemManagerBase::GetInstance()->SetSelectedActor(pSelectedActor);
			GizmoActor->SetActorLocation(pSelectedActor->GetActorLocation());
			GizmoActor->SetActorRotation(pSelectedActor->GetActorRotation());
			
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
float AUIControllerBase::GetAverageFPS()
{
	return AverageFPS;
}

void AUIControllerBase::ComputeFPS(float DeltaTime)
{
	FPSMeasurments.Add(DeltaTime);

	if (FPSMeasurments.Num() > 10) {
		FPSMeasurments.RemoveAt(0);
	}
	FLOAT32 total_fps = 0;
	for (int i = 0; i < FPSMeasurments.Num(); i++) {
		total_fps += 1/FPSMeasurments[i];
	}

	AverageFPS = total_fps / FPSMeasurments.Num();
}