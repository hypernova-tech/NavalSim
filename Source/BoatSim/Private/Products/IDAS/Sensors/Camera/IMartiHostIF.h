// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MartiCommIF/MartiDataTypes.h"



class IMartiHostIF
{
public:
	IMartiHostIF() {};
	virtual void OnRecievedMessage(SMartiGenericMessage* p_commands) = 0;

};