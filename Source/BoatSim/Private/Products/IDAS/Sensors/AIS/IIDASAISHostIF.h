// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "IDASAISDataTypes.h"



class IIDASAISHostIF
{
public:
	IIDASAISHostIF() {};
	virtual void OnRecievedMessage(INT8U message_id, void* p_commands) = 0;

};