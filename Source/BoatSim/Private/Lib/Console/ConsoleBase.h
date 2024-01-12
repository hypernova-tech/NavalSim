// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CCLICommandManager.h"
#include <Lib/UDP/UdpConnection.h>
#include <Lib/SystemManager/ISystemAPI.h>
#include "ConsoleBase.generated.h"

class ISystemAPI;


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
	bool ProcessCommands(FString command, TMap<FString, FString>& options, FString &error_message, INT32S& delay_tick);
	CCLICommandManager CommandManager;
	ISystemAPI* pSystemAPI;

	bool ProcessHelpCommand(TMap<FString, FString>& options, FString& error_message);
	bool ProcessWorkspaceCommand(TMap<FString, FString>& options, FString& error_message);
	bool ProcessProcessCommand(TMap<FString, FString>& options, FString& error_message);
	bool ProcessPrintCommand(TMap<FString, FString>& options, FString& error_message);
	bool ProcessSimCommand(TMap<FString, FString>& options, FString& error_message);
	bool ProcessCreateCommand(TMap<FString, FString>& options, FString& error_message, INT32S& delay_tick);
	bool ProcessDestroyCommand(TMap<FString, FString>& options, FString& error_message);
	bool ProcessSetCommand(TMap<FString, FString>& options, FString& error_message);
	bool ProcessGetCommand(TMap<FString, FString>& options, FString& error_message);
	bool ProcessExecCommand(TMap<FString, FString>& options, FString& error_message);

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
	void SendConsoleResponse(FString name, FString option, FString prop, FString val);
	FString CreateAndSerializeJson(TMap<FString, FString> &data);
	bool SetParent(AActor* p_actor, FString parent, FString full_path);
	
	bool ParseVectorFromString(const FString& InString, FVector& OutVector);
	bool ParseVectorFromString(const FString& InString, FVector2D& OutVector);
	bool SetCamView(FString cmd);
	FTimerHandle TimerHandle;
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	virtual void Command(FString command, int &delay_tick);
	virtual void OnReceivedConnectionData(void* connection, INT8U* p_data, INT32U count);
	void SendToConsole(FString val);
	CCLICommandManager* GetCommandManager();

	void SendActorTransform(AActor* p_actor);
	TArray<FString> GetCallableFunctions(UObject* p_obj, FString category);
	TArray<FString> GetProperties(UObject* p_obj);
	bool SetPropertyValue(UObject* p_obj, const FString& property_name, const FString &value);
	bool GetPropertyValue(UObject* p_obj, const FString& property_name, FString &value);
	bool InvokeFunctionByNameWithParameters(UObject* p_obj, const FName& FunctionName, const TMap<FString, FString>& Parameters);
	bool InvokeFunctionByNameWithParameters(UObject* p_obj, const FName& FunctionName,  TArray<FString>& Parameters);
	bool SetFunctionParameter(UFunction* Function, void* Params, FProperty* Param, const FString& Value);

	void SendSensors();
	void SendActors();
	void SendActorBases();
	void SendBlueprints();
	void SendFunctions(AActor* p_actor, FString category);
	void SendProperies(AActor* p_actor);
};
