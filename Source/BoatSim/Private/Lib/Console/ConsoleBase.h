// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CCLICommandManager.h"
#include "ConsoleBase.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UConsoleBase : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UConsoleBase();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	
	bool ParseCommandLine(TCHAR* CommandLine, FString& Outcommand, TMap<FString, FString>& OutOptions, FString& OutErrorMessage);
	bool ProcessCommands(FString command, TMap<FString, FString>& options, FString &error_message);
	CCLICommandManager CommandManager;
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	virtual void Command(FString command);
	
		
};
