#include "UDPReceiver.h"







AUDPReceiver::AUDPReceiver(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ListenSocket = NULL;
}

void AUDPReceiver::Recv(const FArrayReaderPtr & ArrayReaderPtr, const FIPv4Endpoint & EndPt)
{
	if (!&ArrayReaderPtr) {
		UE_LOG(LogTemp, Warning, TEXT("Cannot read array, nullptr returned."));
		return;
	}
	
	got_new_data = true;

	//if (UpdateInterest)
		Archive(ArrayReaderPtr);
}

bool AUDPReceiver::StartUDPReceiver(const FString & SocketName, const int32 Port)
{
	FIPv4Endpoint Endpoint(FIPv4Address::Any, Port);
	int32 BufferSize = 2 * 1024 * 1024;
	ListenSocket = FUdpSocketBuilder(*SocketName).AsNonBlocking()
		.AsReusable()
		.BoundToEndpoint(Endpoint)
		.WithReceiveBufferSize(BufferSize);
	
	FTimespan ThreadWaitTime = FTimespan::FromMilliseconds(100);
	Receiver = new FUdpSocketReceiver(ListenSocket, ThreadWaitTime, TEXT("UDP Receiver"));
	Receiver->OnDataReceived().BindUObject(this, &AUDPReceiver::Recv);
	Receiver->Start();
	return true;
}

void AUDPReceiver::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	delete Receiver;
	Receiver = nullptr;

	//Clear all sockets
	if (ListenSocket)
	{
		ListenSocket->Close();
		ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(ListenSocket);
	}
}

void AUDPReceiver::Archive(const FArrayReaderPtr & ArrayReaderPtr) {


	if (ArrayReaderPtr.IsValid())
	{
		uint8* Data = ArrayReaderPtr->GetData();
		FUDPData data;
		FMemory::Memcpy(data.Bytes, Data, ArrayReaderPtr->Num());
		data.Len = ArrayReaderPtr->Num();
		

		//Class Parameter Update function to set object ready for a query from blueprint
		UpdateReceiverData(data);
		// Access and manipulate the data in the struct as needed
		// ...
	}


	//*ArrayReaderPtr << data;
	


}

void AUDPReceiver::UpdateReceiverData(FUDPData data)
{
	MyData = data;
	InputBuffer.Add(data);
}

FUDPData AUDPReceiver::GetData()
{
	UpdateInterest = true;	// Ready for new data
	got_new_data = false;
	return MyData;
}


int AUDPReceiver::GetReceivedPacketCount()
{

	return InputBuffer.Num();

}

TArray<uint8>& AUDPReceiver::GetNextPacket()
{
	UpdateInterest = true;	// Ready for new data
	got_new_data = false;

	OutputBuffer.Reset();

	if (InputBuffer.Num() > 0) {
		FUDPData& inp = InputBuffer[0];
		TArray<uint8> curr_inp;
		OutputBuffer.Append(inp.Bytes, inp.Len);
		InputBuffer.RemoveAt(0);
		return OutputBuffer;
	}
	else {
		return OutputBuffer;
	}

	
}



bool AUDPReceiver::IsNewDataReady() {
	return got_new_data;
}