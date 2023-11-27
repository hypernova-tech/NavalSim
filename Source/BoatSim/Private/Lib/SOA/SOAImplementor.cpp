// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/SOA/SOAImplementor.h"



struct SSOAMessage
{
	uint32 MessageId;
	uint8 Data[2048];

};

struct SPosData
{
	double Lat;
	double Lon;
};




ASOAImplementor* ASOAImplementor::Instance = nullptr;
// Sets default values
ASOAImplementor::ASOAImplementor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Instance = this;

   

}

ASOAImplementor* ASOAImplementor::GetInstance()
{
	return Instance;
}

// Called when the game starts or when spawned
void ASOAImplementor::BeginPlay()
{
	Super::BeginPlay();
	pEthernetDataProcessorBase = (GetComponentByClass<UIDASEthernetDataProcessor>());

}

// Called every frame
void ASOAImplementor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASOAImplementor::OnReceivedNewMessage(const TArray<uint8>& data, int32 bytes_read)
{
	TotalReceivedCount++;
	int count = bytes_read;

	if (pEthernetDataProcessorBase != nullptr) {
		pEthernetDataProcessorBase->OnReceivedData(data, bytes_read);
	}
	else {
		
		//UE_LOG(LogTemp, Error, TEXT("pEthernetDataProcessorBase null"));
	}
}


void ASOAImplementor::Subscribe(int subject_no, ISOAObserver* p_observer)
{
	TSet<ISOAObserver*>& ObserverSet = ObserverDictionary.FindOrAdd(subject_no);
	ObserverSet.Add(p_observer);
}
void ASOAImplementor::Publish(int subject_no, UCSOAObserverArgs* p_args)
{
	TSet<ISOAObserver*>* found_observers = ObserverDictionary.Find(subject_no);
	if (found_observers != nullptr )
	{
		for (ISOAObserver* Observer : *found_observers)
		{
			p_args->SetSubjectId(subject_no);
			Observer->Update(p_args);
		}
	}
}