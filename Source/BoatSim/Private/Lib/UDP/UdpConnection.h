// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Networking.h"
#include "Lib/Types/Primitives.h"
#include "Lib/UDP/IConnectionDataReceiver.h"
#include "Lib/Connection/ConnectionBase.h"
#include "UdpConnection.generated.h"

struct SUDPPacket {
	INT32U Id;
	INT32U Data[1024];
};


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UUdpConnection : public UConnectionBase,public FRunnable
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UUdpConnection();
	virtual ~UUdpConnection()  override;




protected:
	// Called when the game starts
	virtual void BeginPlay() override;


	// Called when the game ends or when destroyed
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;


	// Start the UDP thread
	void StartUDPThread();

	bool Init() override; // Do your setup here, allocate memory, ect.
	uint32 Run() override; // Main data processing happens here
	void Stop() override; // Clean up any memory you allocated here


	virtual void OnModifiedConnection(SConnectionInfo info) override;






private:
	// UDP thread object
	FRunnableThread* UDPThread;

	// UDP thread termination flag
	FThreadSafeBool bShouldStopThread;



	FSocket* Socket;

	bool bRunThread;

	ISocketSubsystem* SocketSubsystem;

	

	UPROPERTY(EditAnywhere, Category = "UDP")
		bool IsPublishPacketEnabled = false;

	UPROPERTY(EditAnywhere, Category = "UDP")
		bool ReceiveEnabled = true;

	UPROPERTY(EditAnywhere, Category = "UDP")
		bool SendEnabled = true;

	// Local Endpoint
	FString SocketDescription = "UDP Listen Socket";
	FIPv4Endpoint LocalEndpoint;

	UPROPERTY(EditAnywhere, Category = "UDP")
	uint16 LocalPort = 54000;
	UPROPERTY(EditAnywhere, Category = "UDP")
	int32 SendSize;

	TArray<uint8> ReceivedData;


	// Remote Endpoint
	FIPv4Endpoint RemoteEndpoint;
	FIPv4Address RemoteAddress;
	UPROPERTY(EditAnywhere, Category = "UDP")
	uint16 RemotePort = 54001;
	UPROPERTY(EditAnywhere, Category = "UDP")
	int32 BufferSize;
	UPROPERTY(EditAnywhere, Category = "UDP")
	FString IP = "127.0.0.1";

	// Delegate to be called when data is received

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Send UDP data
	virtual bool SendData(const FString& Message) override;
	virtual bool SendData(const INT8U* p_bytes, INT32U count) override;
	virtual bool SendData(const INT8U* p_bytes, INT32U count, INT32U port) override;

	INT16U GetLocalPort();
	INT16U GetRemotePort();
	FString GetRemoteIP();
	static FString GetMacAddr();

	
	virtual void InitConnection(void* args = nullptr) override;
	SConnectionInfo GetConnectionInfo();


};
