// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Lib/Types/Primitives.h>
#include "InputManagerBase.generated.h"


struct SMousePos
{
public:
	FLOAT64 X;
	FLOAT64 Y;

	FVector2D ToFVector2D()
	{
		return FVector2D(X, Y);
	}

};

UCLASS()
class AInputManagerBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInputManagerBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	APlayerController* pPlayerController;
	
	void HandleMouseLeftDown();
	void HandleMouseLeftUp();
	void HandleMouseRightDown();
	void HandleMouseRightUp();

public:	
	static AInputManagerBase* GetInstance();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void ShowInputInterest(AActor *p_actor);
	void ShowInputInterest(UActorComponent* p_actor);
	SMousePos GetMousePosition();
	SMousePos GetMousePositionLeftDown();
	SMousePos GetMousePositionRightDown();
	FVector2D GetLeftButtonDrag();

private:
	static AInputManagerBase* pInstance;
	SMousePos MousePosLeftDown;
	SMousePos MousePosRightDown;
	

};
