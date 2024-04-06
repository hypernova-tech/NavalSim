// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/ActorBase/ActorAsPlatform.h"
#include <Lib/SOA/SOAImplementor.h>
#include <Lib/SOA/CommonSOAObservers.h>
#include <Lib/SystemManager/SystemManagerBase.h>
#include <Lib/Utils/CUtil.h>

void AActorAsPlatform::BeginPlay()
{
	Super::BeginPlay();
	ASOAImplementor::GetInstance()->Subscribe(CommonSOAObservers::PlatformKinematicObserverId, this);
	pLastData = NewObject< UPlatformKinematicData>();
	pMainMesh = CUtil::FindComponentOfTag<UStaticMeshComponent>(this, "MainMesh");
	pMainMesh->OnComponentBeginOverlap.AddDynamic(this, &AActorAsPlatform::OnOverlapBegin);
}
void AActorAsPlatform::OnOverlapBegin(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Handle the overlap event here (e.g., check the actor type and respond accordingly)
	
}
void AActorAsPlatform::Update(UCSOAObserverArgs* p_args)
{
	if (p_args->GetSubjectId() == CommonSOAObservers::PlatformKinematicObserverId) {
		UPlatformKinematicData* p_kinematic = Cast<UPlatformKinematicData>(p_args);

		if (p_kinematic != nullptr) {


			//Mutex.Lock();
			pLastData = p_kinematic;
			//p_kinematic->Copy(pLastData);
			IsDirty = true;
			//Mutex.Unlock();
			UpdateKinematicData();

		}
	}
}

void AActorAsPlatform::UpdateKinematicData()
{
	bool should_unlock = true;
	//Mutex.Lock();
	if (IsDirty) {
		if (pLastData->BotId == ActorId) {
			AMapOrigin* p_origin = ASystemManagerBase::GetInstance()->GetMapOrigin();

			FVector llh = pLastData->GetLocationLLH();
			p_origin->ChangeCenterCoordinateOnce(llh);
			FVector new_pos = p_origin->ConvertLLHToUEXYZ(llh);
			FVector euler = pLastData->GetEulerRPYDeg();

			//Mutex.Unlock();
			should_unlock = false;

			IsDirty = false;

			SetActorLocation(new_pos);
			SetActorRotation(FRotator(euler.Y, euler.Z, euler.X));
		}
		


	}

}