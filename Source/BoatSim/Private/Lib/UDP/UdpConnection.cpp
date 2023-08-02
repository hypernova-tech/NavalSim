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
				.WithReceiveBufferSize(BufferSize)
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

	UDPThread = FRunnableThread::Create(this, TEXT("Give your thread a good name"));
}



// Send UDP data
bool UUdpConnection::SendUDPData(const FString& Message)
{

#if false
	FIPv4Address out_adr;
	FIPv4Address::Parse(IPAddress, out_adr);
	FIPv4Endpoint Endpoint(out_adr, Port);
	TSharedRef<FInternetAddr> InternetAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	InternetAddr->SetIp(Endpoint.Address.Value);
	InternetAddr->SetPort(Endpoint.Port);

	// Create the UDP socket
	FSocket* Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_DGram, TEXT("MyUDPSocket"), false);
	Socket->SetReuseAddr();

	// Connect the socket to the IP address and port
	bool bConnectSuccessful = Socket->Connect(*InternetAddr);
	if (!bConnectSuccessful)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to connect UDP socket to %s:%d"), *IPAddress, Port);
		Socket->Close();
		return false;
	}

	UE_LOG(LogTemp, Display, TEXT("Connected UDP socket to %s:%d"), *IPAddress, Port);

	// Convert the message string to bytes
	TCHAR_TO_UTF8(*Message);
	int32 BytesSent = 0;

	// Send the UDP data
	bool bSendSuccessful = Socket->Send((uint8*)UTF8Chars.Get(), UTF8Chars, BytesSent);
	if (!bSendSuccessful || BytesSent != UTF8Chars.Length())
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to send UDP data"));
		Socket->Close();
		return false;
	}

	Socket->Close();
	return true;

#endif

	return false;
}

bool UUdpConnection::SendUDPData(const INT8U* p_bytes, INT32U count)
{
	return false;
}
