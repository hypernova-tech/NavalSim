// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Lib/ActorBase/ActorBase.h"
#include "Agent.generated.h"


USTRUCT(BlueprintType)
struct FAgentObjectDefEntry
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		FString Name;
	UPROPERTY(EditAnywhere)
		UObject* pObj;

	FAgentObjectDefEntry()
	{
		pObj = nullptr;
	}
};

USTRUCT(BlueprintType)
struct FAgentObjectDefContainer
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		TMap<FString, FAgentObjectDefEntry> Entries;


	void Add(FString name, UObject* p_part)
	{
		FAgentObjectDefEntry entry;
		entry.Name = name;
		entry.pObj = p_part;
		Entries.Add(name, entry);


	};
	FAgentObjectDefEntry Find(FString name)
	{
		auto ret = Entries.Find(name);

		if (ret) {
			FAgentObjectDefEntry def = *ret;
			return def;
		}
		else {
			return FAgentObjectDefEntry();
		}
		
	};
};

UCLASS()
class AAgent : public AActorBase
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AAgent();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	UPROPERTY(EditAnywhere)
		FAgentObjectDefContainer AgentObjectDefContainer;

	UFUNCTION(BlueprintCallable)
		void AddObjectDef(FString name, UObject *p_part);
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override; 


	UFUNCTION(BlueprintCallable)
		FAgentObjectDefEntry FindDef(FString name);
};
