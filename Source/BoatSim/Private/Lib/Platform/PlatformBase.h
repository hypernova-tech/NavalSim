// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CBoatBase.h"
#include "PlatformBase.generated.h"

/**
 * 
 */
UCLASS()
class APlatformBase : public ACBoatBase
{
	GENERATED_BODY()
protected:

	UPROPERTY(EditAnywhere)
	bool AutoPosseses = false;


public:
	virtual void BeginPlay() override;
};
