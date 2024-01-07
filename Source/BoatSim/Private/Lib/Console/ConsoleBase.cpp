// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/Console/ConsoleBase.h"
#include <Lib/Utils/CUtil.h>
#include <Lib/SystemManager/SystemManagerBase.h>
#include <Lib/Gimbal/GimbalBase.h>
#include <Lib/Sensor/GenericSensor/CameraBase.h>
#include <Lib/Sensor/GenericSensor/RadarBase.h>
#include <Lib/Math/CMath.h>
#include "Dom/JsonObject.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonSerializer.h"

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

void UConsoleBase::SendActorTransform(AActor* p_actor)
{
    FVector vec;

    if (p_actor) {
        SendConsoleResponse(p_actor->GetName(), CCLICommandManager::Position, TOW(p_actor->GetActorLocation()));

        vec = CMath::GetActorEulerAnglesRPY(p_actor);
        SendConsoleResponse(p_actor->GetName(), CCLICommandManager::Rotation, vec);

        SendConsoleResponse(p_actor->GetName(), CCLICommandManager::Position, p_actor->GetActorScale3D());
    }

}

void UConsoleBase::Command(FString command, int& delay_tick)
{

  
    TArray<FString> CommandTokens;

    TMap<FString, FString> outopt;
    FString outcommand;
    FString out_error_message;

    bool ret = ParseCommandLine(command.GetCharArray().GetData(), outcommand, outopt, out_error_message);

    if (ret) {
        FString command_process_error_message = "";
        auto command_ret = ProcessCommands(outcommand, outopt, command_process_error_message, delay_tick);

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
            int tick_delay = 0;
            Command(cmd, tick_delay);

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





FString UConsoleBase::CreateAndSerializeJson(TMap<FString,FString> &data)
{
    // Create a new JSON object
    TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);

    // Add some simple data
  
    
    // Iterate over all key-value pairs in the map
    for (const TPair<FString, FString>& pair : data)
    {
        JsonObject->SetStringField(pair.Key, pair.Value);
    }
   

    
    FString OutputString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
    FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

    return OutputString;
}



void UConsoleBase::SendConsoleResponse(AActor* p_actor)
{
    auto p_base = pSystemAPI->ToActorBase(p_actor);
    CJsonDataContainer json_data;
    if (p_base) {
        p_base->SaveJSON(json_data);
        auto ret_json = CreateAndSerializeJson(json_data.Data);
        pSystemAPI->SendConsoleResponse(ret_json);
    }
}

void UConsoleBase::SendConsoleResponse(FString name, FString option, INT32S ret)
{
    TMap<FString, FString> data;

    if (name != "") {
        data.Add("name", name);
    }
    
    data.Add("option", option);
    data.Add("value", CUtil::IntToString(ret));
    auto ret_json = CreateAndSerializeJson(data);
    pSystemAPI->SendConsoleResponse(ret_json);
    //pSystemAPI->SendConsoleResponse("--name "+name+" --"+option + " " + CUtil::IntToString(ret));
    
}
void UConsoleBase::SendConsoleResponse(FString name, FString option, FLOAT64 ret)
{
    TMap<FString, FString> data;

    if (name != "") {
        data.Add("name", name);
    }

    data.Add("option", option);
    data.Add("value", CUtil::FloatToString(ret));
    auto ret_json = CreateAndSerializeJson(data);
    pSystemAPI->SendConsoleResponse(ret_json);

    //pSystemAPI->SendConsoleResponse("--name " + name + " --" + option + " " + CUtil::FloatToString(ret));
}
void UConsoleBase::SendConsoleResponse(FString name, FString option, BOOLEAN ret)
{

    TMap<FString, FString> data;

    if (name != "") {
        data.Add("name", name);
    }

    data.Add("option", option);
    data.Add("value", CUtil::BoolToStringBinary(ret));
    auto ret_json = CreateAndSerializeJson(data);
    pSystemAPI->SendConsoleResponse(ret_json);
    //pSystemAPI->SendConsoleResponse("--name " + name + " --" + option + " " + CUtil::BoolToStringBinary(ret));
}

void UConsoleBase::SendConsoleResponse(FString name, FString option, FVector2D ret)
{
    FString retstr = CUtil::FloatToString(ret.X) + " " + CUtil::FloatToString(ret.Y);

    TMap<FString, FString> data;

    if (name != "") {
        data.Add("name", name);
    }

    data.Add("option", option);
    data.Add("value", (retstr));
    auto ret_json = CreateAndSerializeJson(data);
    pSystemAPI->SendConsoleResponse(ret_json);

    //pSystemAPI->SendConsoleResponse("--name " + name + " --" + option + " " + "\""+ retstr + "\"");
}

void UConsoleBase::SendConsoleResponse(FString name, FString option, FColor ret)
{
    FString retstr = CUtil::IntToString(ret.R) + " " + CUtil::IntToString(ret.G) + " " + CUtil::IntToString(ret.B) + " " + CUtil::IntToString(ret.A);

    TMap<FString, FString> data;

    if (name != "") {
        data.Add("name", name);
    }

    data.Add("option", option);
    data.Add("value", (retstr));
    auto ret_json = CreateAndSerializeJson(data);
    pSystemAPI->SendConsoleResponse(ret_json);
}
void UConsoleBase::SendConsoleResponse(FString name, FString option, FVector4 ret)
{
    FString retstr = CUtil::FloatToString(ret.X) + " " + CUtil::FloatToString(ret.Y) + " " + CUtil::FloatToString(ret.Z) + " " + CUtil::FloatToString(ret.W);

    TMap<FString, FString> data;

    if (name != "") {
        data.Add("name", name);
    }

    data.Add("option", option);
    data.Add("value", (retstr));
    auto ret_json = CreateAndSerializeJson(data);
    pSystemAPI->SendConsoleResponse(ret_json);
}

void UConsoleBase::SendConsoleResponse(FString name, FString option, INT32S ind, FVector ret)
{
    FString retstr = CUtil::FloatToString(ret.X) + " " + CUtil::FloatToString(ret.Y);

    TMap<FString, FString> data;

    if (name != "") {
        data.Add("name", name);
    }

    data.Add("option", option);
    data.Add("ind", CUtil::IntToString(ind));
    data.Add("value", (retstr));
    auto ret_json = CreateAndSerializeJson(data);
    pSystemAPI->SendConsoleResponse(ret_json);

    //pSystemAPI->SendConsoleResponse("--name " + name + " --" + option + " " + "\""+ retstr + "\"");
}
void UConsoleBase::SendConsoleResponse(FString name, FString option, FString prop, FString val)
{
   
    TMap<FString, FString> data;

    if (name != "") {
        data.Add("name", name);
    }

    data.Add("option", option);
    data.Add("property", prop);
    data.Add("value", val);
    auto ret_json = CreateAndSerializeJson(data);
    pSystemAPI->SendConsoleResponse(ret_json);
}

void UConsoleBase::SendConsoleResponse(FString name, FString option, FVector ret)
{
    FString retstr = CUtil::FloatToString(ret.X) + " " + CUtil::FloatToString(ret.Y) + " " + CUtil::FloatToString(ret.Z);

    TMap<FString, FString> data;

    if (name != "") {
        data.Add("name", name);
    }

    data.Add("option", option);
    data.Add("value", (retstr));
    auto ret_json = CreateAndSerializeJson(data);
    pSystemAPI->SendConsoleResponse(ret_json);

    //pSystemAPI->SendConsoleResponse("--name " + name + " --" + option + " " + "\""+ retstr + "\"");
}

void UConsoleBase::SendConsoleResponse(FString name, FString option, FString ret)
{
    TMap<FString, FString> data;

    if (name != "") {
        data.Add("name", name);
    }

    data.Add("option", option);
    data.Add("value", (ret));
    auto ret_json = CreateAndSerializeJson(data);
    pSystemAPI->SendConsoleResponse(ret_json);
    //pSystemAPI->SendConsoleResponse(option + " :" + ret);
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
        SendBlueprints();

        return true;
    }
    auto print_actors = CommandManager.HasActors();
    if (print_actors) {
        SendActors();
        return true;
    }
   
    if (CommandManager.HasA(CCLICommandManager::ActorBases)) {
        SendActorBases();
        return true;
    }

    if (CommandManager.HasA(CCLICommandManager::ActorBases)) {
        SendActorBases();
        return true;
    }

    auto has_sensors = CommandManager.HasSensors();
    if (has_sensors) {
        SendSensors();
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
void UConsoleBase::SendSensors()
{
    auto all_sensors = pSystemAPI->GetAllSensors();
    for (auto p_sensor : all_sensors)
    {
        pSystemAPI->SendConsoleResponse("<sensor>" + p_sensor->GetName());
    }
}
void UConsoleBase::SendActors()
{
    
#if true
    auto all_actors = ASystemManagerBase::GetInstance()->GetAllActorInWorld();
    for (auto p_act : all_actors)
    {
       if (p_act->IsA<AActorBase>()) {
            continue;
       }
       auto full_name =  p_act->GetFullName();
       TArray<FString> hier;
       CUtil::GetActorHierarchy(p_act, hier);
       FString path = "";
       for (INT32S i = hier.Num() - 1; i >= 0; i--) {
           path += hier[i];
           if (i != 0) {
               path += "/";
           }
           
       }

        pSystemAPI->SendConsoleResponse("<actor>" + path);
    }
#endif

    SendActorBases();
}

void UConsoleBase::SendActorBases()
{
    TArray<AActor*> all_actors;
    ASystemManagerBase::GetInstance()->QueryActors(EActorQueryArgs::ActorBases, all_actors);
    ASystemManagerBase::GetInstance()->QueryActors(EActorQueryArgs::Platforms, all_actors);

    FString path = "";

#if true
    for (auto p_act : all_actors)
    {
        auto full_name = p_act->GetFullName();
        TArray<FString> hier;
        TArray<FString> hiercomp;
        CUtil::GetActorHierarchy(p_act, hier);
        //CUtil::GetComponentHierarchy(p_act->GetRootComponent(), hiercomp);
        //CUtil::GetActorAndComponentHierarchy(p_act, hierall);
        path = "";
        for (INT32S i = hier.Num() - 1; i >= 0; i--) {
            path += hier[i];
            if (i != 0) {
                path += "/";
            }

        }
        pSystemAPI->SendConsoleResponse("<actor>" + path);

    }
#endif


#if false
    for (auto p_act : all_actors)
    {
        if (CUtil::GetParentActor(p_act) != nullptr) {
            continue; // its info will we sent owners paths
        }
        TArray<FString> actor_comp_paths = CUtil::GetAllComponentPaths(p_act);

        for (auto comp_path : actor_comp_paths) {
            pSystemAPI->SendConsoleResponse("<actor>" + comp_path);
        }

#endif
#if false
        TArray<USceneComponent*> static_mesh_componnets;
        p_act->GetComponents<USceneComponent>(static_mesh_componnets);
        USceneComponent* p_root = p_act->GetRootComponent();
        
        if (!static_mesh_componnets.Contains(p_root)) {
            static_mesh_componnets.Insert(p_root, 0);
        }

        for (auto p_comp : static_mesh_componnets) {
            TArray<UActorComponent*> ret = CUtil::GetComponentHierarchyToTop(p_comp);

            path = p_act->GetName()+"/";
            
            for (INT32S i = ret.Num() - 1; i >= 0; i--) {
   
                path += ret[i]->GetName();
                if (i != 0) {
                    path += "/";
                }
                if (CUtil::IsDefaultSceneComponent((USceneComponent*)ret[i])) {
                    if (ret[i]->GetOwner() != p_act) {
                        path += ret[i]->GetOwner()->GetName();

                        if (i != 0) {
                            path += "/";
                        }
                    }
                }


            }
            


            
            

        }
    
  


       
    }
#endif
    TArray<AActor*> paths;
    ASystemManagerBase::GetInstance()->QueryActors(EActorQueryArgs::ActorBasesOnlyPaths, paths);


    for (auto p_act : paths)
    {
        pSystemAPI->SendConsoleResponse("<path>" + p_act->GetName());
    }

    pSystemAPI->SendConsoleResponse("<allsent>");
}

void UConsoleBase::SendFunctions(AActor* p_actor, FString category)
{
    auto ret = GetCallableFunctions(p_actor, category);

    for (FString func : ret) {
        pSystemAPI->SendConsoleResponse("<func>" + func);
    }
}
void UConsoleBase::SendProperies(AActor* p_actor)
{
    auto ret = GetProperties(p_actor);
    for (FString prop : ret) {
        pSystemAPI->SendConsoleResponse("<property>" + prop);
    }

}
void UConsoleBase::SendBlueprints()
{
    auto bp_info = ASystemManagerBase::GetInstance()->GetDataManager()->GetBlueprintInfo();

    for (auto entry : *bp_info) {

        //pSystemAPI->SendConsoleResponse("<bp>" + entry.Name);
        TMap<FString, FString> data;
        data.Add("class", "bp");
        if (entry.Name != "") {
            data.Add("name", entry.Name);
        }

        data.Add("maincategory", ASystemManagerBase::GetInstance()->GetDataManager()->GetCategoryName(entry.Category));
        auto ret_json = CreateAndSerializeJson(data);
        pSystemAPI->SendConsoleResponse(ret_json);

    }


}
bool UConsoleBase::SetCamView(FString cmd)
{
    if (cmd == "top") {
        return pSystemAPI->SetCamView(ECamView::CamViewTop);
    
    }else if (cmd == "right") {
        return pSystemAPI->SetCamView(ECamView::CamViewRight);
     
    }else if (cmd == "left") {
        return pSystemAPI->SetCamView(ECamView::CamViewLeft);
   
    }
    else if (cmd == "front") {
        return pSystemAPI->SetCamView(ECamView::CamViewFront);

    }
    else if (cmd == "back") {
        return pSystemAPI->SetCamView(ECamView::CamViewBack);
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

bool UConsoleBase::ProcessCreateCommand(TMap<FString, FString>& options, FString& error_message, INT32S &delay_tick)
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
        FString parent="", fullpath="";

        CommandManager.GetValue(CCLICommandManager::Parent, parent);
        CommandManager.GetValue(CCLICommandManager::FullPath, fullpath);
        CommandManager.GetValue(CCLICommandManager::Tick, delay_tick);
        auto actor_created = ASystemManagerBase::GetInstance()->CreateActor(bp, name, FVector::ZeroVector, FVector::ZeroVector, FVector::OneVector);
        if (actor_created == nullptr) {
            error_message = "object cannot created";
            return false;
        }
        SetParent(actor_created, parent, fullpath);

        SendActorBases();

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
            SendActorBases();
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

bool UConsoleBase::SetParent(AActor *p_actor, FString parent, FString full_path)
{
    if (full_path != "" && parent != full_path) {
        auto p_comp = ASystemManagerBase::GetInstance()->FindComponent(full_path);
        bool ret_parenting = CUtil::SetParentComponent(p_actor, p_comp);
        return true;
    }
    else {
        if (parent == "null") {
            
            p_actor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
        }
        else {

            AActor* p_parent = ASystemManagerBase::GetInstance()->FindActor(parent);
            CUtil::SetParent(p_actor, p_parent);
        }
        return true;
    }
}

bool UConsoleBase::ProcessSetCommand(TMap<FString, FString>& options, FString& error_message)
{
    error_message = "";
    FVector2D vec2d;
    FColor color;
    bool ret;
    FString sret;
    FString name;
    AActor* p_actor;
    USceneComponent* p_scene_comp;
    FVector vec;
    bool is_enabled;
    bool is_active;
    INT32S sint;
    FLOAT64 dbl;
    SConnectionInfo conn;


    FString controller;
    ret = CommandManager.GetContoroller(controller);

    if (ret) {
        bool ctrl_ret = ASystemManagerBase::GetInstance()->SetMainPlayerController(controller);
        return ctrl_ret;
    }

    ret = CommandManager.GetValue(CCLICommandManager::UIVisible, is_enabled);
    if (ret) {
        if (pSystemAPI->SetIsUIVisible(is_enabled)) {
            return true;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::CamSpeed, dbl);
    if (ret) {
        if (pSystemAPI->SetCamSpeed(nullptr, dbl)) {
            return true;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::CamView, sret);
    if (ret) {
        if (SetCamView(sret)) {
            return true;
        }
    }
    else {

    }
    ret = CommandManager.GetValue(CCLICommandManager::PlatformSpeed, dbl);
    if (ret) {
        if (pSystemAPI->SetSpeedMeterPerSec(nullptr, dbl)) {
            return true;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::PlatformTurnRate, dbl);
    if (ret) {
        if (pSystemAPI->SetTurnRateDegPerSec(nullptr, dbl)) {
            return true;
        }
    }
    else {

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

    int region = 0;
    bool one_success = false;

    if (name == "env") {

        ret = CommandManager.GetValue(CCLICommandManager::TimeOfDayHour, dbl);
        if (ret) {
            if (pSystemAPI->SetTimeOfDayHr(region, dbl)) {
                one_success = true;
            }
        }
        else {

        }

        ret = CommandManager.GetValue(CCLICommandManager::EnvTimeFlowScale, dbl);
        if (ret) {
            if (pSystemAPI->SetEnvTimeFlowScale(region, dbl)) {
                one_success = true;
            }
        }
        else {

        }

        ret = CommandManager.GetValue(CCLICommandManager::RainPercent, dbl);
        if (ret) {
            if (pSystemAPI->SetRainPercent(region, dbl)) {
                one_success = true;
            }
        }
        else {

        }


        ret = CommandManager.GetValue(CCLICommandManager::CloudPercent, dbl);
        if (ret) {
            if (pSystemAPI->SetCloudPercent(region, dbl)) {
                one_success = true;
            }
        }
        else {

        }

        ret = CommandManager.GetValue(CCLICommandManager::FogPercent, dbl);
        if (ret) {
            if (pSystemAPI->SetFogPercent(region, dbl)) {
                one_success = true;
            }
        }
        else {

        }

        ret = CommandManager.GetValue(CCLICommandManager::SnowPercent, dbl);
        if (ret) {
            if (pSystemAPI->SetRainPercent(region, dbl)) {
                one_success = true;
            }
        }
        else {

        }

        ret = CommandManager.GetValue(CCLICommandManager::DustPercent, dbl);
        if (ret) {
            if (pSystemAPI->SetDustPercent(region, dbl)) {
                one_success = true;
            }
        }
        else {

        }

        ret = CommandManager.GetValue(CCLICommandManager::ThunderPercent, dbl);
        if (ret) {
            if (pSystemAPI->SetThunderPercent(region, dbl)) {
                one_success = true;
            }
        }
        else {

        }


        ret = CommandManager.GetValue(CCLICommandManager::EnvSoundFxEnabled, is_enabled);
        if (ret) {
            if (pSystemAPI->SetEnvSoundEnabled(region, is_enabled)) {
                one_success = true;
            }
        }
        else {

        }

        ret = CommandManager.GetValue(CCLICommandManager::WindSpeed, dbl);
        if (ret) {
            if (pSystemAPI->SetWindSpeedMeterPerSec(region, dbl)) {
                one_success = true;
            }
        }
        else {

        }

        ret = CommandManager.GetValue(CCLICommandManager::WindDirection, dbl);
        if (ret) {
            if (pSystemAPI->SetWindDirectionDeg(region, dbl)) {
                one_success = true;
            }
        }
        else {

        }



        return one_success;
    }
    p_actor = ASystemManagerBase::GetInstance()->FindActor(name);
    
    if (p_actor == nullptr) {
        p_scene_comp = ASystemManagerBase::GetInstance()->FindComponent(name);
        if (p_scene_comp != nullptr) {
            p_actor = p_scene_comp->GetOwner();
        }
    }
    if (p_actor == nullptr ) {
        error_message += (("cannot find actor "));
        return false;
    }


    { // terrain command
        
        ret = CommandManager.GetValue(CCLICommandManager::TerrHMap, sret);
        if (ret) {
            if (pSystemAPI->SetTerrainHeightMap(p_actor, sret)) {
                one_success = true;
            }
        }
        else {

        }

        ret = CommandManager.GetValue(CCLICommandManager::TerrDMap, sret);
        if (ret) {
            if (pSystemAPI->SetTerrainDepthMap(p_actor, sret)) {
                one_success = true;
            }
        }
        else {

        }

        ret = CommandManager.GetValue(CCLICommandManager::TerrBaseTexture, sret);
        if (ret) {
            if (pSystemAPI->SetTerraiBaseTexture(p_actor, sret)) {
                one_success = true;
            }
        }
        else {

        }

        ret = CommandManager.GetValue(CCLICommandManager::TerrHMapMinLvlMt, dbl);
        if (ret) {
            if (pSystemAPI->SetTerrainHeightMapMinLevelMeter(p_actor, dbl)) {
                one_success = true;
            }
        }
        else {

        }

        ret = CommandManager.GetValue(CCLICommandManager::TerrHMapMaxLvlMt, dbl);
        if (ret) {
            if (pSystemAPI->SetTerrainHeightMapMaxLevelMeter(p_actor, dbl)) {
                one_success = true;
            }
        }
        else {

        }


        ret = CommandManager.GetValue(CCLICommandManager::TerrDMapMinLvlMt, dbl);
        if (ret) {
            if (pSystemAPI->SetTerrainDepthMapMinLevelMeter(p_actor, dbl)) {
                one_success = true;
            }
        }
        else {

        }

  

        ret = CommandManager.GetValue(CCLICommandManager::TerrDMapMaxLvlMt, dbl);
        if (ret) {
            if (pSystemAPI->SetTerrainDepthMapMaxLevelMeter(p_actor, dbl)) {
                one_success = true;
            }
        }
        else {

        }

        
        ret = CommandManager.GetValue(CCLICommandManager::TerrTopLeftLLH, vec);
        if (ret) {
            if (pSystemAPI->SetTerrainTopLeftCornerCoordLLH(p_actor, vec)) {
                one_success = true;
            }
        }
        else {

        }
        ret = CommandManager.GetValue(CCLICommandManager::TerrBottomRightLLH, vec);
        if (ret) {
            if (pSystemAPI->SetTerrainBottomRightCornerCoordLLH(p_actor, vec)) {
                one_success = true;
            }
        }
        else {

        }

    }



   


    ret = CommandManager.GetValue(CCLICommandManager::StandaloneModeEnabled, is_enabled);
    if (ret) {
        if (pSystemAPI->SetActorStandaloneModeEnabled(p_actor, is_enabled)) {
            one_success = true;
        }

    }

   
    ret = CommandManager.GetValue(CCLICommandManager::Suppressed, is_enabled);
    if (ret) {
        if (pSystemAPI->SetActorSuppressed(p_actor, is_enabled)) {
            one_success = true;
        }

    }

    ret = CommandManager.GetActive(is_active);
    if (ret) {
        CUtil::SetActorActive(p_actor, is_active);
        one_success =  true;

    }


    ret = CommandManager.GetEnabled(is_enabled);
    if (ret) {
        pSystemAPI->SetActorEnabled(p_actor, is_enabled);
        one_success = true;

    }

    int  instance_no = -1;
    ret = CommandManager.GetInstance(instance_no);
    if (ret) {
        pSystemAPI->SetActorInstanceNo(p_actor, instance_no);
        one_success = true;

    }

    ret = CommandManager.GetPosition(vec);
    if (ret) {
        pSystemAPI->SetActorPosition(p_actor, TOUE(vec));
        //p_actor->SetActorLocation(vec * WORLDTOUNREALUNIT);
        one_success = true;
    }

    ret = CommandManager.GetRelPosition(vec);
    if (ret) {
        pSystemAPI->SetActorRelPosition(p_actor, TOUE(vec ));
        //p_actor->SetActorRelativeLocation(vec * WORLDTOUNREALUNIT);
        one_success = true;
    }

    ret = CommandManager.GetRotation(vec);
    if (ret) {
        pSystemAPI->SetActorRot(p_actor, vec);
        //CMath::SetActorRotation(p_actor, vec);
        one_success = true;
    }
    ret = CommandManager.GetRelRotation(vec);
    if (ret) {
        pSystemAPI->SetActorRelRot(p_actor, vec);
        //CMath::SetActorRelativeRotation(p_actor, vec);
        one_success = true;
    }

    ret = CommandManager.GetScale(vec);
    if (ret) {
        p_actor->SetActorScale3D(vec);
        one_success = true;
    }
    FString parent;
    ret = CommandManager.GetParent(parent);
    if (ret) {
        FString full_path = "";
        CommandManager.GetValue(CCLICommandManager::FullPath, full_path);

        if (p_actor != nullptr) {
            if (SetParent(p_actor, parent, full_path)) {
                SendActorBases();
            }
        }
        
    }

    ret = CommandManager.GetValue(CCLICommandManager::SensorSlotIndex, sint);
    if (ret) {
        if (pSystemAPI->SetSlotIndex(p_actor, sint)) {
            one_success = true;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::Focused, is_enabled);
    if (ret) {
        if (pSystemAPI->FocusCamera(p_actor)) {
            one_success = true;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::Property, sret);
    if (ret) {
        FString prop_value;
        ret = CommandManager.GetValue(CCLICommandManager::PropertyValue, prop_value);
        if (ret) {
            if (SetPropertyValue(p_actor, sret, prop_value)) {
                one_success = true;
            }
        }
        else {
            error_message += " value not defined";
            return false;
        }
      
    }
    else {

    }

  

    
    ret = CommandManager.GetValue(CCLICommandManager::IPAddr1, sret);
    if (ret) {
        
        if (pSystemAPI->GetConnectionInfo(p_actor, 0, conn)) {
            conn.IpAddr = sret;
            if (pSystemAPI->SetConnectionInfo(p_actor, 0, conn)) {
                one_success = true;
            }
        }
       
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::LocalPort1, sint);
    if (ret) {

        if (pSystemAPI->GetConnectionInfo(p_actor, 0, conn)) {
            conn.LocalPort = sint;
            if (pSystemAPI->SetConnectionInfo(p_actor, 0, conn)) {
                one_success = true;
            }
        }

    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::RemotePort1, sint);
    if (ret) {

        if (pSystemAPI->GetConnectionInfo(p_actor, 0, conn)) {
            conn.RemotePort = sint;
            if (pSystemAPI->SetConnectionInfo(p_actor, 0, conn)) {
                one_success = true;
            }
        }

    }
    else {

    }



    ret = CommandManager.GetValue(CCLICommandManager::IPAddr2, sret);
    if (ret) {

        if (pSystemAPI->GetConnectionInfo(p_actor, 1, conn)) {
            conn.IpAddr = sret;
            if (pSystemAPI->SetConnectionInfo(p_actor, 1, conn)) {
                one_success = true;
            }
        }

    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::LocalPort2, sint);
    if (ret) {

        if (pSystemAPI->GetConnectionInfo(p_actor, 1, conn)) {
            conn.LocalPort = sint;
            if (pSystemAPI->SetConnectionInfo(p_actor, 1, conn)) {
                one_success = true;
            }
        }

    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::RemotePort2, sint);
    if (ret) {

        if (pSystemAPI->GetConnectionInfo(p_actor, 1, conn)) {
            conn.RemotePort = sint;
            if (pSystemAPI->SetConnectionInfo(p_actor, 1, conn)) {
                one_success = true;
            }
        }

    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::HorizontalFov, dbl);
    if (ret) {
        if (pSystemAPI->SetHorizontalFov(p_actor, dbl)) {
            one_success = true;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::VericalFov, dbl);
    if (ret) {
        if (pSystemAPI->SetVerticalFov(p_actor, dbl)) {
            one_success = true;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::HorizontalScanStepAngleDeg, dbl);
    if (ret) {
        if (pSystemAPI->SetHorizontalScanStepAngleDeg(p_actor, dbl)) {
            one_success = true;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::VerticalScanStepAngleDeg, dbl);
    if (ret) {
        if (pSystemAPI->SetVerticalScanStepAngleDeg(p_actor, dbl)) {
            one_success = true;
        }
    }
    else {

    }
    ret = CommandManager.GetValue(CCLICommandManager::Beam, is_enabled);
    if (ret) {
        if (pSystemAPI->SetBeamEnabled(p_actor, is_enabled)) {
            one_success = true;
        }
    }
    else {

    }
    ret = CommandManager.GetValue(CCLICommandManager::RangeMin, dbl);
    if (ret) {
        if (pSystemAPI->SetRangeMinMeter(p_actor, dbl)) {
            one_success = true;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::RangeMax, dbl);
    if (ret) {
        if (pSystemAPI->SetRangeMaxMeter(p_actor, dbl)) {
            one_success = true;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::MeasurementErrorMean, dbl);
    if (ret) {
        if (pSystemAPI->SetMeasurementErrorMean(p_actor, dbl)) {
            one_success = true;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::MeasurementErrorStd, dbl);
    if (ret) {
        if (pSystemAPI->SetMeasurementErrorStd(p_actor, dbl)) {
            one_success = true;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::EnableSurfaceDetect, is_enabled);
    if (ret) {
        if (pSystemAPI->SetEnableSurfaceDetect(p_actor, is_enabled)) {
            one_success = true;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::EnableSubsurfaceDetect, is_enabled);
    if (ret) {
        if (pSystemAPI->SetEnableSubsurfaceDetect(p_actor, is_enabled)) {
            one_success = true;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::EnableFoamDetect, is_enabled);
    if (ret) {
        if (pSystemAPI->SetEnableFoamDetect(p_actor, is_enabled)) {
            one_success = true;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::SeaSurfaceDetectionProb, dbl);
    if (ret) {
        if (pSystemAPI->SetSeaSurfaceDetectionProb(p_actor, dbl)) {
            one_success = true;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::MaxSurfacePenetration, dbl);
    if (ret) {
        if (pSystemAPI->SetMaxSurfacePenetration(p_actor, dbl)) {
            one_success = true;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::RadarScanLevel, dbl);
    if (ret) {
        if (pSystemAPI->SetRadarScanLevel(p_actor, dbl)) {
            one_success = true;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::RadarScannerRPM, dbl);
    if (ret) {
        if (pSystemAPI->SetRadarScannerRPM(p_actor, dbl)) {
            one_success = true;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::RadarGainType, sint);
    if (ret) {
        if (pSystemAPI->SetRadarGainType(p_actor, sint)) {
            one_success = true;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::RadarGainLevel, sint);
    if (ret) {
        if (pSystemAPI->SetRadarGainLevel(p_actor, sint)) {
            one_success = true;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::RadarSeaClutterType, sint);
    if (ret) {
        if (pSystemAPI->SetRadarSeaClutterType(p_actor, sint)) {
            one_success = true;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::RadarSeaClutterLevel, sint);
    if (ret) {
        if (pSystemAPI->SetRadarSeaClutterLevel(p_actor, sint)) {
            one_success = true;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::RadarSeaClutterAutoOffset, sint);
    if (ret) {
        if (pSystemAPI->SetRadarSeaClutterAutoOffset(p_actor, sint)) {
            one_success = true;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::RadarRainClutterLevel, sint);
    if (ret) {
        if (pSystemAPI->SetRadarRainClutterLevel(p_actor, sint)) {
            one_success = true;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::RadarMaxGuardZoneCount, sint);
    if (ret) {
        if (pSystemAPI->SetRadarMaxGuardZoneCount(p_actor, sint)) {
            one_success = true;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::RadarMaxSectorBlankingZoneCount, sint);
    if (ret) {
        if (pSystemAPI->SetRadarMaxSectorBlankingZoneCount(p_actor, sint)) {
            one_success = true;;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::Closed, is_enabled);
    if (ret) {
        if (pSystemAPI->SetPathClosed(p_actor, is_enabled)) {
            one_success = true;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::Bake, sint);
    if (ret) {
        if (pSystemAPI->Bake(p_actor)) {
            one_success = true;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::Speed, dbl);
    if (ret) {
        if (pSystemAPI->SetPathSpeed(p_actor, dbl)) {
            one_success = true;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::Attach, sret);
    if (ret) {
        if (pSystemAPI->AttachToPath(p_actor, sret)) {
            one_success = true;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::Detach, sret);
    if (ret) {
        if (pSystemAPI->DetachFromPath(p_actor, sret)) {
            one_success = true;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::LineColor, color);
    if (ret) {
        if (pSystemAPI->SetPathLineColor(p_actor, color) ){
            one_success = true;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::SegmentCount, sint);
    if (ret) {
        if (pSystemAPI->SetPathSegmentCount(p_actor, sint)) {
            one_success = true;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::Straight, is_enabled);
    if (ret) {
        if (pSystemAPI->SetPathStraight(p_actor, is_enabled)) {
            one_success = true;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::TurnRate, dbl);
    if (ret) {
        if (pSystemAPI->SetPathTurnRate(p_actor, dbl)) {
            one_success = true;
        }
    }
    else {

    }

    {

        INT32S wp_ind;
        ret = CommandManager.GetValue(CCLICommandManager::Wp, wp_ind);
        if (ret) {
            FVector wp_pos;
            ret = CommandManager.GetValue(CCLICommandManager::WpPos, wp_pos);
            if (ret) {
                
                pSystemAPI->AddOrModifyWaypointToPath(p_actor, wp_ind, wp_pos);
            }


        }
        else {

        }

    }



  
    return one_success;
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
    FVector4 vec4d;
    FColor color;
    SConnectionInfo conn_info;


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

    ret = CommandManager.HasA(CCLICommandManager::UIVisible);
    if (ret) {
        if (pSystemAPI->GetIsUIVisible(is_enabled)) {
            SendConsoleResponse(name, CCLICommandManager::UIVisible, is_enabled);
            return true;
        }
    }
    else {

    }


    ret = CommandManager.GetValue(CCLICommandManager::Selected, sret);
    if (ret) {
        auto selected = pSystemAPI->GetSelectedActor();

        if (selected) {
            SendConsoleResponse("",CCLICommandManager::Selected, selected->GetName());
            return true;
        }
        else {
            SendConsoleResponse("",CCLICommandManager::Selected, FString("none"));
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



    if (name == "env") {
        int region = -1;
        ret = CommandManager.HasA(CCLICommandManager::TimeOfDayHour);
        if (ret) {
            if (pSystemAPI->GetTimeOfDayHr(region, dbl)) {
                SendConsoleResponse(name, CCLICommandManager::TimeOfDayHour, dbl);
                return true;
            }
        }
        else {

        }


        ret = CommandManager.HasA(CCLICommandManager::RainPercent);
        if (ret) {
            if (pSystemAPI->GetRainPercent(region, dbl)) {
                SendConsoleResponse(name, CCLICommandManager::RainPercent, dbl);
                return true;
            }
        }
        else {

        }


        ret = CommandManager.HasA(CCLICommandManager::CloudPercent);
        if (ret) {
            if (pSystemAPI->GetCloudPercent(region, dbl)) {
                SendConsoleResponse(name, CCLICommandManager::CloudPercent, dbl);
                return true;
            }
        }
        else {

        }

        ret = CommandManager.HasA(CCLICommandManager::FogPercent);
        if (ret) {
            if (pSystemAPI->GetFogPercent(region, dbl)) {
                SendConsoleResponse(name, CCLICommandManager::FogPercent, dbl);
                return true;
            }
        }
        else {

        }

        ret = CommandManager.HasA(CCLICommandManager::SnowPercent);
        if (ret) {
            if (pSystemAPI->GetSnowPercent(region, dbl)) {
                SendConsoleResponse(name, CCLICommandManager::SnowPercent, dbl);
                return true;
            }
        }
        else {

        }

        ret = CommandManager.HasA(CCLICommandManager::DustPercent);
        if (ret) {
            if (pSystemAPI->GetDustPercent(region, dbl)) {
                SendConsoleResponse(name, CCLICommandManager::DustPercent, dbl);
                return true;
            }
        }
        else {

        }

        ret = CommandManager.HasA(CCLICommandManager::ThunderPercent);
        if (ret) {
            if (pSystemAPI->GetThunderPercent(region, dbl)) {
                SendConsoleResponse(name, CCLICommandManager::ThunderPercent, dbl);
                return true;
            }
        }
        else {

        }

        ret = CommandManager.HasA(CCLICommandManager::EnvSoundFxEnabled);
        if (ret) {
            if (pSystemAPI->GetEnvSoundEnabled(region, is_enabled)) {
                SendConsoleResponse(name, CCLICommandManager::EnvSoundFxEnabled, is_enabled);
                return true;
            }
        }
        else {

        }

        ret = CommandManager.HasA(CCLICommandManager::WindSpeed);
        if (ret) {
            if (pSystemAPI->GetWindSpeedMeterPerSec(region, dbl)) {
                SendConsoleResponse(name, CCLICommandManager::WindSpeed, is_enabled);
                return true;
            }
        }
        else {

        }

        ret = CommandManager.HasA(CCLICommandManager::WindDirection);
        if (ret) {
            if (pSystemAPI->GetWindDirectionDeg(region, dbl)) {
                SendConsoleResponse(name, CCLICommandManager::WindDirection, is_enabled);
                return true;
            }
        }
        else {

        }

 

        ret = CommandManager.HasA(CCLICommandManager::EnvTimeFlowScale);
        if (ret) {
            if (pSystemAPI->GetEnvTimeFlowScale(region, dbl)) {
                SendConsoleResponse(name, CCLICommandManager::EnvTimeFlowScale, dbl);
                return true;
            }
        }
        else {

        }

       
    }

    p_actor = ASystemManagerBase::GetInstance()->FindActor(name);


    if (p_actor == nullptr ) {
        error_message += (("cannot find actor "));
        return false;
    }

    int instance_no = -1;
    ret = CommandManager.GetInstance(instance_no);
    if (ret) {
        
        SendConsoleResponse("",CCLICommandManager::Instance, instance_no);
        return true;

    }

    ret = CommandManager.HasPosition();
    if (ret) {
        SendConsoleResponse(name,CCLICommandManager::Position, TOW(p_actor->GetActorLocation()));
        return true;

    }

    ret = CommandManager.HasRelposition();
    if (ret) {
       
        SendConsoleResponse(name, CCLICommandManager::RelPosition, TOW(p_actor->GetActorLocation()));
        return true;

    }

    ret = CommandManager.HasRotation();
    if (ret) {

        vec = CMath::GetActorEulerAnglesRPY(p_actor);
        SendConsoleResponse(name, CCLICommandManager::Rotation, vec);
        return true;
    }
    ret = CommandManager.HasRelrotation();
    if (ret) {
        vec = CMath::GetActorReleativeEulerAnglesRPY(p_actor);
        SendConsoleResponse(name, CCLICommandManager::RelRotation, vec);
        return true;
    }

    ret = CommandManager.HasScale();
    if (ret) {
        vec = p_actor->GetActorScale();;
        
        SendConsoleResponse(name, CCLICommandManager::Scale, vec);
        return true;
    }

    ret = CommandManager.HasA(CCLICommandManager::Func);
    if (ret) {
     
        SendFunctions(p_actor,   "");
        return true;
    }
    else {
       
    }

    ret = CommandManager.GetValue(CCLICommandManager::Property, sret);
    if (ret) {
        FString prop_val;
        if (GetPropertyValue(p_actor, sret, prop_val)) {
            SendConsoleResponse(name, CCLICommandManager::Property, sret, prop_val);
        }
        else {
            error_message += " property not found";
        }
        
    }
    else {

    }

    ret = CommandManager.HasA(CCLICommandManager::Properties);
    if (ret) {

        SendProperies(p_actor);
    }
    else {

    }

    ret = CommandManager.HasA(CCLICommandManager::Suppressed);
    if (ret) {
        if (pSystemAPI->GetActorSuppressed(p_actor, is_enabled)) {
            SendConsoleResponse(name, CCLICommandManager::Suppressed, is_enabled);
            return true;
        }
    }
    else {

    }

    ret = CommandManager.HasA(CCLICommandManager::StandaloneModeEnabled);
    if (ret) {
        if (pSystemAPI->SetActorStandaloneModeEnabled(p_actor, is_enabled)) {
            SendConsoleResponse(name, CCLICommandManager::StandaloneModeEnabled, is_enabled);
            return true;
        }
    }
    else {

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

    ret = CommandManager.HasA(CCLICommandManager::All);
    if (ret) {

   
        SendConsoleResponse(p_actor);
        return true;
      
    }
    else {

    }

    ret = CommandManager.HasA(CCLICommandManager::SensorSlotIndex);
    if (ret) {

        if (pSystemAPI->GetSlotIndex(p_actor, sint)) {
            SendConsoleResponse(name, CCLICommandManager::SensorSlotIndex, sint);
            return true;
        }
    }
    else {

    }


 

    ret = CommandManager.HasA(CCLICommandManager::HorizontalFov);
    if (ret) {
        if (pSystemAPI->GetHorizontalFov(p_actor, dbl)) {
            SendConsoleResponse(name, CCLICommandManager::HorizontalFov, dbl);
            return true;
        }
    }
    else {

    }

    ret = CommandManager.HasA(CCLICommandManager::VericalFov);
    if (ret) {
        if (pSystemAPI->GetVerticalFov(p_actor, dbl)) {
            SendConsoleResponse(name, CCLICommandManager::VericalFov, dbl);
            return true;
        }
    }
    else {

    }

    ret = CommandManager.HasA(CCLICommandManager::IPAddr1);
    if (ret) {
        if (pSystemAPI->GetConnectionInfo(p_actor,0, conn_info)) {
            SendConsoleResponse(name, CCLICommandManager::IPAddr1, conn_info.IpAddr);
            return true;
        }
    }
    else {

    }
    ret = CommandManager.HasA(CCLICommandManager::LocalPort1);
    if (ret) {
        if (pSystemAPI->GetConnectionInfo(p_actor, 0, conn_info)) {
            SendConsoleResponse(name, CCLICommandManager::LocalPort1, conn_info.LocalPort);
            return true;
        }
    }
    else {

    }
    ret = CommandManager.HasA(CCLICommandManager::RemotePort1);
    if (ret) {
        if (pSystemAPI->GetConnectionInfo(p_actor, 0, conn_info)) {
            SendConsoleResponse(name, CCLICommandManager::RemotePort1, conn_info.RemotePort);
            return true;
        }
    }
    else {

    }

    ret = CommandManager.HasA(CCLICommandManager::IPAddr2);
    if (ret) {
        if (pSystemAPI->GetConnectionInfo(p_actor, 1, conn_info)) {
            SendConsoleResponse(name, CCLICommandManager::IPAddr2, conn_info.IpAddr);
            return true;
        }
    }
    else {

    }
    ret = CommandManager.HasA(CCLICommandManager::LocalPort2);
    if (ret) {
        if (pSystemAPI->GetConnectionInfo(p_actor, 1, conn_info)) {
            SendConsoleResponse(name, CCLICommandManager::LocalPort2, conn_info.LocalPort);
            return true;
        }
    }
    else {

    }
    ret = CommandManager.HasA(CCLICommandManager::RemotePort2);
    if (ret) {
        if (pSystemAPI->GetConnectionInfo(p_actor, 1, conn_info)) {
            SendConsoleResponse(name, CCLICommandManager::RemotePort2, conn_info.RemotePort);
            return true;
        }
    }
    else {

    }
    ret = CommandManager.HasA(CCLICommandManager::Beam);
    if (ret) {
        if (pSystemAPI->GetBeamEnabled(p_actor, is_enabled)) {
            SendConsoleResponse(name, CCLICommandManager::Beam, is_enabled);
            return true;
        }
    }
    else {

    }


    ret = CommandManager.HasA(CCLICommandManager::RangeMin);
    if (ret) {
        if (pSystemAPI->GetRangeMinMeter(p_actor, dbl)) {
            SendConsoleResponse(name, CCLICommandManager::RangeMin, dbl);
            return true;
        }
    }
    else {

    }

    ret = CommandManager.HasA(CCLICommandManager::RangeMax);
    if (ret) {
        if (pSystemAPI->GetRangeMaxMeter(p_actor, dbl)) {
            SendConsoleResponse(name, CCLICommandManager::RangeMax, dbl);
            return true;
        }
    }
    else {

    }

    ret = CommandManager.HasA(CCLICommandManager::HorizontalScanStepAngleDeg);
    if (ret) {
        if (pSystemAPI->GetHorizontalScanStepAngleDeg(p_actor, dbl)) {
            SendConsoleResponse(name, CCLICommandManager::HorizontalScanStepAngleDeg, dbl);
            return true;
        }
    }
    else {

    }

    ret = CommandManager.HasA(CCLICommandManager::VerticalScanStepAngleDeg);
    if (ret) {
        if (pSystemAPI->GetVerticalScanStepAngleDeg(p_actor, dbl)) {
            SendConsoleResponse(name, CCLICommandManager::VerticalScanStepAngleDeg, dbl);
            return true;
        }
    }
    else {

    }

    ret = CommandManager.HasA(CCLICommandManager::MeasurementErrorMean);
    if (ret) {
        if (pSystemAPI->GetMeasurementErrorMean(p_actor, dbl)) {
            SendConsoleResponse(name, CCLICommandManager::MeasurementErrorMean, dbl);
            return true;
        }
    }
    else {

    }

    ret = CommandManager.HasA(CCLICommandManager::MeasurementErrorStd);
    if (ret) {
        if (pSystemAPI->GetMeasurementErrorStd(p_actor, dbl)) {
            SendConsoleResponse(name, CCLICommandManager::MeasurementErrorStd, dbl);
            return true;
        }
    }
    else {

    }

    ret = CommandManager.HasA(CCLICommandManager::EnableSurfaceDetect);
    if (ret) {
        if (pSystemAPI->GetEnableSurfaceDetect(p_actor, is_enabled)) {
            SendConsoleResponse(name, CCLICommandManager::EnableSurfaceDetect, is_enabled);
            return true;
        }
    }
    else {

    }

    ret = CommandManager.HasA(CCLICommandManager::EnableSubsurfaceDetect);
    if (ret) {
        if (pSystemAPI->GetEnableSubsurfaceDetect(p_actor, is_enabled)) {
            SendConsoleResponse(name, CCLICommandManager::EnableSubsurfaceDetect, is_enabled);
            return true;
        }
    }
    else {

    }

    ret = CommandManager.HasA(CCLICommandManager::EnableFoamDetect);
    if (ret) {
        if (pSystemAPI->GetEnableFoamDetect(p_actor, is_enabled)) {
            SendConsoleResponse(name, CCLICommandManager::EnableFoamDetect, is_enabled);
            return true;
        }
    }
    else {

    }

    ret = CommandManager.HasA(CCLICommandManager::SeaSurfaceDetectionProb);
    if (ret) {
        if (pSystemAPI->GetSeaSurfaceDetectionProb(p_actor, dbl)) {
            SendConsoleResponse(name, CCLICommandManager::EnableFoamDetect, dbl);
            return true;
        }
    }
    else {

    }

    ret = CommandManager.HasA(CCLICommandManager::MaxSurfacePenetration);
    if (ret) {
        if (pSystemAPI->GetMaxSurfacePenetration(p_actor, dbl)) {
            SendConsoleResponse(name, CCLICommandManager::MaxSurfacePenetration, dbl);
            return true;
        }
    }
    else {

    }

    ret = CommandManager.HasA(CCLICommandManager::RadarScanLevel);
    if (ret) {
        if (pSystemAPI->GetRadarScanLevel(p_actor, sint)) {
            SendConsoleResponse(name, CCLICommandManager::RadarScanLevel, sint);
            return true;
        }
    }
    else {

    }

    ret = CommandManager.HasA(CCLICommandManager::RadarScannerRPM);
    if (ret) {
        if (pSystemAPI->GetRadarScannerRPM(p_actor, sint)) {
            SendConsoleResponse(name, CCLICommandManager::RadarScannerRPM, sint);
            return true;
        }
    }
    else {

    }

    ret = CommandManager.HasA(CCLICommandManager::RadarGainType);
    if (ret) {
        if (pSystemAPI->GetRadarGainType(p_actor, sint)) {
            SendConsoleResponse(name, CCLICommandManager::RadarGainType, sint);
            return true;
        }
    }
    else {

    }

    ret = CommandManager.HasA(CCLICommandManager::RadarGainLevel);
    if (ret) {
        if (pSystemAPI->GetRadarGainLevel(p_actor, sint)) {
            SendConsoleResponse(name, CCLICommandManager::RadarGainLevel, sint);
            return true;
        }
    }
    else {

    }

    ret = CommandManager.HasA(CCLICommandManager::RadarSeaClutterType);
    if (ret) {
        if (pSystemAPI->GetRadarSeaClutterType(p_actor, sint)) {
            SendConsoleResponse(name, CCLICommandManager::RadarSeaClutterType, sint);
            return true;
        }
    }
    else {

    }

    ret = CommandManager.HasA(CCLICommandManager::RadarSeaClutterLevel);
    if (ret) {
        if (pSystemAPI->GetRadarSeaClutterLevel(p_actor, sint)) {
            SendConsoleResponse(name, CCLICommandManager::RadarSeaClutterLevel, sint);
            return true;
        }
    }
    else {

    }

    ret = CommandManager.HasA(CCLICommandManager::RadarSeaClutterAutoOffset);
    if (ret) {
        if (pSystemAPI->GetRadarSeaClutterAutoOffset(p_actor, sint)) {
            SendConsoleResponse(name, CCLICommandManager::RadarSeaClutterAutoOffset, sint);
            return true;
        }
    }
    else {

    }

    ret = CommandManager.HasA(CCLICommandManager::RadarRainClutterLevel);
    if (ret) {
        if (pSystemAPI->GetRadarRainClutterLevel(p_actor, sint)) {
            SendConsoleResponse(name, CCLICommandManager::RadarRainClutterLevel, sint);
            return true;
        }
    }
    else {

    }

    ret = CommandManager.HasA(CCLICommandManager::RadarMaxGuardZoneCount);
    if (ret) {
        if (pSystemAPI->GetRadarMaxGuardZoneCount(p_actor, sint)) {
            SendConsoleResponse(name, CCLICommandManager::RadarMaxGuardZoneCount, sint);
            return true;
        }
    }
    else {

    }

    ret = CommandManager.HasA(CCLICommandManager::RadarMaxSectorBlankingZoneCount);
    if (ret) {
        if (pSystemAPI->GetRadarMaxSectorBlankingZoneCount(p_actor, sint)) {
            SendConsoleResponse(name, CCLICommandManager::RadarMaxSectorBlankingZoneCount, sint);
            return true;
        }
    }
    else {

    }



    ret = CommandManager.HasA(CCLICommandManager::Closed);
    if (ret) {
        if (pSystemAPI->GetPathClosed(p_actor, is_enabled)) {
            SendConsoleResponse(name, CCLICommandManager::Closed, is_enabled);
            return true;
        }
    }
    else {

    }


    ret = CommandManager.HasA(CCLICommandManager::Speed);
    if (ret) {
        if (pSystemAPI->GetPathSpeed(p_actor, dbl)) {
            SendConsoleResponse(name, CCLICommandManager::Speed, dbl);
            return true;
        }
    }
    else {

    }

    ret = CommandManager.HasA(CCLICommandManager::SegmentCount);
    if (ret) {
        if (pSystemAPI->GetPathSegmentCount(p_actor, sint)) {
            SendConsoleResponse(name, CCLICommandManager::SegmentCount, sint);
            return true;
        }
    }
    else {

    }

    ret = CommandManager.HasA(CCLICommandManager::LineColor);
    if (ret) {
        if (pSystemAPI->GetPathLineColor(p_actor, color)) {
            SendConsoleResponse(name, CCLICommandManager::LineColor, color);
            return true;
        }
    }
    else {

    }

    ret = CommandManager.HasA(CCLICommandManager::Straight);
    if (ret) {
        if (pSystemAPI->GetPathStraight(p_actor, is_enabled)) {
            SendConsoleResponse(name, CCLICommandManager::Straight, is_enabled);
            return true;
        }
    }
    else {

    }

    ret = CommandManager.HasA(CCLICommandManager::TurnRate);
    if (ret) {
        if (pSystemAPI->GetPathTurnRate(p_actor, dbl)) {
            SendConsoleResponse(name, CCLICommandManager::TurnRate, dbl);
            return true;
        }
    }
    else {

    }

    ret = CommandManager.HasA(CCLICommandManager::Attach);
    if (ret) {
        if (pSystemAPI->GetActorAttachedToPath(p_actor, sret)) {
            SendConsoleResponse(name, CCLICommandManager::Attach, sret);
            return true;
        }
    }
    else {

    }


    {

        INT32S wp_ind;
        ret = CommandManager.GetValue(CCLICommandManager::Wp, wp_ind);
        if (ret) {
            if (pSystemAPI->GetWaypointPosition(p_actor, wp_ind, vec)) {
                SendConsoleResponse(name, CCLICommandManager::Wp, wp_ind, TOW(vec));
            }
 
        }
        else {

        }

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
        else {
            error_message = "failed to load";
        }
    }
    else {
        
    }

    ret = CommandManager.GetValue(CCLICommandManager::SaveFile, path);
    if (ret) {
        if (pSystemAPI->Save(path)) {
            return true;
        }
        else {
            error_message = "failed to save";
        }
    }
    else {
        
    }


    return false;

}

bool UConsoleBase::ProcessExecCommand(TMap<FString, FString>& options, FString& error_message)
{
    error_message = "";

    bool ret;

    FString name;
    AActor* p_actor;
  
    FString func;
    FString params[10] = {};
    FString values[10] = {};

    ret = CommandManager.GetValue(CCLICommandManager::Name, name);
    if (ret) {
        error_message += (("enter a valid name "));
    }
    else {

    }

    p_actor = ASystemManagerBase::GetInstance()->FindActor(name);


    if (p_actor == nullptr) {
        error_message += (("cannot find actor "));
        return false;
    }

    ret = CommandManager.GetValue(CCLICommandManager::Func, func);
    if (!ret) {
        error_message += (("enter a function name "));
        return false;
    }
    else {

    }
    FString param_name[] = {CCLICommandManager::FuncParam1,CCLICommandManager::FuncParam2, CCLICommandManager::FuncParam3, CCLICommandManager::FuncParam4, CCLICommandManager::FuncParam5,
                             CCLICommandManager::FuncParam6,CCLICommandManager::FuncParam7, CCLICommandManager::FuncParam8, CCLICommandManager::FuncParam9, CCLICommandManager::FuncParam10 };
    
    
     TMap<FString, FString> param_map;
    for (int i = 0; i < 10; i++) {
        ret = CommandManager.GetValue(param_name[i], params[i]);
    }

    TArray<FString> param_array;

    for (int32 i = 0; i < 10; ++i)
    {
        if (params[i] != "") {
            param_array.Add(params[i]);
        }
        
    }

    ret = InvokeFunctionByNameWithParameters(p_actor, FName(func), param_array);
    return ret;
}

bool UConsoleBase::ProcessCommands(FString command, TMap<FString, FString>& options, FString& error_message, INT32S &delay_tick)
{

    delay_tick = 0;

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
        return  ProcessCreateCommand(options, error_message, delay_tick);
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
    else if (command == CommandManager.ExecCommand) {
        return ProcessExecCommand(options, error_message);
    }

    return false;

}

bool UConsoleBase::InvokeFunctionByNameWithParameters(UObject *p_obj, const FName& FunctionName, const TMap<FString, FString>& Parameters)
{
    UFunction* Function = p_obj->GetClass()->FindFunctionByName(FunctionName);
    if (Function == nullptr)
    {
        return false;
    }

    // Create a struct on scope based on the function's parameters
    FStructOnScope FuncParams(Function);

    // Iterate over the function's properties
    for (TFieldIterator<FProperty> It(Function); It; ++It)
    {
        FProperty* Property = *It;
        auto p = Parameters.Find(Property->GetName());
        if (p)
        {
            if (!SetFunctionParameter(Function, FuncParams.GetStructMemory(), Property, *p))
            {
                return false; // Failed to set a parameter, return early
            }
        }
    }

    GetOwner()->ProcessEvent(Function, FuncParams.GetStructMemory());
   
    return true;
}


bool UConsoleBase::InvokeFunctionByNameWithParameters(UObject* p_obj, const FName& FunctionName,  TArray<FString>& Parameters)
{
    UFunction* Function = p_obj->GetClass()->FindFunctionByName(FunctionName);
    if (Function == nullptr)
    {
        return false;
    }

    // Create a struct on scope based on the function's parameters
    FStructOnScope FuncParams(Function);

    // Initialize an index to track the position in the Parameters array
    int32 ParamIndex = 0;

    // Iterate over the function's properties
    for (TFieldIterator<FProperty> It(Function); It; ++It)
    {
        FProperty* Property = *It;
        if (Parameters.IsValidIndex(ParamIndex))
        {
            const FString& ParamValue = Parameters[ParamIndex];
            if (!SetFunctionParameter(Function, FuncParams.GetStructMemory(), Property, ParamValue))
            {
                return false; // Failed to set a parameter, return early
            }
            ParamIndex++;

            
        }
        else
        {
            // Not enough parameters provided
            return false;
        }

        if (ParamIndex == Parameters.Num()) {
            break;
        }
    }

    // Ensure all parameters were used
    if (ParamIndex != Parameters.Num())
    {
        return false; // Extra parameters were provided
    }

    p_obj->ProcessEvent(Function, FuncParams.GetStructMemory());

    return true;
}
bool UConsoleBase::SetFunctionParameter(UFunction* Function, void* Params, FProperty* Param, const FString& Value)
{
    if (FNumericProperty* NumericProperty = CastField<FNumericProperty>(Param))
    {
        if (NumericProperty->IsFloatingPoint())
        {
            double NumValue = FCString::Atod(*Value);
            NumericProperty->SetFloatingPointPropertyValue(Param->ContainerPtrToValuePtr<void>(Params), NumValue);
        }
        else if (FIntProperty* IntProperty = CastField<FIntProperty>(Param))
        {
            int32 NumValue = FCString::Atoi(*Value);
            IntProperty->SetPropertyValue(Param->ContainerPtrToValuePtr<void>(Params), NumValue);
        }
        else if (FUInt32Property* UInt32Property = CastField<FUInt32Property>(Param))
        {
            uint32 NumValue = static_cast<uint32>(FCString::Strtoui64(*Value, nullptr, 10));
            UInt32Property->SetPropertyValue(Param->ContainerPtrToValuePtr<void>(Params), NumValue);
        }
        else if (FInt64Property* Int64Property = CastField<FInt64Property>(Param))
        {
            int64 NumValue = FCString::Atoi64(*Value);
            Int64Property->SetPropertyValue(Param->ContainerPtrToValuePtr<void>(Params), NumValue);
        }
        else if (FUInt64Property* UInt64Property = CastField<FUInt64Property>(Param))
        {
            uint64 NumValue = FCString::Strtoui64(*Value, nullptr, 10);
            UInt64Property->SetPropertyValue(Param->ContainerPtrToValuePtr<void>(Params), NumValue);
        }
        // Handle other integer types (int64, uint64, etc.) similarly
    }
    else if (FStrProperty* StringProperty = CastField<FStrProperty>(Param))
    {
        StringProperty->SetPropertyValue(Param->ContainerPtrToValuePtr<void>(Params), Value);
    }
    else if (FStructProperty* StructProperty = CastField<FStructProperty>(Param))
    {
        if (StructProperty->Struct == TBaseStructure<FVector>::Get())
        {
            FVector VectorValue;
            VectorValue.InitFromString(Value);
            StructProperty->CopyCompleteValue(StructProperty->ContainerPtrToValuePtr<void>(Params), &VectorValue);
        }
        else if (StructProperty->Struct == TBaseStructure<FVector2D>::Get())
        {
            FVector2D Vector2DValue;
            Vector2DValue.InitFromString(Value);
            StructProperty->CopyCompleteValue(StructProperty->ContainerPtrToValuePtr<void>(Params), &Vector2DValue);
        }
        // Add other struct types as needed
    }
    // Add other property types as needed

    return true;
}

TArray<FString> UConsoleBase::GetCallableFunctions(UObject* p_obj, FString category)
{
    TArray<FString> FunctionSignatures;
    FString Category = category;
    if (!p_obj)
    {
        return FunctionSignatures;
    }

    for (TFieldIterator<UFunction> FuncIt(p_obj->GetClass()); FuncIt; ++FuncIt)
    {
        UFunction* Function = *FuncIt;
        FString func_name = Function->GetName();
        if (!func_name.EndsWith(TEXT("_"))){
            continue;
        }

        // Check the category of the function
#if 0
        const FString FunctionCategory = Function->GetMetaData(TEXT("Category"));
        if (Category != "" && FunctionCategory != Category)
        {
            continue;
        }
#endif

        FString FunctionSignature;

        // Add return type
        if (Function->HasAnyFunctionFlags(FUNC_HasOutParms))
        {
            for (TFieldIterator<FProperty> PropIt(Function); PropIt; ++PropIt)
            {
                FProperty* Prop = *PropIt;
                if (Prop->HasAnyPropertyFlags(CPF_OutParm) && !Prop->HasAnyPropertyFlags(CPF_ConstParm))
                {
                    FunctionSignature += Prop->GetCPPType() + " ";
                    break;
                }
            }
        }
        else
        {
            FunctionSignature += "void ";
        }

        // Add function name
        FunctionSignature += Function->GetName();

        // Add parameters
        FunctionSignature += "(";
        bool bFirstParam = true;
        for (TFieldIterator<FProperty> PropIt(Function); PropIt; ++PropIt)
        {
            FProperty* Prop = *PropIt;
            if (!Prop->HasAnyPropertyFlags(CPF_Parm) || Prop->HasAnyPropertyFlags(CPF_ReturnParm))
            {
                continue;
            }

            if (!bFirstParam)
            {
                FunctionSignature += ", ";
            }
            bFirstParam = false;

            FunctionSignature += Prop->GetCPPType() + " " + Prop->GetName();
        }
        FunctionSignature += ")";

        FunctionSignatures.Add(FunctionSignature);
    }

    return FunctionSignatures;
}
TArray<FString> UConsoleBase::GetProperties(UObject* p_obj)
{
    TArray<FString> PropertiesList;

    if (!p_obj)
    {
        return PropertiesList;
    }

    for (TFieldIterator<FProperty> PropIt(p_obj->GetClass()); PropIt; ++PropIt)
    {
        FProperty* Property = *PropIt;
     

        FString PropertyType = Property->GetCPPType();
        FString PropertyName = Property->GetName();

        if (PropertyName.EndsWith("_")) {
            PropertiesList.Add(FString::Printf(TEXT("%s %s"), *PropertyType, *PropertyName));
        }

        
    }

    return PropertiesList;
}
bool UConsoleBase::SetPropertyValue(UObject* p_obj, const FString& property_name, const FString& value)
{
    if (!p_obj)
    {
        return false;
    }
    if (!property_name.EndsWith(TEXT("_"))) {
        return false;
    }
    // Find the property by name
    FProperty* Property = FindFProperty<FProperty>(p_obj->GetClass(), FName(property_name));
    if (!Property)
    {
        return false; // Property not found
    }

    // Check property type and set value accordingly
    if (FStrProperty* StrProperty = CastField<FStrProperty>(Property))
    {
        StrProperty->SetPropertyValue_InContainer(p_obj, value);
    }
    else if (FNumericProperty* NumProperty = CastField<FNumericProperty>(Property))
    {
        if (NumProperty->IsFloatingPoint())
        {
            
                double DoubleValue = FCString::Atod(*value);
                NumProperty->SetFloatingPointPropertyValue(NumProperty->ContainerPtrToValuePtr<void>(p_obj), DoubleValue);
            
           
        }
        else if (NumProperty->IsInteger())
        {
            
                int64 Int64Value = FCString::Atoi64(*value);
                NumProperty->SetIntPropertyValue(NumProperty->ContainerPtrToValuePtr<void>(p_obj), Int64Value);
            
           
        }
        else if (FBoolProperty* BoolProperty = CastField<FBoolProperty>(Property))
        {
            bool BoolValue = value.ToBool();
            BoolProperty->SetPropertyValue_InContainer(p_obj, BoolValue);
        }
    }
    else if (FStructProperty* StructProperty = CastField<FStructProperty>(Property))
    {
        if (StructProperty->Struct->GetFName() == NAME_Vector)
        {
            FVector VecValue;
            VecValue.InitFromString(value);
            StructProperty->ContainerPtrToValuePtr<FVector>(p_obj)->operator=(VecValue);
        }
        else if (StructProperty->Struct->GetFName() == NAME_Vector2D)
        {
            FVector2D Vec2DValue;
            Vec2DValue.InitFromString(value);
            StructProperty->ContainerPtrToValuePtr<FVector2D>(p_obj)->operator=(Vec2DValue);
        }
    }
    else
    {
        return false; // Unsupported property type
    }

    return true;
}

bool UConsoleBase::GetPropertyValue(UObject* p_obj, const FString& property_name, FString& value)
{
    if (!p_obj)
    {
        return false;
    }

    // Find the property by name
    FProperty* Property = FindFProperty<FProperty>(p_obj->GetClass(), FName(*property_name));
    if (!Property)
    {
        return false; // Property not found
    }

    // Check property type and get value accordingly
    if (FStrProperty* StrProperty = CastField<FStrProperty>(Property))
    {
        value = StrProperty->GetPropertyValue_InContainer(p_obj);
    }
    else if (FBoolProperty* BoolProperty = CastField<FBoolProperty>(Property))
    {
        bool BoolValue = BoolProperty->GetPropertyValue_InContainer(p_obj);
        value = BoolValue ? TEXT("true") : TEXT("false");
    }
    else if (FNumericProperty* NumProperty = CastField<FNumericProperty>(Property))
    {
        if (NumProperty->IsFloatingPoint())
        {
            double NumValue = NumProperty->GetFloatingPointPropertyValue(NumProperty->ContainerPtrToValuePtr<void>(p_obj));
            value = FString::Printf(TEXT("%f"), NumValue);
        }
        else if (NumProperty->IsInteger())
        {
            int64 NumValue = NumProperty->GetSignedIntPropertyValue(NumProperty->ContainerPtrToValuePtr<void>(p_obj));
            value = FString::Printf(TEXT("%lld"), NumValue);
        }
    }
    else if (FStructProperty* StructProperty = CastField<FStructProperty>(Property))
    {
        if (StructProperty->Struct->GetFName() == NAME_Vector)
        {
            FVector VecValue = *StructProperty->ContainerPtrToValuePtr<FVector>(p_obj);
            value = VecValue.ToString();
        }
        else if (StructProperty->Struct->GetFName() == NAME_Vector2D)
        {
            FVector2D Vec2DValue = *StructProperty->ContainerPtrToValuePtr<FVector2D>(p_obj);
            value = Vec2DValue.ToString();
        }
    }
    else
    {
        return false; // Unsupported property type
    }

    return true;
}