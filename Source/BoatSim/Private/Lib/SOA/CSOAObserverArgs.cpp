// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/SOA/CSOAObserverArgs.h"

void UCSOAObserverArgs::SetSubjectId(int id)
{
	SubjectId = id;
}

int UCSOAObserverArgs::GetSubjectId()
{
	return SubjectId;
}
