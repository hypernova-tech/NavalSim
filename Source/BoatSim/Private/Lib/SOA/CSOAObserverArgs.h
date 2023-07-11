// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Lib/BaseObj/BaseObj.h"
#include "CSOAObserverArgs.generated.h"

/**
 * 
 */
UCLASS()
class UCSOAObserverArgs : public UBaseObj
{
	GENERATED_BODY()


private:
	int SubjectId;


public:
	void SetSubjectId(int id);
	int GetSubjectId();
	
};
