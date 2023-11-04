// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/JSON/CJsonDataContainer.h"
#include <Lib/Utils/CUtil.h>

CJsonDataContainer::CJsonDataContainer()
{
}

CJsonDataContainer::~CJsonDataContainer()
{
}

void CJsonDataContainer::Add(FString name, INT32S ret)
{
	Data.Add(name, CUtil::IntToString(ret));
}

void CJsonDataContainer::Add(FString name, INT32U ret)
{
	Data.Add(name, CUtil::IntToString(ret));
}

void CJsonDataContainer::Add(FString name, FLOAT64 ret)
{
	Data.Add(name, CUtil::FloatToString(ret));
}

void CJsonDataContainer::Add(FString name, BOOLEAN ret)
{
	if (ret) {
		Data.Add(name, "1");
	}
	else {
		Data.Add(name, "0");
	}
	
}

void CJsonDataContainer::Add(FString name, FVector2D ret)
{
	Data.Add(name, CUtil::VectorToString(ret));
}

void CJsonDataContainer::Add(FString name, FVector ret)
{
	Data.Add(name, CUtil::VectorToString(ret));
}

void CJsonDataContainer::Add(FString name, FString ret)
{
	Data.Add(name, (ret));
}

void CJsonDataContainer::Add(FString name, FColor ret)
{
	Data.Add(name, CUtil::ColorToString(ret));
}

void CJsonDataContainer::Add(FString name, FVector4 ret)
{
	Data.Add(name, CUtil::VectorToString(ret));
}

