// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/Sensor/AISPublisher/AISPublisher.h"

void AAISPublisher::SendAISData(INT32U src_addr, INT32U prio, INT32U png, void* data, INT32S data_len)
{
	SAISHdr hdr;
	hdr.SrcAddr = src_addr;
	hdr.Prio = prio;
	hdr.Png = png;
	pCommIF->SendData((INT8U*) & hdr, sizeof(SAISHdr), (INT8U*)data, data_len);
}
