// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Lib/XML/tinyxml/tinyxml2.h"
#include "ConfigManager.generated.h"

class UCDescBase;
class UCParamDescBase;
class UCSensorDescBase;
class UGimbalDescBase;
class UBoatDescBase;
class USceneDescBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UConfigManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UConfigManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	

	UPROPERTY(EditAnywhere)
		FString FilePath = "Path/To/Your/XML/File.xml";

    TArray<UDescBase*> Descs;

    float ParseFloat(tinyxml2::XMLElement* paramsElement, FString name, float def_value = 0);
    FString ParseString(tinyxml2::XMLElement* paramsElement, FString name, FString def_value = "");
    FVector2D ParseVector2D(tinyxml2::XMLElement* paramsElement, FString name, FVector2D def_value = FVector2D::ZeroVector);
    bool ParsePose(tinyxml2::XMLElement* paramsElement, FString name, FVector& pos, FVector& rpy);
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
    virtual void ParseXML();
    TArray<UDescBase*>& GetAllDesc();
private:
   
    


};

UCLASS()
class UDescBase:public UObject
{
    GENERATED_BODY()
 

};

UCLASS()
class UParamDescBase :public UDescBase
{
    GENERATED_BODY()
public:
    FVector2D RangeMeter;
    float FovDeg;
    FVector2D YawLimit;
    FVector2D PitchLimit;
    FVector2D RollLimit;
    float YawRateDegPerSec;
    float PitchRateDegPerSec;
    float RollRateDegPerSec;
    bool IsLocal;
    FVector PlacementPosition;
    FVector PlacementRotationRPY;
    FString SensorName;
    FString GimbalName;

    FVector2D CameraSize;
    FString CameraSensorType;
};
UCLASS()
class USensorDescBase :public UDescBase
{
    GENERATED_BODY()
public:
    FString Name;
    FString Type;
    FString Blueprint;




protected:
    TArray< UParamDescBase*> Params;


public:
    void AddParamDesc(UParamDescBase* p_desc)
    {
        Params.Add(p_desc);
    }
};
UCLASS()
class UGimbalDescBase :public UDescBase
{
    GENERATED_BODY()
public:
    FString Name;
    FString Blueprint;
protected:
    TArray< UParamDescBase*> Params;

public:
    void AddParamDesc(UParamDescBase* p_desc)
    {
        Params.Add(p_desc);
    }

};
UCLASS()
class UBoatDescBase :public UDescBase
{
    GENERATED_BODY()
public:
    FString Name;
    FString Type;
    FString Blueprint;

protected:
    TArray<UParamDescBase*> Params;

public:
    void AddParamDesc(UParamDescBase* p_desc)
    {
        Params.Add(p_desc);
    }
};
UCLASS()
class USceneDescBase :public UDescBase
{
    GENERATED_BODY()
public:
    FString Owner;

protected:
    TArray<UParamDescBase*> Params;

public:
    void AddParamDesc(UParamDescBase* p_desc)
    {
        Params.Add(p_desc);
    }

};