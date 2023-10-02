// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/InputManager/InputManagerBase.h"
#include "InputManagerBase.h"
#include <Lib/Utils/CUtil.h>

AInputManagerBase* AInputManagerBase::pInstance = nullptr;
// Sets default values
AInputManagerBase::AInputManagerBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	pInstance = this;

}

// Called when the game starts or when spawned
void AInputManagerBase::BeginPlay()
{
	Super::BeginPlay();
	pPlayerController = GetWorld()->GetFirstPlayerController();
	pPlayerController->InputComponent->BindAction("LeftMouseButtonClick", IE_Pressed, this, &AInputManagerBase::HandleMouseLeftDown).bConsumeInput = false;
	pPlayerController->InputComponent->BindAction("LeftMouseButtonClick", IE_Released, this, &AInputManagerBase::HandleMouseLeftUp).bConsumeInput = false;

	pPlayerController->InputComponent->BindAction("RightMouseButtonClick", IE_Pressed, this, &AInputManagerBase::HandleMouseRightDown).bConsumeInput = false;
	pPlayerController->InputComponent->BindAction("RightMouseButtonClick", IE_Released, this, &AInputManagerBase::HandleMouseRightUp).bConsumeInput = false;

	
}

AInputManagerBase* AInputManagerBase::GetInstance()
{
	return pInstance;
}

// Called every frame
void AInputManagerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInputManagerBase::HandleMouseLeftDown()
{
	MousePosLeftDown = GetMousePosition();
	CUtil::DebugLog(" Drag MousePosLeftDown "+ MousePosLeftDown.ToFVector2D().ToString());
}
void AInputManagerBase::HandleMouseLeftUp()
{

}
void AInputManagerBase::HandleMouseRightDown()
{
	MousePosRightDown = GetMousePosition();
}
void AInputManagerBase::HandleMouseRightUp()
{

}

void AInputManagerBase::ShowInputInterest(AActor* p_actor)
{
	p_actor->AddTickPrerequisiteActor(this);
}

void AInputManagerBase::ShowInputInterest(UActorComponent* p_comp)
{
	p_comp->AddTickPrerequisiteActor(this);
}

SMousePos AInputManagerBase::GetMousePosition()
{
	SMousePos ret;
	pPlayerController->GetMousePosition(ret.X, ret.Y);
	return ret;
}

FVector2D AInputManagerBase::GetLeftButtonDrag()
{
	auto curr_pos = GetMousePosition();
	FVector2D ret =  FVector2D(curr_pos.X - MousePosLeftDown.X, curr_pos.Y - MousePosLeftDown.Y);
	return ret;
}

SMousePos AInputManagerBase::GetMousePositionLeftDown()
{
	return MousePosLeftDown;
}
SMousePos AInputManagerBase::GetMousePositionRightDown()
{
	return MousePosRightDown;
}
