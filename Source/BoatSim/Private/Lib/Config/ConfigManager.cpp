// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/Config/ConfigManager.h"
#include "Misc/CString.h"

// Sets default values for this component's properties
UConfigManager::UConfigManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UConfigManager::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UConfigManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}



bool UConfigManager::ParsePose(const FString& Text, FVector &translation, FVector &rpy)
{
    FVector ParsedVector;
    TArray<FString> VectorValues;
    Text.ParseIntoArray(VectorValues, TEXT(" "), true);

    if (VectorValues.Num() >= 6)
    {
        translation.X = FCString::Atof(*VectorValues[0]);
        translation.Y = FCString::Atof(*VectorValues[1]);
        translation.Z = FCString::Atof(*VectorValues[2]);

        rpy.X = FCString::Atof(*VectorValues[3]);
        rpy.Y = FCString::Atof(*VectorValues[4]);
        rpy.Z = FCString::Atof(*VectorValues[5]);

        return true;
    }

    return false;
}

bool UConfigManager::ParseVector2D(const FString& Text, FVector2D& ret)
{
    FVector ParsedVector;
    TArray<FString> VectorValues;
    Text.ParseIntoArray(VectorValues, TEXT(" "), true);

    if (VectorValues.Num() >=2)
    {
        ret.X = FCString::Atof(*VectorValues[0]);
        ret.Y = FCString::Atof(*VectorValues[1]);
     

        return true;
    }

    return false;
}
float UConfigManager::ParseFloat(tinyxml2::XMLElement* paramsElement, FString name, float def_value)
{
    tinyxml2:: XMLElement* p_elm = paramsElement->FirstChildElement(TCHAR_TO_ANSI(*name));

    if (p_elm != nullptr) {
        float val = FCString::Atof((const TCHAR *) p_elm->GetText());
        return val;

    }

    return def_value;

}
FString UConfigManager::ParseString(tinyxml2::XMLElement* paramsElement, FString name, FString def_value)
{
    tinyxml2::XMLElement* p_elm = paramsElement->FirstChildElement(TCHAR_TO_ANSI(*name));

    if (p_elm != nullptr) {
        FString val = FString((const TCHAR*)p_elm->GetText());
        return val;

    }

    return def_value;
}

void UConfigManager::ParseXML()
{
    tinyxml2::XMLDocument xmlDoc;
    
    tinyxml2::XMLElement* sensorElement;

    

    if (xmlDoc.LoadFile(TCHAR_TO_UTF8(*FilePath)) == tinyxml2::XML_SUCCESS)
    {
        tinyxml2::XMLElement* rootElement = xmlDoc.RootElement();
        if (rootElement)
        {
            
            // Process sensor elements
            sensorElement = rootElement->FirstChildElement("sensor");
            while (sensorElement)
            {
                FString name = ParseString(sensorElement,"name");
                FString type = ParseString(sensorElement, "type");
                FString blueprint = ParseString(sensorElement, "blueprint");

                USensorDescBase *p_sensor_desc = NewObject< USensorDescBase> ();

                p_sensor_desc->Name = name;
                p_sensor_desc->Type = type;
                p_sensor_desc->Blueprint = blueprint;

                tinyxml2::XMLElement* paramsElement = sensorElement->FirstChildElement("params");
                float range = ParseFloat(paramsElement, "range");
                float fov = ParseFloat(paramsElement, "fov");
                FString cam_sensor_size = ParseString(sensorElement, "camsensorsize"); 
                FString cam_sensor_type = ParseString(sensorElement, "camsensortype");   
                // Process sensor data as needed
                // ...

                UParamDescBase* p_param_desc = NewObject< UParamDescBase>();

                p_param_desc->RangeMeter = FVector2D(0, range);
                p_param_desc->FovDeg = fov;
                ParseVector2D(cam_sensor_size, p_param_desc->CameraSize);
               
                p_param_desc->CameraSensorType = cam_sensor_type;
                p_sensor_desc->AddParamDesc(p_param_desc);


                Descs.Add(p_sensor_desc);
                sensorElement = sensorElement->NextSiblingElement("sensor");

               
            }

            // Process gimbal element
            tinyxml2::XMLElement* gimbalElement = rootElement->FirstChildElement("gimbal");
            if (gimbalElement)
            {
                FString name = FString(gimbalElement->FirstChildElement("name")->GetText());

                UGimbalDescBase* p_gimbal = NewObject < UGimbalDescBase>();

                tinyxml2::XMLElement* paramsElement = gimbalElement->FirstChildElement("params");
                float yawMin = FCString::Atof((const TCHAR*)paramsElement->FirstChildElement("yawlimits")->GetText());
                float yawMax = FCString::Atof((const TCHAR*)paramsElement->FirstChildElement("yawlimits")->NextSiblingElement()->GetText());
                float pitchMin = FCString::Atof((const TCHAR*)paramsElement->FirstChildElement("pitchlimits")->GetText());
                float pitchMax = FCString::Atof((const TCHAR*)paramsElement->FirstChildElement("pitchlimits")->NextSiblingElement()->GetText());
                float rollMin = FCString::Atof((const TCHAR*)paramsElement->FirstChildElement("rolllimits")->GetText());
                float rollMax = FCString::Atof((const TCHAR*)paramsElement->FirstChildElement("rolllimits")->NextSiblingElement()->GetText());
                float yawRate = FCString::Atof((const TCHAR*)paramsElement->FirstChildElement("yawrate")->GetText());
                float pitchRate = FCString::Atof((const TCHAR*)paramsElement->FirstChildElement("pitchrate")->GetText());
                float rollRate = FCString::Atof((const TCHAR*)paramsElement->FirstChildElement("rollrate")->GetText());

                UParamDescBase* p_param_desc = NewObject < UParamDescBase>();

                p_param_desc->YawLimit = FVector2D(yawMin, yawMax);
                p_param_desc->PitchLimit = FVector2D(pitchMin, pitchMax);
                p_param_desc->RollLimit = FVector2D(rollMin, rollMax);

                p_param_desc->YawRateDegPerSec = yawRate;
                p_param_desc->PitchRateDegPerSec = pitchRate;
                p_param_desc->RollRateDegPerSec = rollRate;

                p_gimbal->AddParamDesc(p_param_desc);

                Descs.Add(p_gimbal);

                // Process gimbal data as needed
                // ...
            }

            // Process boat element
            tinyxml2::XMLElement* boatElement = rootElement->FirstChildElement("boat");
            if (boatElement)
            {

                UBoatDescBase* p_boat_desc = NewObject < UBoatDescBase>();
                FString name = FString(boatElement->FirstChildElement("name")->GetText());
                FString blueprint = FString(boatElement->FirstChildElement("blueprint")->GetText());
                
                p_boat_desc->Name = name;
                p_boat_desc->Blueprint = blueprint;

                sensorElement = boatElement->FirstChildElement("sensor");
                while (sensorElement)
                {
                    FString sensorName = FString(sensorElement->FirstChildElement("name")->GetText());
                    FString pose = FString(sensorElement->FirstChildElement("pose")->GetText());
                    FString gimbal = FString(sensorElement->FirstChildElement("gimbal")->GetText());

                    // Process boat sensor data as needed
                    // ...
                    UParamDescBase* p_param_desc = NewObject <UParamDescBase>();
                    p_param_desc->SensorName = sensorName;
                    ParsePose(pose, p_param_desc->PlacementPosition, p_param_desc->PlacementRotationRPY);
                    p_param_desc->GimbalName = gimbal;
                    sensorElement = sensorElement->NextSiblingElement("sensor");

                    p_boat_desc->AddParamDesc(p_param_desc);

                   
                }

                Descs.Add(p_boat_desc);
                
            }

            // Process scene elements
            tinyxml2::XMLElement* sceneElement = rootElement->FirstChildElement("scene");
            if (sceneElement)
            {
                tinyxml2::XMLElement* elementElement = sceneElement->FirstChildElement("element");
                while (elementElement)
                {
                    USceneDescBase* p_scene_desc = NewObject<USceneDescBase>();
                    FString elementName = FString(elementElement->FirstChildElement("name")->GetText());
                    FString pose = FString(elementElement->FirstChildElement("pose")->GetText());


                    p_scene_desc->Owner = elementName;
                   
                    UParamDescBase* p_param_desc = NewObject< UParamDescBase>();

                    ParsePose(pose, p_param_desc->PlacementPosition, p_param_desc->PlacementRotationRPY);

                    p_scene_desc->AddParamDesc(p_param_desc);

                    elementElement = elementElement->NextSiblingElement("element");
                    Descs.Add(p_scene_desc);
                }

               
            }
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load XML file: %s"), *FilePath);
    }
}

TArray<UDescBase*>& UConfigManager::GetAllDesc()
{
    return Descs;
}
