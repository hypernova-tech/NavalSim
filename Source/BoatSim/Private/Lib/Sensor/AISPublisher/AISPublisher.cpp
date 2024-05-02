// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/Sensor/AISPublisher/AISPublisher.h"

void AAISPublisher::SendAISData(void* data, INT32S data_len)
{
	pCommIF->SendData(data, data_len);
}
