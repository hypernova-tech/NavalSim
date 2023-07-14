// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Networking.h"
#include "UdpConnection.generated.h"


UCLASS()
class UUdpConnection : public UActorComponent,public FRunnable
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



	// Send UDP data
	bool SendUDPData(const FString& Message);

	bool Init() override; // Do your setup here, allocate memory, ect.
	uint32 Run() override; // Main data processing happens here
	void Stop() override; // Clean up any memory you allocated here

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
private:
	// UDP thread object
	FRunnableThread* UDPThread;

	// UDP thread termination flag
	FThreadSafeBool bShouldStopThread;



	FSocket* Socket;

	bool bRunThread;

	ISocketSubsystem* SocketSubsystem;


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
	FString IP = "192.168.50.232";
};
