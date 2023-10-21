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
    ASystemManagerBase::GetInstance()->SetConsoleConnection(pUDPConnection);
    pSystemAPI = ASystemManagerBase::GetInstance()->GetSystemAPI();
	// ...
	
}




// Called every frame
void UConsoleBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


CCLICommandManager* UConsoleBase::GetCommandManager()
{
    return &CommandManager;
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
                pSystemAPI->SendConsoleResponse(TEXT("OK"));
            }
            else {
                pSystemAPI->SendConsoleResponse(TEXT("FAILED") + command_process_error_message);
            }
        }
    }
    else {
        pSystemAPI->SendConsoleResponse(TEXT("FAILED") + out_error_message);
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

void UConsoleBase::SendToConsole(FString val)
{
    if (pUDPConnection != nullptr) {
        pUDPConnection->SendUDPData(val);
    }
   
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

void UConsoleBase::SendConsoleResponse(FString option, INT32S ret)
{
    pSystemAPI->SendConsoleResponse(option + " :" + CUtil::IntToString(ret));
}
void UConsoleBase::SendConsoleResponse(FString option, FLOAT64 ret)
{
    pSystemAPI->SendConsoleResponse(option + " :" + CUtil::FloatToString(ret));
}
void UConsoleBase::SendConsoleResponse(FString option, BOOLEAN ret)
{
    pSystemAPI->SendConsoleResponse(option + " :" + CUtil::BoolToStringBinary(ret));
}

void UConsoleBase::SendConsoleResponse(FString option, FVector2D ret)
{
    pSystemAPI->SendConsoleResponse(option + " :" + ret.ToString());
}
void UConsoleBase::SendConsoleResponse(FString option, FVector ret)
{
    pSystemAPI->SendConsoleResponse(option + " :" + ret.ToString());
}

void UConsoleBase::SendConsoleResponse(FString option, FString ret)
{
    pSystemAPI->SendConsoleResponse(option + " :" + ret);
}

bool UConsoleBase::ProcessHelpCommand(TMap<FString, FString>& options, FString& error_message)
{

    error_message = "";
    FString sret;




    auto cmd_info = CommandManager.GetCommandInfo();


    for (auto It = (*cmd_info).CreateConstIterator(); It; ++It)
    {
        FString Key = It->Key;
        TArray<SCommandOptionInfo> ValueArray = It->Value;

        // Iterate over the TArray
        sret = "";
        pSystemAPI->SendConsoleResponse(Key);
        for (SCommandOptionInfo& Option : ValueArray)
        {
            sret = Option.Option + " " + "[" + Option.Description + "]";

            if (pUDPConnection != nullptr) {
                pSystemAPI->SendConsoleResponse(sret);
            }

        }
    }
    pSystemAPI->SendConsoleResponse("\n");
    return true;
}


bool UConsoleBase::ProcessProcessCommand(TMap<FString, FString>& options, FString& error_message)
{

    error_message = "";
    auto instance = CommandManager.GetProcessKillInstanceCount();

    if (ASystemManagerBase::GetInstance()->GetInstanceNo() == instance) {

        ASystemManagerBase::GetInstance()->ForceExit();
        return true;
    }


    else {
        error_message += "empty name value";
        return false;
    }
}

bool UConsoleBase::ProcessPrintCommand(TMap<FString, FString>& options, FString& error_message)
{

    error_message = "";


    auto bp = CommandManager.HasBP();
    if (bp) {
        auto bp_info = ASystemManagerBase::GetInstance()->GetDataContainer()->GetBlueprintInfo();
        for (auto entry : *bp_info) {

            pSystemAPI->SendConsoleResponse("BP: " + entry.Name);
        }

        return true;
    }
    auto print_actors = CommandManager.HasActors();
    if (print_actors) {
        auto all_actors = ASystemManagerBase::GetInstance()->GetAllActorInWorld();
        for (auto p_act : all_actors)
        {

            pSystemAPI->SendConsoleResponse("<actor>" + p_act->GetName());
        }
        return true;
    }

    auto has_sensors = CommandManager.HasSensors();
    if (has_sensors) {
        auto all_sensors = pSystemAPI->GetAllSensors();
        for (auto p_sensor : all_sensors)
        {
            pSystemAPI->SendConsoleResponse("<sensor>" + p_sensor->GetName());
        }
        return true;
    }


    FString sensor_type;
    auto has_sensor = CommandManager.GetSensorType(sensor_type);
    if (has_sensor) {
        auto all_sensors_of_type = pSystemAPI->GetSensorsOfType(pSystemAPI->StringToSensor(sensor_type));
        for (auto p_sensor : all_sensors_of_type)
        {
            pSystemAPI->SendConsoleResponse(p_sensor->GetName());
        }
        return true;
    }


    auto has_sensors_types = CommandManager.HasSensorTypes();
    if (has_sensors_types) {
        auto types = pSystemAPI->GetAllSensorTypes();
        for (auto stype : types)
        {
            pSystemAPI->SendConsoleResponse(pSystemAPI->SensorToString(stype));
        }
        return true;
    }

    return false;
}
bool UConsoleBase::ProcessSimCommand(TMap<FString, FString>& options, FString& error_message)
{
    error_message = "";
    FVector2D vec2d;
    bool ret;


    bool val = false;
    ret = CommandManager.GetStart(val);
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

bool UConsoleBase::ProcessCreateCommand(TMap<FString, FString>& options, FString& error_message)
{
    error_message = "";
    FString name;


    name = CommandManager.GetName();
    if (name != "") {

    }
    else {
        error_message += "empty name value";
        return false;
    }

    auto bp = CommandManager.GetBP();
    if (bp != "") {

        auto actor_created = ASystemManagerBase::GetInstance()->CreateActor(bp, name, FVector::ZeroVector, FVector::ZeroVector, FVector::OneVector);
        if (actor_created == nullptr) {
            error_message = "object cannot created";
            return false;
        }

    }

    auto model = CommandManager.GetModel();
    if (model != "") {

    }

    return true;
}

bool UConsoleBase::ProcessDestroyCommand(TMap<FString, FString>& options, FString& error_message)
{
    error_message = "";
    FString name;


    name = CommandManager.GetName();
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

    return false;
}

bool UConsoleBase::ProcessSetCommand(TMap<FString, FString>& options, FString& error_message)
{
    error_message = "";
    FVector2D vec2d;
    bool ret;
    FString sret;
    FString name;
    AActor* p_actor;
    FVector vec;
    bool is_enabled;
    bool is_active;
    INT32S sint;
    FLOAT64 dbl;


    FString controller;
    ret = CommandManager.GetContoroller(controller);

    if (ret) {
        bool ctrl_ret = ASystemManagerBase::GetInstance()->SetMainPlayerController(controller);
        return ctrl_ret;
    }

    ret = CommandManager.GetValue(CCLICommandManager::Selected, sret);
    if (ret) {
        auto selected = pSystemAPI->FindActor(sret);

        if (selected) {
            pSystemAPI->SetSelectedActor(selected);
            pSystemAPI->SetGizmoTrackedActor(selected);
            return true;
        }
        else {
            error_message += (("cannot find actor "));
            return false;
        }
    }
    else {

    }

    name = CommandManager.GetName();
    if (name != "") {

    }
    else {
        error_message += "empty name value";
        return false;
    }

    p_actor = ASystemManagerBase::GetInstance()->FindActor(name);


    if (p_actor == nullptr) {
        error_message += (("cannot find actor "));
        return false;
    }


    ret = CommandManager.GetActive(is_active);
    if (ret) {
        CUtil::SetActorActive(p_actor, is_active);
        return true;

    }


    ret = CommandManager.GetEnabled(is_enabled);
    if (ret) {
        pSystemAPI->SetActorEnabled(p_actor, is_enabled);
        return true;

    }

    int  instance_no = -1;
    ret = CommandManager.GetInstance(instance_no);
    if (ret) {
        pSystemAPI->SetActorInstanceNo(p_actor, instance_no);
        return true;

    }

    ret = CommandManager.GetPosition(vec);
    if (ret) {
        p_actor->SetActorLocation(vec * WORLDTOUNREALUNIT);
    }

    ret = CommandManager.GetRelPosition(vec);
    if (ret) {
        p_actor->SetActorRelativeLocation(vec * WORLDTOUNREALUNIT);
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

    ret = CommandManager.GetValue(CCLICommandManager::SensorSlotIndex, sint);
    if (ret) {
        if (pSystemAPI->SetSlotIndex(p_actor, sint)) {
            return true;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::HorizontalFov, dbl);
    if (ret) {
        if (pSystemAPI->SetHorizontalFov(p_actor, dbl)) {
            return true;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::VericalFov, dbl);
    if (ret) {
        if (pSystemAPI->SetVerticalFov(p_actor, dbl)) {
            return true;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::HorizontalScanStepAngleDeg, dbl);
    if (ret) {
        if (pSystemAPI->SetHorizontalScanStepAngleDeg(p_actor, dbl)) {
            return true;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::VerticalScanStepAngleDeg, dbl);
    if (ret) {
        if (pSystemAPI->SetVerticalScanStepAngleDeg(p_actor, dbl)) {
            return true;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::MeasurementErrorMean, dbl);
    if (ret) {
        if (pSystemAPI->SetMeasurementErrorMean(p_actor, dbl)) {
            return true;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::MeasurementErrorStd, dbl);
    if (ret) {
        if (pSystemAPI->SetMeasurementErrorStd(p_actor, dbl)) {
            return true;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::EnableSurfaceDetect, is_enabled);
    if (ret) {
        if (pSystemAPI->SetEnableSurfaceDetect(p_actor, is_enabled)) {
            return true;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::EnableSubsurfaceDetect, is_enabled);
    if (ret) {
        if (pSystemAPI->SetEnableSubsurfaceDetect(p_actor, is_enabled)) {
            return true;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::EnableFoamDetect, is_enabled);
    if (ret) {
        if (pSystemAPI->SetEnableFoamDetect(p_actor, is_enabled)) {
            return true;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::SeaSurfaceDetectionProb, dbl);
    if (ret) {
        if (pSystemAPI->SetSeaSurfaceDetectionProb(p_actor, dbl)) {
            return true;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::MaxSurfacePenetration, dbl);
    if (ret) {
        if (pSystemAPI->SetMaxSurfacePenetration(p_actor, dbl)) {
            return true;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::RadarScanLevel, dbl);
    if (ret) {
        if (pSystemAPI->SetRadarScanLevel(p_actor, dbl)) {
            return true;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::RadarScannerRPM, dbl);
    if (ret) {
        if (pSystemAPI->SetRadarScannerRPM(p_actor, dbl)) {
            return true;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::RadarGainType, sint);
    if (ret) {
        if (pSystemAPI->SetRadarGainType(p_actor, sint)) {
            return true;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::RadarGainLevel, sint);
    if (ret) {
        if (pSystemAPI->SetRadarGainLevel(p_actor, sint)) {
            return true;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::RadarSeaClutterType, sint);
    if (ret) {
        if (pSystemAPI->SetRadarSeaClutterType(p_actor, sint)) {
            return true;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::RadarSeaClutterLevel, sint);
    if (ret) {
        if (pSystemAPI->SetRadarSeaClutterLevel(p_actor, sint)) {
            return true;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::RadarSeaClutterAutoOffset, sint);
    if (ret) {
        if (pSystemAPI->SetRadarSeaClutterAutoOffset(p_actor, sint)) {
            return true;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::RadarRainClutterLevel, sint);
    if (ret) {
        if (pSystemAPI->SetRadarRainClutterLevel(p_actor, sint)) {
            return true;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::RadarMaxGuardZoneCount, sint);
    if (ret) {
        if (pSystemAPI->SetRadarMaxGuardZoneCount(p_actor, sint)) {
            return true;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::RadarMaxSectorBlankingZoneCount, sint);
    if (ret) {
        if (pSystemAPI->SetRadarMaxSectorBlankingZoneCount(p_actor, sint)) {
            return true;
        }
    }
    else {

    }

    

    return true;
}
bool UConsoleBase::ProcessGetCommand(TMap<FString, FString>& options, FString& error_message)
{

    error_message = "";
    FVector2D vec2d;
    bool ret;
    FString sret;
    FString name;
    AActor* p_actor;
    FVector vec;
    bool is_enabled;
    INT32S sint;
    FLOAT64 dbl;



    ret = CommandManager.HasController();
    if (ret) {
        auto player_controller = ASystemManagerBase::GetInstance()->GetMainPlayerController();
        if (player_controller != nullptr) {
            pSystemAPI->SendConsoleResponse("Possed: " + player_controller->GetName());
        }
        else {
            pSystemAPI->SendConsoleResponse("possed: nullptr (world)");
        }

        return true;
    }


    ret = CommandManager.GetValue(CCLICommandManager::Selected, sret);
    if (ret) {
        auto selected = pSystemAPI->GetSelectedActor();

        if (selected) {
            SendConsoleResponse(CCLICommandManager::Selected, selected->GetName());
            return true;
        }
        else {
            SendConsoleResponse(CCLICommandManager::Selected, FString("none"));
            return true;
        }
    }
    else {
        
       
    }

    name = CommandManager.GetName();
    if (name != "") {

    }
    else {
        error_message += "empty name value";
        return false;
    }

    if (name == "*") {
        auto all_actors = ASystemManagerBase::GetInstance()->GetAllActorInWorld();
        for (auto p_act : all_actors)
        {

            pSystemAPI->SendConsoleResponse("Actor: " + p_act->GetName());
        }
        return true;
    }

    p_actor = ASystemManagerBase::GetInstance()->FindActor(name);


    if (p_actor == nullptr) {
        error_message += (("cannot find actor "));
        return false;
    }

    int instance_no = -1;
    ret = CommandManager.GetInstance(instance_no);
    if (ret) {
        pSystemAPI->SendConsoleResponse("Instance: " + CUtil::IntToString(instance_no));
        return true;

    }

    ret = CommandManager.HasPosition();
    if (ret) {
        pSystemAPI->SendConsoleResponse("Pos: " + p_actor->GetActorLocation().ToString());
        return true;

    }

    ret = CommandManager.HasRelposition();
    if (ret) {
        pSystemAPI->SendConsoleResponse("Relpos: " + p_actor->GetActorLocation().ToString());
        return true;

    }

    ret = CommandManager.HasRotation();
    if (ret) {

        vec = CMath::GetActorEulerAnglesRPY(p_actor);
        pSystemAPI->SendConsoleResponse("Rot: " + vec.ToString());
        return true;
    }
    ret = CommandManager.HasRelrotation();
    if (ret) {
        vec = CMath::GetActorReleativeEulerAnglesRPY(p_actor);
        pSystemAPI->SendConsoleResponse("Relrot: " + vec.ToString());
        return true;
    }

    ret = CommandManager.HasScale();
    if (ret) {
        vec = p_actor->GetActorScale();;
        pSystemAPI->SendConsoleResponse("Scale: " + vec.ToString());
        return true;
    }

    ret = CommandManager.HasEnabled();
    if (ret) {
        is_enabled = pSystemAPI->GetActorEnabled(p_actor);
        FString str = is_enabled ? "1" : "0";
        pSystemAPI->SendConsoleResponse("Enabled: " + is_enabled);
        return true;
    }

    ret = CommandManager.HasParent();
    if (ret) {
        auto parent = CUtil::GetParentActor(p_actor);
        if (parent != nullptr) {
            pSystemAPI->SendConsoleResponse("Parent: " + parent->GetName());
        }
        else {
            pSystemAPI->SendConsoleResponse("Parent: nullptr (world)");
        }

        return true;
    }



    ret = CommandManager.HasA(CCLICommandManager::SensorSlotIndex);
    if (ret) {

        if (pSystemAPI->GetSlotIndex(p_actor, sint)) {
            SendConsoleResponse(CCLICommandManager::SensorSlotIndex, sint);
            return true;
        }
    }
    else {

    }


    ret = CommandManager.HasA(CCLICommandManager::HorizontalFov);
    if (ret) {
        if (pSystemAPI->GetHorizontalFov(p_actor, dbl)) {
            SendConsoleResponse(CCLICommandManager::HorizontalFov, dbl);
            return true;
        }
    }
    else {

    }

    ret = CommandManager.HasA(CCLICommandManager::VericalFov);
    if (ret) {
        if (pSystemAPI->GetVerticalFov(p_actor, dbl)) {
            SendConsoleResponse(CCLICommandManager::VericalFov, dbl);
            return true;
        }
    }
    else {

    }

    ret = CommandManager.HasA(CCLICommandManager::HorizontalScanStepAngleDeg);
    if (ret) {
        if (pSystemAPI->GetHorizontalScanStepAngleDeg(p_actor, dbl)) {
            SendConsoleResponse(CCLICommandManager::HorizontalScanStepAngleDeg, dbl);
            return true;
        }
    }
    else {

    }

    ret = CommandManager.HasA(CCLICommandManager::VerticalScanStepAngleDeg);
    if (ret) {
        if (pSystemAPI->SetVerticalScanStepAngleDeg(p_actor, dbl)) {
            SendConsoleResponse(CCLICommandManager::VerticalScanStepAngleDeg, dbl);
            return true;
        }
    }
    else {

    }

    ret = CommandManager.HasA(CCLICommandManager::MeasurementErrorMean);
    if (ret) {
        if (pSystemAPI->GetMeasurementErrorMean(p_actor, dbl)) {
            SendConsoleResponse(CCLICommandManager::MeasurementErrorMean, dbl);
            return true;
        }
    }
    else {

    }

    ret = CommandManager.HasA(CCLICommandManager::MeasurementErrorStd);
    if (ret) {
        if (pSystemAPI->GetMeasurementErrorStd(p_actor, dbl)) {
            SendConsoleResponse(CCLICommandManager::MeasurementErrorStd, dbl);
            return true;
        }
    }
    else {

    }

    ret = CommandManager.HasA(CCLICommandManager::EnableSurfaceDetect);
    if (ret) {
        if (pSystemAPI->GetEnableSurfaceDetect(p_actor, is_enabled)) {
            SendConsoleResponse(CCLICommandManager::EnableSurfaceDetect, is_enabled);
            return true;
        }
    }
    else {

    }

    ret = CommandManager.HasA(CCLICommandManager::EnableSubsurfaceDetect);
    if (ret) {
        if (pSystemAPI->GetEnableSubsurfaceDetect(p_actor, is_enabled)) {
            SendConsoleResponse(CCLICommandManager::EnableSubsurfaceDetect, is_enabled);
            return true;
        }
    }
    else {

    }

    ret = CommandManager.HasA(CCLICommandManager::EnableFoamDetect);
    if (ret) {
        if (pSystemAPI->GetEnableFoamDetect(p_actor, is_enabled)) {
            SendConsoleResponse(CCLICommandManager::EnableFoamDetect, is_enabled);
            return true;
        }
    }
    else {

    }

    ret = CommandManager.HasA(CCLICommandManager::SeaSurfaceDetectionProb);
    if (ret) {
        if (pSystemAPI->GetSeaSurfaceDetectionProb(p_actor, dbl)) {
            SendConsoleResponse(CCLICommandManager::EnableFoamDetect, dbl);
            return true;
        }
    }
    else {

    }

    ret = CommandManager.HasA(CCLICommandManager::MaxSurfacePenetration);
    if (ret) {
        if (pSystemAPI->GetMaxSurfacePenetration(p_actor, dbl)) {
            SendConsoleResponse(CCLICommandManager::MaxSurfacePenetration, dbl);
            return true;
        }
    }
    else {

    }

    ret = CommandManager.HasA(CCLICommandManager::RadarScanLevel);
    if (ret) {
        if (pSystemAPI->GetRadarScanLevel(p_actor, sint)) {
            SendConsoleResponse(CCLICommandManager::RadarScanLevel, sint);
            return true;
        }
    }
    else {

    }

    ret = CommandManager.HasA(CCLICommandManager::RadarScannerRPM);
    if (ret) {
        if (pSystemAPI->GetRadarScannerRPM(p_actor, sint)) {
            SendConsoleResponse(CCLICommandManager::RadarScannerRPM, sint);
            return true;
        }
    }
    else {

    }

    ret = CommandManager.HasA(CCLICommandManager::RadarGainType);
    if (ret) {
        if (pSystemAPI->GetRadarGainType(p_actor, sint)) {
            SendConsoleResponse(CCLICommandManager::RadarGainType, sint);
            return true;
        }
    }
    else {

    }

    ret = CommandManager.HasA(CCLICommandManager::RadarGainLevel);
    if (ret) {
        if (pSystemAPI->GetRadarGainLevel(p_actor, sint)) {
            SendConsoleResponse(CCLICommandManager::RadarGainLevel, sint);
            return true;
        }
    }
    else {

    }

    ret = CommandManager.HasA(CCLICommandManager::RadarSeaClutterType);
    if (ret) {
        if (pSystemAPI->GetRadarSeaClutterType(p_actor, sint)) {
            SendConsoleResponse(CCLICommandManager::RadarSeaClutterType, sint);
            return true;
        }
    }
    else {

    }

    ret = CommandManager.HasA(CCLICommandManager::RadarSeaClutterLevel);
    if (ret) {
        if (pSystemAPI->GetRadarSeaClutterLevel(p_actor, sint)) {
            SendConsoleResponse(CCLICommandManager::RadarSeaClutterLevel, sint);
            return true;
        }
    }
    else {

    }

    ret = CommandManager.HasA(CCLICommandManager::RadarSeaClutterAutoOffset);
    if (ret) {
        if (pSystemAPI->GetRadarSeaClutterAutoOffset(p_actor, sint)) {
            SendConsoleResponse(CCLICommandManager::RadarSeaClutterAutoOffset, sint);
            return true;
        }
    }
    else {

    }

    ret = CommandManager.HasA(CCLICommandManager::RadarRainClutterLevel);
    if (ret) {
        if (pSystemAPI->GetRadarRainClutterLevel(p_actor, sint)) {
            SendConsoleResponse(CCLICommandManager::RadarRainClutterLevel, sint);
            return true;
        }
    }
    else {

    }

    ret = CommandManager.HasA(CCLICommandManager::RadarMaxGuardZoneCount);
    if (ret) {
        if (pSystemAPI->GetRadarMaxGuardZoneCount(p_actor, sint)) {
            SendConsoleResponse(CCLICommandManager::RadarMaxGuardZoneCount, sint);
            return true;
        }
    }
    else {

    }

    ret = CommandManager.HasA(CCLICommandManager::RadarMaxSectorBlankingZoneCount);
    if (ret) {
        if (pSystemAPI->GetRadarMaxSectorBlankingZoneCount(p_actor, sint)) {
            SendConsoleResponse(CCLICommandManager::RadarMaxSectorBlankingZoneCount, sint);
            return true;
        }
    }
    else {

    }

 

    return true;
}

bool UConsoleBase::ProcessWorkspaceCommand(TMap<FString, FString>& options, FString& error_message)
{
    bool ret;

    CommandManager.SetCommandOptions(&options);
    FString path;

    ret = CommandManager.GetValue(CCLICommandManager::LoadFile, path);
    if (ret) {
        if (pSystemAPI->Load(path)) {
            return true;
        }
    }
    else {
        
    }

    ret = CommandManager.GetValue(CCLICommandManager::SaveFile, path);
    if (ret) {
        if (pSystemAPI->Save(path)) {
            return true;
        }
    }
    else {
        
    }


    return false;

}

bool UConsoleBase::ProcessCommands(FString command, TMap<FString, FString>& options, FString& error_message)
{



    CommandManager.SetCommandOptions(&options);

    if (command == "help") {
        return ProcessHelpCommand(options, error_message);
    }
    else if (command == CommandManager.WorkspaceCommand) {

        return ProcessWorkspaceCommand(options, error_message);

    }
    else if (command == CommandManager.ProcessCommand) {

        return ProcessProcessCommand(options, error_message);

    }
    else if (command == CommandManager.PrintCommand) {

        return ProcessPrintCommand(options, error_message);
    }
    else if (command == CommandManager.SimCommand) {
        return ProcessSimCommand(options, error_message);

    }
    else if (command == CommandManager.CreateCommand) {
        return  ProcessCreateCommand(options, error_message);
    }
    else if (command == CommandManager.DestroyCommand) {
        return ProcessDestroyCommand(options, error_message);
    }
    else if (command == CommandManager.SetCommand) {
        return ProcessSetCommand(options, error_message);
    }
    else if (command == CommandManager.GetCommand) {
        return ProcessGetCommand(options, error_message);
    }



    return false;

}

