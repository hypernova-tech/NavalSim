// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/Console/ConsoleBase.h"
#include <Lib/Utils/CUtil.h>
#include <Lib/SystemManager/SystemManagerBase.h>
#include <Lib/Gimbal/GimbalBase.h>
#include <Lib/Sensor/GenericSensor/CameraBase.h>
#include <Lib/Sensor/GenericSensor/RadarBase.h>
#include <Lib/Math/CMath.h>


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

    pUDPConnection = GetOwner()->GetComponentByClass<UUdpConnection>();
    pUDPConnection->AddConnectionDataReceiver(this);

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

    TMap<FString, FString> outopt;
    FString outcommand;
    FString out_error_message;

    bool ret = ParseCommandLine(command.GetCharArray().GetData(), outcommand, outopt, out_error_message);

    if (ret) {
        FString command_process_error_message = "";
        auto command_ret = ProcessCommands(outcommand, outopt, command_process_error_message);

        if (pUDPConnection != nullptr) {
            if (command_ret) {
                pUDPConnection->SendUDPData(TEXT("OK"));
            }
            else {
                pUDPConnection->SendUDPData(TEXT("FAILED") + command_process_error_message);
            }
        }
    }
    else {
        pUDPConnection->SendUDPData(TEXT("FAILED") + out_error_message);
        CUtil::DebugLog(out_error_message);
    }
    
}

void UConsoleBase::OnReceivedConnectionData(void* connection, INT8U* p_data, INT32U count)
{
    // Create a task to execute on the game thread
    FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([&]()
        {

            // Update the actor's location
            auto cmd = CUtil::CharToFString((char*)p_data);
            Command(cmd);

        }, TStatId(), nullptr, ENamedThreads::GameThread);

    // Wait for the task to complete
    FTaskGraphInterface::Get().WaitUntilTaskCompletes(Task);
   

}

bool UConsoleBase::ParseCommandLine(TCHAR* CommandLine, FString& OutCommand, TMap<FString, FString>& OutOptions,FString& OutErrorMessage)
{
    FString CmdLineStr(CommandLine);
    TArray<FString> Tokens;
    FString CurrentToken;
    bool bInsideQuotes = false;

    for (TCHAR Char : CmdLineStr)
    {
        if (Char == '"')
        {
            bInsideQuotes = !bInsideQuotes;
            continue;
        }

        if (Char == ' ' && !bInsideQuotes)
        {
            if (!CurrentToken.IsEmpty())
            {
                Tokens.Add(CurrentToken);
                CurrentToken.Empty();
            }
        }
        else
        {
            CurrentToken += Char;
        }
    }

    // Error: Unmatched quotes
    if (bInsideQuotes)
    {
        OutErrorMessage = TEXT("Error: Unmatched quotes in command line.");
        return false;
    }

    // Add the last token
    if (!CurrentToken.IsEmpty())
    {
        Tokens.Add(CurrentToken);
    }

    // Error: Empty command line
    if (Tokens.Num() == 0)
    {
        OutErrorMessage = TEXT("Error: Empty command line.");
        return false;
    }

    // The first token is usually the command
    OutCommand = Tokens[0];
    Tokens.RemoveAt(0);

    // Error: Empty command
    if (OutCommand.IsEmpty())
    {
        OutErrorMessage = TEXT("Error: Empty command.");
        return false;
    }

    for (int32 Index = 0; Index < Tokens.Num(); ++Index)
    {
        if (Tokens[Index].StartsWith("--"))
        {
            FString Key = Tokens[Index].Mid(2);
            FString Value;

            // Error: Empty key
            if (Key.IsEmpty())
            {
                OutErrorMessage = TEXT("Error: Empty key.");
                return false;
            }

            if (Index + 1 < Tokens.Num() && !Tokens[Index + 1].StartsWith("--"))
            {
                Value = Tokens[++Index];
            }

            OutOptions.Add(Key, Value);
        }
    }

    return true;
}




//cmd enable <vehiclename> <actorname> <true|false>
//gimbal <vehiclename> <actorname> angles <x y z>
#if false
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
    else   if (tokens[0] == "camera") {//camera sida_name sensor_name fov 5 

        ACameraBase* p_camera = ASystemManagerBase::GetInstance()->FindActor<ACameraBase>(tokens[1], tokens[2]);


        if (p_camera != nullptr && tokens[3] == "fov") {
            float fov = CUtil::StringToFloat64(tokens[4]);
            p_camera->SetFovDeg(fov);
               
        }
    }
    else   if (tokens[0] == "radar") { //radar sida_name sensor_name rage 1000 

        ARadarBase* p_radar = ASystemManagerBase::GetInstance()->FindActor<ARadarBase>(tokens[1], tokens[2]);


        if (p_radar != nullptr && tokens[3] == "range") {
            float min_range = CUtil::StringToFloat64(tokens[4]);
            float max_range = CUtil::StringToFloat64(tokens[5]);
            p_radar->SetRangeMeter(FVector2d(min_range, max_range));

        }
    }
    else   if (tokens[0] == "beam") {// beam sida_name sensor_name enable true

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
    else if (tokens[0] == "add") { // add boat MIR name mir1 pose 10.0 -0 0 0 0 0 0.3 0.3 0.3
        if (tokens[1] == "boat") {
            FString model_name = tokens[2];
            FString boat_name = tokens[4];
            FVector world_pos = FVector(WORLDTOUNREALUNIT * CUtil::StringToFloat64(tokens[6]), WORLDTOUNREALUNIT * CUtil::StringToFloat64(tokens[7]), WORLDTOUNREALUNIT * CUtil::StringToFloat64(tokens[8]));
            FVector world_rot = FVector(CUtil::StringToFloat64(tokens[9]), CUtil::StringToFloat64(tokens[10]), CUtil::StringToFloat64(tokens[11]));
            FVector scale = FVector(CUtil::StringToFloat64(tokens[12]), CUtil::StringToFloat64(tokens[13]), CUtil::StringToFloat64(tokens[14]));


            ASystemManagerBase::GetInstance()->AddBoat(model_name, boat_name, world_pos, world_rot, scale);
        }
    }
}
#endif
bool UConsoleBase::ProcessCommands(FString command, TMap<FString, FString>& options, FString& error_message)
{
    error_message = "";
    CommandManager.SetCommandOptions(&options);

    if (command == "help") {
        auto cmd_info = CommandManager.GetCommandInfo();


        for (auto It = (*cmd_info).CreateConstIterator(); It; ++It)
        {
            FString Key = It->Key;
            TArray<SCommandOptionInfo> ValueArray = It->Value;

            // Iterate over the TArray
            FString ret = "";
            pUDPConnection->SendUDPData(Key);
            for (SCommandOptionInfo& Option : ValueArray)
            {
                ret = Option.Option + " " + "["+Option.Description+"]";

                if (pUDPConnection != nullptr) {
                    pUDPConnection->SendUDPData(ret);
                }
                
            }
        }
        pUDPConnection->SendUDPData("\n");
        return true;

    }
  
    else if (command == "process") {
        auto instance  = CommandManager.GetProcessKillInstanceCount();
      
        if (ASystemManagerBase::GetInstance()->GetInstanceNo() == instance) {
            
            ASystemManagerBase::GetInstance()->ForceExit();
            return true;
        }

     
        else {
            error_message += "empty name value";
            return false;
        }

      
    }
    else if (command == "print") { 
        auto bp = CommandManager.HasBP();
        if (bp) {
            auto bp_info = ASystemManagerBase::GetInstance()->GetDataContainer()->GetBlueprintInfo();
            for (auto entry : *bp_info) {
                pUDPConnection->SendUDPData("BP: "+ entry.Name);
            }

            return true;
        }
        auto print_actors = CommandManager.HasActors();
        if (print_actors) {
            auto all_actors = ASystemManagerBase::GetInstance()->GetAllActorInWorld();
            for (auto p_actor : all_actors)
            {

                pUDPConnection->SendUDPData(p_actor->GetName());
            }
            return true;
        }

        return false;
        
    }
    else if (command == "sim") {
        bool val = false;
        bool ret = CommandManager.GetStart(val);
        if (ret) {
           if (val) {
                ASystemManagerBase::GetInstance()->StartSimulation();
           }

            return true;
        }
        ret = CommandManager.GetPause(val);
        if (ret) {
            if (val) {
                ASystemManagerBase::GetInstance()->PauseSimulation();
            }

            return true;
        }

        ret = CommandManager.GetResume(val);
        if (ret) {
            if (val) {
                ASystemManagerBase::GetInstance()->ResumeSimulation();
            }

            return true;
        }

        return false;

    }
    else if (command == "create") {
        auto name = CommandManager.GetName();
        if (name != "") {
            
        }
        else {
            error_message += "empty name value";
            return false;
        }

        auto bp = CommandManager.GetBP();
        if (bp != "") {
           
            auto ret = ASystemManagerBase::GetInstance()->CreateActor(bp, name, FVector::ZeroVector, FVector::ZeroVector, FVector::OneVector);
            if (ret ==nullptr) {
                error_message = "object cannot created";
                return false;
            }
            
        }

        auto model = CommandManager.GetModel();
        if (model != "") {

        }
    }
    else if (command == "destroy") {
        auto name = CommandManager.GetName();
        if (name != "") {
            if (ASystemManagerBase::GetInstance()->DestroyActor(name)) {
                return true;
            }
            else {

            }

        }
        else {
            error_message += "empty name value";
            return false;
        }


    }
    else if (command == "set") {


        FString controller;
        bool ret = CommandManager.GetContoroller(controller);

        if (ret) {
            bool ctrl_ret = ASystemManagerBase::GetInstance()->SetMainPlayerController(controller);
            return ctrl_ret;
        }

        auto name = CommandManager.GetName();
        if (name != "") {

        }
        else {
            error_message += "empty name value";
            return false;
        }
        
        AActor *p_actor = ASystemManagerBase::GetInstance()->FindActor(name);
        FVector vec;

        if (p_actor == nullptr) {
            error_message += (("cannot find actor "));
            return false;
        }

        ret = CommandManager.GetPosition(vec );
        if (ret) {
            p_actor->SetActorLocation(vec * WORLDTOUNREALUNIT);
        }

        ret = CommandManager.GetRelPosition(vec);
        if (ret) {
            p_actor->SetActorRelativeLocation(vec*WORLDTOUNREALUNIT);
        }

        ret = CommandManager.GetRotation(vec);
        if (ret) {
            CMath::SetActorRotation(p_actor, vec);
        }
        ret = CommandManager.GetRelRotation(vec);
        if (ret) {
            CMath::SetActorRelativeRotation(p_actor, vec);
        }

        ret = CommandManager.GetScale(vec);
        if (ret) {
            p_actor->SetActorScale3D(vec);
        }
        FString parent;
        ret = CommandManager.GetParent(parent);
        if (ret) {
           AActor* p_parent = ASystemManagerBase::GetInstance()->FindActor(parent);
           CUtil::SetParent(p_actor, p_parent);
        }

  
    }
    else if (command == "get") {


        bool ret = CommandManager.HasController();
        if (ret) {
            auto player_controller = ASystemManagerBase::GetInstance()->GetMainPlayerController();
            if (player_controller != nullptr) {
                pUDPConnection->SendUDPData("Possed: " + player_controller->GetName());
            }
            else {
                pUDPConnection->SendUDPData("possed: nullptr (world)");
            }

            return true;
        }


        auto name = CommandManager.GetName();
        if (name != "") {

        }
        else {
            error_message += "empty name value";
            return false;
        }

        if (name == "*") {
            auto all_actors =  ASystemManagerBase::GetInstance()->GetAllActorInWorld();
            for (auto p_actor: all_actors)
            {
          
                pUDPConnection->SendUDPData("Actor: "+ p_actor->GetName());
            }
            return true;
        }

        AActor* p_actor = ASystemManagerBase::GetInstance()->FindActor(name);
        FVector vec;

        if (p_actor == nullptr) {
            error_message += (("cannot find actor "));
            return false;
        }

        ret = CommandManager.HasPosition();
        if (ret) {
            pUDPConnection->SendUDPData("Pos: " + p_actor->GetActorLocation().ToString());
            return true;
            
        }

        ret = CommandManager.HasRelposition();
        if (ret) {
            pUDPConnection->SendUDPData("Relpos: " + p_actor->GetActorLocation().ToString());
            return true;
          
        }

        ret = CommandManager.HasRotation();
        if (ret) {
            
            vec = CMath::GetActorEulerAnglesRPY(p_actor);
            pUDPConnection->SendUDPData("Rot: " + vec.ToString());
            return true;
        }
        ret = CommandManager.HasRelrotation();
        if (ret) {
            vec = CMath::GetActorReleativeEulerAnglesRPY(p_actor);
            pUDPConnection->SendUDPData("Relrot: " + vec.ToString());
            return true;
        }

        ret = CommandManager.HasScale();
        if (ret) {
            vec = p_actor->GetActorScale();;
            pUDPConnection->SendUDPData("Scale: " + vec.ToString());
            return true;
        }

        ret = CommandManager.HasParent();
        if (ret) {
            auto parent = CUtil::GetParentActor(p_actor);
            if (parent != nullptr) {
                pUDPConnection->SendUDPData("Parent: " + parent->GetName());
            }
            else {
                pUDPConnection->SendUDPData("Parent: nullptr (world)");
            }
           
            return true;
        }

     
      
    }



    return false;

}
