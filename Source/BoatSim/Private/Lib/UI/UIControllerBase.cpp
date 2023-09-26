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
	FindActorAtClickPosition(locationX, locationY);
}

void AUIControllerBase::OnMouseLeftButtonUp(int locationX, int locationY)
{
	CUtil::DebugLog("OnMouseLeftButtonUp");
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
	FVector End = Start + (WorldDirection * TOUE(1000));  // MaxTraceDistance is up to you, e.g., 10000.0f

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
			CUtil::DebugLogScreen(ClickedActor->GetName());
			ASystemManagerBase::GetInstance()->GetConsole()->SendToConsole("clicked: "+ ClickedActor->GetName());
			
		}
	}
}

void AUIControllerBase::SetConsoleOutputText(FString text)
{
	if (ConsoleText) {
		ConsoleText->SetText(FText::FromString(text));
	}
	
}
