// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/Config/ConfigManager.h"
#include "Misc/CString.h"
#include <Lib/Utils/CUtil.h>

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


float UConfigManager::ParseFloat(tinyxml2::XMLElement* paramsElement, FString name, float def_value)
{
    tinyxml2:: XMLElement* p_elm = paramsElement->FirstChildElement(TCHAR_TO_ANSI(*name));

    if (p_elm != nullptr) {
        const char* textValue = p_elm->GetText();
     
        float val = CUtil::ConvertToFloat(textValue);
        return val;

    }

    return def_value;

}
FString UConfigManager::ParseString(tinyxml2::XMLElement* paramsElement, FString name, FString def_value)
{
    tinyxml2::XMLElement* p_elm = paramsElement->FirstChildElement(TCHAR_TO_ANSI(*name));

    if (p_elm != nullptr) {
        const char* textValue = p_elm->GetText();

        // Convert the const char* to an FString
        FString val = CUtil::ConvertToFString(textValue);
        return val;

    }

    return def_value;
}

FVector2D UConfigManager::ParseVector2D(tinyxml2::XMLElement* paramsElement, FString name, FVector2D def_value)
{
    tinyxml2::XMLElement* p_elm = paramsElement->FirstChildElement(TCHAR_TO_ANSI(*name));

    if (p_elm != nullptr) {
        const char* textValue = p_elm->GetText();
        FVector2D vec;
        bool ret = CUtil::ParseVector2D(CUtil::ConvertToFString(textValue), vec);
        if (ret) {
            return vec;
        }
        else {
            return  def_value;
        }
    }

    return def_value;
}
bool UConfigManager::ParsePose(tinyxml2::XMLElement* paramsElement, FString name, FVector& pos, FVector& rpy)
{
    tinyxml2::XMLElement* p_elm = paramsElement->FirstChildElement(TCHAR_TO_ANSI(*name));
    if (p_elm != nullptr) {
        const char* textValue = p_elm->GetText();
        FVector2D vec;
        bool ret = CUtil::ParsePose(CUtil::ConvertToFString(textValue), pos, rpy);
        if (ret) {
            return true;
        }
        else {
            return  false;
        }
    }

    return false;
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
                FVector2D range = ParseVector2D(paramsElement, "range");
                float fov = ParseFloat(paramsElement, "fov");

                
                FVector2D sensor_size = ParseVector2D(paramsElement, "camsensorsize");
                FString cam_sensor_type = ParseString(paramsElement, "camsensortype");
                // Process sensor data as needed
                // ...

                UParamDescBase* p_param_desc = NewObject< UParamDescBase>();

           
                p_param_desc->RangeMeter = range;
                p_param_desc->FovDeg = fov;
                p_param_desc->CameraSize = sensor_size;
               
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
                FVector2D yaw_limits = ParseVector2D(paramsElement, "yawlimits");
                FVector2D pitch_limits = ParseVector2D(paramsElement, "pitchlimits"); 
                FVector2D roll_limits = ParseVector2D(paramsElement, "rolllimits");
                float yawRate = ParseFloat(paramsElement, "yawrate");
                float pitchRate = ParseFloat(paramsElement, "pitchrate");
                float rollRate = ParseFloat(paramsElement, "rollrate");
                   
             
                UParamDescBase* p_param_desc = NewObject < UParamDescBase>();

                p_param_desc->YawLimit = yaw_limits;
                p_param_desc->PitchLimit = pitch_limits;
                p_param_desc->RollLimit = roll_limits;

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
            while (boatElement)
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
                    FString gimbal = FString(sensorElement->FirstChildElement("gimbal")->GetText());

                    // Process boat sensor data as needed
                    // ...
                    UParamDescBase* p_param_desc = NewObject <UParamDescBase>();
                    p_param_desc->SensorName = sensorName;
                    ParsePose(sensorElement, "pose", p_param_desc->PlacementPosition, p_param_desc->PlacementRotationRPY);
                    p_param_desc->GimbalName = gimbal;
                    sensorElement = sensorElement->NextSiblingElement("sensor");

                    p_boat_desc->AddParamDesc(p_param_desc);

                   
                }

                Descs.Add(p_boat_desc);
                boatElement = boatElement->NextSiblingElement("boat");
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

                    ParsePose(elementElement, "pose", p_param_desc->PlacementPosition, p_param_desc->PlacementRotationRPY);

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
