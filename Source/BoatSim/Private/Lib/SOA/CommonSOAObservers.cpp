// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/SOA/CommonSOAObservers.h"
CommonSOAObservers* CommonSOAObservers::pInstance = nullptr;
CommonSOAObservers::CommonSOAObservers()
{
	pInstance = this;
}

CommonSOAObservers::~CommonSOAObservers()
{
}
