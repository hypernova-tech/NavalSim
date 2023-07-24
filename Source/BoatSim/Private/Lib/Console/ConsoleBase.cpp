// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/Console/ConsoleBase.h"
#include <Lib/Utils/CUtil.h>
#include <Lib/SystemManager/SystemManagerBase.h>
#include <Lib/Gimbal/GimbalBase.h>



// Sets default values for this component's properties
UConsoleBase::UConsoleBase()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UConsoleBase::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}




// Called every frame
void UConsoleBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UConsoleBase::Command(FString command)
{

  
    TArray<FString> CommandTokens;
    command.ParseIntoArrayWS(CommandTokens);

    if (CommandTokens.Num() < 2)
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid command format. Expected: <cmd> <id> <param1> <param2> <param3> <param4>"));
        return;
    }

    FString Cmd = CommandTokens[0];
    int32 ID = FCString::Atoi(*CommandTokens[1]);

    FString Param1 = CommandTokens.Num() > 2 ? CommandTokens[2] : FString();
    FString Param2 = CommandTokens.Num() > 3 ? CommandTokens[3] : FString();
    FString Param3 = CommandTokens.Num() > 4 ? CommandTokens[4] : FString();
    FString Param4 = CommandTokens.Num() > 5 ? CommandTokens[5] : FString();
    FString Param5 = CommandTokens.Num() > 6 ? CommandTokens[6] : FString();

    // Now you can process the parsed command as needed.
    // Example: You can call functions or perform actions based on the command and its parameters.
    // For simplicity, let's just log the parsed values.

    UE_LOG(LogTemp, Warning, TEXT("Parsed Command: Cmd=%s, ID=%d, Param1=%s, Param2=%s, Param3=%s, Param4=%s"),
        *Cmd, ID, *Param1, *Param2, *Param3, *Param4);



    ProcessCommands(CommandTokens);

    
}

//cmd enable <vehiclename> <actorname> <true|false>
//gimbal <vehiclename> <actorname> angles <x y z>

void UConsoleBase::ProcessCommands(TArray<FString> tokens)
{
    if (tokens.Num() < 7) {
        return;
    }
    if (tokens[0] == "gimbal") {  
        FString gimbal_command = tokens[3];
        AGimbalBase* p_gimbal = ASystemManagerBase::GetInstance()->FindActor<AGimbalBase>(tokens[1], tokens[2]);
        if (p_gimbal != nullptr && gimbal_command == "angles") {
            double roll = CUtil::StringToFloat64(tokens[4]);
            double pitch = CUtil::StringToFloat64(tokens[5]);
            double yaw = CUtil::StringToFloat64(tokens[6]);

            
            p_gimbal->SetCommand(roll, pitch, yaw);
        }
    } else   if (tokens[0] == "enable") {

        AActorBase* p_actor = ASystemManagerBase::GetInstance()->FindActor<AActorBase>(tokens[1], tokens[2]);
        

        if (p_actor != nullptr ) {
            if (tokens[3] == "true") {
                p_actor->SetEnabled(true);
            }else  if (tokens[3] == "false") {
                p_actor->SetEnabled(false);
            }
     
        }
    }
}
