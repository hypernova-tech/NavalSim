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
	pMainMesh->OnComponentEndOverlap.AddDynamic(this, &AActorAsPlatform::OnOverlapEnd);


}
void AActorAsPlatform::OnOverlapBegin(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Handle the overlap event here (e.g., check the actor type and respond accordingly)
	if (OtherActor == this) {
		return;
	}
	if (OtherActor->IsA<AActorBase>()) {
		AActorBase* p_base = (AActorBase*)OtherActor;
		if (p_base->GetIsBlockingObject()) {
			CollidedActors.Add(p_base);
		}
		
	}
}

void AActorAsPlatform::OnOverlapEnd(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->IsA<AActorBase>()) {
		CollidedActors.Remove(OtherActor);
	}
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

bool AActorAsPlatform::IsColldingWithBlockingObject()
{
	return CollidedActors.Num() > 0;
}

bool AActorAsPlatform:: CanMoveTowards(FVector dir)
{
	bool ret = false;

	for (auto actor : CollidedActors) {
		FVector collision_dir = actor->GetActorLocation() - GetActorLocation();
		collision_dir.Normalize();
		if (collision_dir.Dot(dir) > 0) {
			return false;
		}
	}

	return true;


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

			FVector dir = new_pos - GetActorLocation();
			dir.Normalize();

			//Mutex.Unlock();
			should_unlock = false;

			IsDirty = false;
			if (!IsColldingWithBlockingObject()) {
				SetActorLocation(new_pos);
				SetActorRotation(FRotator(euler.Y, euler.Z, euler.X));
			}
			else {
				if (CanMoveTowards(dir)) {
					SetActorLocation(new_pos);
					SetActorRotation(FRotator(euler.Y, euler.Z, euler.X));
				}
			}
	
		}
		


	}

}