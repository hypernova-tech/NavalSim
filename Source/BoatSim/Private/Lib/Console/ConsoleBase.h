// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CCLICommandManager.h"
#include <Lib/UDP/UdpConnection.h>
#include <Lib/SystemManager/ISystemAPI.h>
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
	ISystemAPI* pSystemAPI;

	bool ProcessHelpCommand(TMap<FString, FString>& options, FString& error_message);
	bool ProcessWorkspaceCommand(TMap<FString, FString>& options, FString& error_message);
	bool ProcessProcessCommand(TMap<FString, FString>& options, FString& error_message);
	bool ProcessPrintCommand(TMap<FString, FString>& options, FString& error_message);
	bool ProcessSimCommand(TMap<FString, FString>& options, FString& error_message);
	bool ProcessCreateCommand(TMap<FString, FString>& options, FString& error_message);
	bool ProcessDestroyCommand(TMap<FString, FString>& options, FString& error_message);
	bool ProcessSetCommand(TMap<FString, FString>& options, FString& error_message);
	bool ProcessGetCommand(TMap<FString, FString>& options, FString& error_message);

	void SendConsoleResponse(AActor *p_actor);
	void SendConsoleResponse(FString name, FString option, INT32S ret);
	void SendConsoleResponse(FString name, FString option, FLOAT64 ret);
	void SendConsoleResponse(FString name, FString option, BOOLEAN ret);
	void SendConsoleResponse(FString name, FString option, FVector2D ret);
	void SendConsoleResponse(FString name, FString option, FVector ret);
	void SendConsoleResponse(FString name, FString option, FString ret);
	void SendConsoleResponse(FString name, FString option, FColor ret);
	void SendConsoleResponse(FString name, FString option, FVector4 ret);
	void SendConsoleResponse(FString name, FString option, INT32S ind, FVector ret);
	FString CreateAndSerializeJson(TMap<FString, FString> &data);

	void SendSensors();
	void SendActors();
	void SendActorBases();
	void SendBlueprints();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	virtual void Command(FString command);
	virtual void OnReceivedConnectionData(void* connection, INT8U* p_data, INT32U count);
	void SendToConsole(FString val);
	CCLICommandManager* GetCommandManager();

	void SendActorTransform(AActor* p_actor);
		
};
