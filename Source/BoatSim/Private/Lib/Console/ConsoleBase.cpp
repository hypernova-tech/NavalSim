// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/Console/ConsoleBase.h"
#include <Lib/Utils/CUtil.h>
#include <Lib/SystemManager/SystemManagerBase.h>
#include <Lib/Gimbal/GimbalBase.h>
#include <Lib/Sensor/GenericSensor/CameraBase.h>
#include <Lib/Sensor/GenericSensor/RadarBase.h>


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
    ProcessCommands(CommandTokens);

    
}

//cmd enable <vehiclename> <actorname> <true|false>
//gimbal <vehiclename> <actorname> angles <x y z>

void UConsoleBase::ProcessCommands(TArray<FString> tokens)
{
  
    if (tokens[0] == "gimbal") {  
        if (tokens.Num() < 7) {
            return;
        }
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
    else   if (tokens[0] == "enableall") {

        ASystemManagerBase::GetInstance()->EnableAllActors();
    }
    else   if (tokens[0] == "disableall") {

        ASystemManagerBase::GetInstance()->DisableAllActors();
    }
    else   if (tokens[0] == "camera") {

        ACameraBase* p_camera = ASystemManagerBase::GetInstance()->FindActor<ACameraBase>(tokens[1], tokens[2]);


        if (p_camera != nullptr && tokens[3] == "fov") {
            float fov = CUtil::StringToFloat64(tokens[4]);
            p_camera->SetFovDeg(fov);
               
        }
    }
    else   if (tokens[0] == "radar") {

        ARadarBase* p_radar = ASystemManagerBase::GetInstance()->FindActor<ARadarBase>(tokens[1], tokens[2]);


        if (p_radar != nullptr && tokens[3] == "range") {
            float min_range = CUtil::StringToFloat64(tokens[4]);
            float max_range = CUtil::StringToFloat64(tokens[5]);
            p_radar->SetRangeMeter(FVector2d(min_range, max_range));

        }
    }
    else   if (tokens[0] == "beam") {

        ASensorBase* p_sensor = ASystemManagerBase::GetInstance()->FindActor<ASensorBase>(tokens[1], tokens[2]);


        if (p_sensor != nullptr && tokens[3] == "enable") {
            if (tokens[4] == "true") {
                p_sensor->ShowBeam = true;
            }
            else  if (tokens[4] == "false") {
                p_sensor->ShowBeam = false;
            }

        }
    }
    else if (tokens[0] == "add") { // add boat MIR name mir1 pose -80852.000000 -41044 1684 3 4 5 
        if (tokens[1] == "boat") {
            FString model_name = tokens[2];
            FString boat_name = tokens[4];
            FVector world_pos = FVector(CUtil::StringToFloat64(tokens[6]), CUtil::StringToFloat64(tokens[7]), CUtil::StringToFloat64(tokens[8]));
            FVector world_rot = FVector(CUtil::StringToFloat64(tokens[9]), CUtil::StringToFloat64(tokens[10]), CUtil::StringToFloat64(tokens[11]));


            ASystemManagerBase::GetInstance()->AddBoat(model_name, boat_name, world_pos, world_rot);
        }
    }
}
