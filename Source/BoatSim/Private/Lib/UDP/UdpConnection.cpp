// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/UDP/UdpConnection.h"
#include "Networking/Public/Networking.h"
#include <Lib/SystemManager/SystemManagerBase.h>
#include <Lib/Types/Primitives.h>

// Constructor
UUdpConnection::UUdpConnection()
{

}

UUdpConnection::~UUdpConnection()
{
	if (UDPThread)
	{
		// Kill() is a blocking call, it waits for the thread to finish.
		// Hopefully that doesn't take too long
		UDPThread->Kill();
		delete UDPThread;
	}
}

// Called when the game starts or when spawned
void UUdpConnection::BeginPlay()
{
	Super::BeginPlay();


	SocketSubsystem = nullptr;
	if (SocketSubsystem == nullptr) {
		SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
	}

	SendSize = 2 * 1024 * 1024;
	BufferSize = 2 * 1024 * 1024;

	LocalEndpoint = FIPv4Endpoint(FIPv4Address::Any, LocalPort);

	FIPv4Address::Parse(IP, RemoteAddress);
	RemoteEndpoint = FIPv4Endpoint(RemoteAddress, RemotePort);

	Socket = nullptr;

	if (SocketSubsystem != nullptr)
	{
		if (Socket == nullptr)
		{
			Socket = FUdpSocketBuilder(SocketDescription)
				.AsNonBlocking()
				.AsReusable()
				.BoundToEndpoint(LocalEndpoint)
				.WithReceiveBufferSize(SendSize)
				.WithSendBufferSize(BufferSize)
				.WithBroadcast();
		}
	}


	StartUDPThread();
}

// Called every frame
void UUdpConnection::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{

}

// Called when the game ends or when destroyed
void UUdpConnection::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// Stop the UDP thread
	Stop();

	Super::EndPlay(EndPlayReason);
}

bool UUdpConnection::Init()
{
	return true;
}// Do your setup here, allocate memory, ect.
uint32 UUdpConnection::Run()
{
	
	TSharedRef<FInternetAddr> targetAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	uint32 Size;
	while (!bShouldStopThread) {
		while (Socket != nullptr && Socket->HasPendingData(Size))
		{
		
			int32 BytesRead = 0;

			ReceivedData.SetNumUninitialized(FMath::Min(Size, 65507u));
			Socket->RecvFrom(ReceivedData.GetData(), ReceivedData.Num(), BytesRead, *targetAddr);

	
			
			ASystemManagerBase::GetInstance()->GetSOAImplementor()->OnReceivedNewMessage(ReceivedData, BytesRead);
		}
	}
	
	return 0;
}// Main data processing happens here

void UUdpConnection::Stop()  // Clean up any memory you allocated here
{
	if (SocketSubsystem != nullptr) {
		bShouldStopThread = true;
		SocketSubsystem->DestroySocket(Socket);
		Socket = nullptr;
		SocketSubsystem = nullptr;
		bRunThread = false;
	}

	
	
}

// Start the UDP thread
void UUdpConnection::StartUDPThread()
{
	if (ReceiveEnabled) {
		UDPThread = FRunnableThread::Create(this, TEXT("udprcvthread"));
	}
	
}

// Send UDP data
bool UUdpConnection::SendUDPData(const FString& Message)
{
	return false;
}

bool UUdpConnection::SendUDPData(const INT8U* p_bytes, INT32U count)
{
	if (Socket == nullptr) {
		return false;
	}
	INT32S bytes_sent = 0;
	Socket->SendTo(p_bytes, count, bytes_sent, *RemoteEndpoint.ToInternetAddr());
	bool ret =  bytes_sent == count;

	return ret;
}

INT16U UUdpConnection::GetLocalPort()
{
	return LocalPort;
}

INT16U UUdpConnection::GetRemotePort()
{
	return RemotePort;
}

FString UUdpConnection::GetRemoteIP()
{
	return IP;
}

