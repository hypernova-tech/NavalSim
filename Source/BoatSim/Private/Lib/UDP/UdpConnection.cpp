// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/UDP/UdpConnection.h"
#include "Networking/Public/Networking.h"
#include <Lib/SystemManager/SystemManagerBase.h>
#include <Lib/Types/Primitives.h>
#include <Lib/Utils/CUtil.h>

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
	ASystemManagerBase::GetInstance()->GetSystemAPI()->RegisterConnection(this);

	
}

void UUdpConnection::InitConnection()
{
	Super::InitConnection();
	
	SocketSubsystem = nullptr;
	if (SocketSubsystem == nullptr) {
		SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
	}

	SendSize = 2 * 1024 * 1024;
	BufferSize = 2 * 1024 * 1024;

	LocalEndpoint = FIPv4Endpoint(FIPv4Address::Any, ASystemManagerBase::GetInstance()->GetSystemAPI()->ConvertToInstancedLocalPort(LocalPort));

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

SConnectionInfo UUdpConnection::GetConnectionInfo()
{
	SConnectionInfo info;

	info.IpAddr = IP;
	info.LocalPort = LocalPort;
	info.RemotePort = RemotePort;

	return info;

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
	ReceivedData.SetNumUninitialized(2000);
	while (!bShouldStopThread) {
		while (Socket != nullptr && Socket->HasPendingData(Size))
		{
		
			int32 BytesRead = 0;

			//ReceivedData.SetNumUninitialized(FMath::Min(Size, 65507u));
			Socket->RecvFrom(ReceivedData.GetData(), ReceivedData.Num(), BytesRead, *targetAddr);

			if (BytesRead > 0) {
				if (IsPublishPacketEnabled) {
					ASystemManagerBase::GetInstance()->GetSOAImplementor()->OnReceivedNewMessage(ReceivedData, BytesRead);
				}


				for (auto connection : ConnectionDataReceivers) {
					auto pdata = ReceivedData.GetData();
					pdata[BytesRead] = 0;
					connection->OnReceivedConnectionData(this, pdata, BytesRead);
				}
			}
	
	
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

void UUdpConnection::OnModifiedConnection(SConnectionInfo info)
{
	IP = info.IpAddr;
	LocalPort = info.LocalPort;
	RemotePort = info.RemotePort;

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
	char dest[2048];

	CUtil::FStringToAsciiChar(Message, dest, sizeof(dest));
	auto ret = SendUDPData((const INT8U*)dest, Message.Len());
	return ret;
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

bool UUdpConnection::SendUDPData(const INT8U* p_bytes, INT32U count, INT32U port)
{
	FIPv4Endpoint end_point;

	end_point = FIPv4Endpoint(RemoteAddress, port);

	if (Socket == nullptr) {
		return false;
	}
	INT32S bytes_sent = 0;
	Socket->SendTo(p_bytes, count, bytes_sent, *end_point.ToInternetAddr());
	bool ret = bytes_sent == count;

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




