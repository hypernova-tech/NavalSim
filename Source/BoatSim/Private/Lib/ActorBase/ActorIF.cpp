// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/ActorBase/ActorIF.h"

void IActorIF::SetBaseActor(AActor* p_actor)
{
	pOwner = p_actor;
}

void IActorIF::SetEnabled(bool val)
{
	
}

bool IActorIF::GetEnabled()
{
	
}
