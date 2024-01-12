// Fill out your copyright notice in the Description page of Project Settings.


#include "Products/RS/BurcAirDefenceSystem/BurcAgent.h"
#include <Lib/Utils/CUtil.h>
#include <Lib/SystemManager/SystemManagerBase.h>
#include <Lib/Agent/Misc/BulletBase.h>

void ABurcAgent::BeginPlay()
{
	Super::BeginPlay();
}

void ABurcAgent::Fire_()
{
	auto p_bullet = (ABulletBase*)CloneBullet();
	p_bullet->SetLinearVelocity(TOUE(p_bullet->GetSpeed()) * GetAimDirection());
	StartShake();
}

void ABurcAgent::FireSerial_(int count, double time_interval)
{
	Fire_();
	SerialFireTimeInterval = time_interval;
	NextFireTime = CUtil::GetTimeSeconds() + SerialFireTimeInterval;
	
	IsSerialFiring = true;

	
}

void ABurcAgent::AimGun_(FVector pos)
{
	AimGimbal(pTaretGimbal, pos);
	AimGimbal(pGunGimbal, pos);

}

void ABurcAgent::AimGimbal(AGimbalBase* p_gimbal, FVector pos)
{
	FVector target_dir = pos - p_gimbal->GetActorLocation();
	target_dir.Normalize();


	// Convert to local space
	FVector local_target_dir = pTaretGimbal->GetTransform().InverseTransformVectorNoScale(target_dir);

	// Calculate yaw and pitch
	double yaw = FMath::Atan2(local_target_dir.Y, local_target_dir.X);
	double pitch = -FMath::Atan2(local_target_dir.Z, local_target_dir.X);

	// Convert radians to degrees
	yaw = FMath::RadiansToDegrees(yaw);
	pitch = FMath::RadiansToDegrees(pitch);

	p_gimbal->SetCommand_(FVector(0, pitch, yaw));
}

bool ABurcAgent::AssignTarget_(FString target_name, double duration)
{
	TargetNameName_ = target_name;
	pTarget = ASystemManagerBase::GetInstance()->FindActor(target_name);

	ASystemManagerBase::GetInstance()->SendConsoleResponse("AssignTarget called target_name: "+ target_name+" duration: "+CUtil::FloatToString(duration));
	return false;
}

FVector ABurcAgent::GetAimDirection()
{
	return FVector(pGunTip->GetActorForwardVector());
}

AActor* ABurcAgent::CloneBullet()
{
	FString bullet_name = ASystemManagerBase::GetInstance()->GenerateUniqueName("bullet");
	
	ABulletBase* p_bullet = (ABulletBase*)(ASystemManagerBase::GetInstance()->CreateActor(BulletAgentName_, bullet_name,
		pGunTip->GetActorLocation(),
		FVector::ZeroVector,
		FVector::OneVector, BulletLifeTimeSec_));

	return p_bullet;
}

void ABurcAgent::SerialFireNext()
{
	RemainingSerialFire--;
	Fire_();
	if (RemainingSerialFire > 0) {
		GetWorld()->GetTimerManager().SetTimer(SerialFireTimerHandle, this, &ABurcAgent::SerialFireNext, SerialFireTimeInterval);
	}
}
void ABurcAgent::StartShake()
{
	ShakeTimer = ShakeDuration;
	bIsShaking = true;
}

void ABurcAgent::ApplyShake()
{
	if (pGun)  // Assuming pGunTip is your nozzle component
	{
		// Create a random offset and rotation
		FVector Offset(FMath::RandRange(-ShakeIntensity, ShakeIntensity),
			FMath::RandRange(-ShakeIntensity, ShakeIntensity),
			FMath::RandRange(-ShakeIntensity, ShakeIntensity));
		FRotator RotationOffset(FMath::RandRange(-ShakeRotationIntensity, ShakeRotationIntensity),
			FMath::RandRange(-ShakeRotationIntensity, ShakeRotationIntensity),
			FMath::RandRange(-ShakeRotationIntensity, ShakeRotationIntensity));

		// Apply the offset and rotation to the nozzle
		pGun->AddActorLocalOffset(Offset);
		pGun->AddActorLocalRotation(RotationOffset);
	}
}
void ABurcAgent::OnPreStep(float DeltaTime)
{
	Super::OnPreStep(DeltaTime);
	pGunTip = ASystemManagerBase::GetInstance()->FindActor(GunTipName_);
	pGun = ASystemManagerBase::GetInstance()->FindActor(GunName_);
	pTaretGimbal = (AGimbalBase*)ASystemManagerBase::GetInstance()->FindActor(TaretGimbalName_);
	pEOGimbal = (AGimbalBase*)ASystemManagerBase::GetInstance()->FindActor(EOGimbalName_);
	pGunGimbal = (AGimbalBase*)ASystemManagerBase::GetInstance()->FindActor(GunGimbalName_);
}

void ABurcAgent::OnStep(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsShaking)
	{
		ShakeTimer -= DeltaTime;
		if (ShakeTimer <= 0.0f)
		{
			bIsShaking = false;
			// Reset nozzle position/rotation if needed
		}
		else
		{
			ApplyShake();
		}
	}

	if (IsSerialFiring) {
		if (CUtil::GetTimeSeconds() >= NextFireTime) {
			Fire_();
			NextFireTime = CUtil::GetTimeSeconds() + SerialFireTimeInterval;
			RemainingSerialFire--;

			if (RemainingSerialFire == 0) {
				IsSerialFiring = false;
			}
		}
	}

	if (pTarget) {
		AimGun_(pTarget->GetActorLocation());
	}
	
}

