// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/Time/CTime.h"

CTime::CTime()
{
}

CTime::~CTime()
{
}

double CTime::GetTimeSecond()
{
    return FPlatformTime::Seconds();
}
