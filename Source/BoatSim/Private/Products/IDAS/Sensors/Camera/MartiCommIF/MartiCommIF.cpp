// Fill out your copyright notice in the Description page of Project Settings.


#include "Products/IDAS/Sensors/Camera/MartiCommIF/MartiCommIF.h"
#include "Products/IDAS/Sensors/Camera/MartiCommIF/MartiDataTypes.h"

void UMartiCommIF::BeginPlay()
{
	Super::BeginPlay();
	
	pUDPConnection->AddConnectionDataReceiver(this);
	RegisterConnectionInfo(0, pUDPConnection->GetConnectionInfo(), pUDPConnection);

}

uint32 UMartiCommIF::Run()
{
	while (!IsStoped) {
		FPlatformProcess::Sleep(0.01);
	}
	return uint32();
}

void UMartiCommIF::Stop()
{
}

void UMartiCommIF::SetHostIf(IMartiHostIF* p_val)
{
	pHostIf = p_val;
}

void UMartiCommIF::SendMessage(EMartiCommandReportId id, INT8U* payload, INT32S size)
{
	SMartiGenericMessage mes;
	mes.SetMessage(id, payload, size);
	pUDPConnection->SendData(mes.Message.AllData, mes.Message.Header.MessageSize);
}

void UMartiCommIF::OnReceivedConnectionData(void* connection, INT8U* p_data, INT32U count)
{
	Super::OnReceivedConnectionData(connection, p_data, count);
	
	SMartiGenericMessage* p_mes = (SMartiGenericMessage*)p_data;

	if (p_mes->IsMessageValid()) {
		pHostIf->OnRecievedMessage(p_mes);
	}


}