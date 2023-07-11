// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ISOAObserver.h"
#include "GameFramework/Actor.h"
#include "Products/IDAS/EthernetProcessor/IDASEthernetDataProcessor.h"
#include "SOAImplementor.generated.h"


UCLASS()
class ASOAImplementor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASOAImplementor();
	UFUNCTION(BlueprintCallable)
		virtual void OnReceivedNewMessage(const TArray<uint8>& InByteArray);


	static ASOAImplementor* GetInstance();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	UPROPERTY(EditAnywhere)
		UIDASEthernetDataProcessor* pEthernetDataProcessorBase;
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	void Subscribe(int subject_no, ISOAObserver* p_observer);
	void Publish(int subject_no, UCSOAObserverArgs* p_args);

private:
	static ASOAImplementor* Instance;
	TMap<int32, TSet<ISOAObserver*>> ObserverDictionary;
	int TotalReceivedCount = 0;
	
};


