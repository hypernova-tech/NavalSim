// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CCLICommandManager.h"
#include <Lib/UDP/UdpConnection.h>
#include "ConsoleBase.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UConsoleBase : public UActorComponent,public IConnectionDataReceiver
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UConsoleBase();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	UUdpConnection* pUDPConnection;

	
	bool ParseCommandLine(TCHAR* CommandLine, FString& Outcommand, TMap<FString, FString>& OutOptions, FString& OutErrorMessage);
	bool ProcessCommands(FString command, TMap<FString, FString>& options, FString &error_message);
	CCLICommandManager CommandManager;
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	virtual void Command(FString command);
	virtual void OnReceivedConnectionData(void* connection, INT8U* p_data, INT32U count);
		
};
