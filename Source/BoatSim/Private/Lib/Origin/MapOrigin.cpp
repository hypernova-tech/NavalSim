// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/Origin/MapOrigin.h"
#include <Lib/Math/CMath.h>
#include <Lib/Console/CCLICommandManager.h>

// Sets default values
AMapOrigin::AMapOrigin()
{

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	

}

FVector AMapOrigin::GetMapOriginLLH()
{
	return CenterLLH_;
}

void AMapOrigin::SetMapOriginLLH(FVector llh)
{
	CenterLLH_ = llh;
}

// Called when the game starts or when spawned
void AMapOrigin::BeginPlay()
{
	Super::BeginPlay();
	Rename(TEXT("jeoref"));
	pGeoReferencingSystem = AGeoReferencingSystem::GetGeoReferencingSystem(GetWorld());
	//UpdateGeoreferencingCenter();
	//Test();
}

void AMapOrigin::UpdateGeoreferencingCenter()
{
	pGeoReferencingSystem->OriginLatitude = CenterLLH_.X;
	pGeoReferencingSystem->OriginLongitude = CenterLLH_.Y;
	pGeoReferencingSystem->OriginAltitude = CenterLLH_.Z;
}

void AMapOrigin::Test()
{
	//pGeoReferencingSystem->OriginLatitude = 30;
	//pGeoReferencingSystem->OriginLongitude = 40;
	//pGeoReferencingSystem->OriginAltitude = 0;


	//pGeoReferencingSystem->ApplySettings();


	FVector uepos;
	FVector geo = FVector(30, 40, 0);
	pGeoReferencingSystem->GeographicToEngine(geo, uepos);
	pGeoReferencingSystem->ProjectedToEngine(geo, uepos);
	pGeoReferencingSystem->GeographicToProjected(geo, uepos);

	geo = FVector(31, 40, 0);
	pGeoReferencingSystem->GeographicToEngine(geo, uepos);
	pGeoReferencingSystem->ProjectedToEngine(geo, uepos);
	pGeoReferencingSystem->GeographicToProjected(geo, uepos);

	geo = FVector(30, 41, 0);
	pGeoReferencingSystem->GeographicToEngine(geo, uepos);
	pGeoReferencingSystem->ProjectedToEngine(geo, uepos);
	pGeoReferencingSystem->GeographicToProjected(geo, uepos);


	geo = FVector(30, 40, 1000);
	pGeoReferencingSystem->GeographicToEngine(geo, uepos);
	pGeoReferencingSystem->ProjectedToEngine(geo, uepos);
	pGeoReferencingSystem->GeographicToProjected(geo, uepos);

	geo = FVector(31, 40, -1000);
	pGeoReferencingSystem->GeographicToEngine(geo, uepos);
	pGeoReferencingSystem->ProjectedToEngine(geo, uepos);
	pGeoReferencingSystem->GeographicToProjected(geo, uepos);

	geo = FVector(30, 41, 2000);
	pGeoReferencingSystem->GeographicToEngine(geo, uepos);
	pGeoReferencingSystem->ProjectedToEngine(geo, uepos);
	pGeoReferencingSystem->GeographicToProjected(geo, uepos);
}

// Called every frame
void AMapOrigin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//Test();

}

FVector AMapOrigin::ConvertLLHToUEXYZ(FVector pos)
{
	FVector ret = FVector::Zero();

	FVector diff_vec = pos - CenterLLH_;
	FVector xyz = FVector::RightVector * diff_vec.Y * 111e3 + FVector::ForwardVector * 111e3 * diff_vec.X + FVector::UpVector * diff_vec.Z;
	xyz = TOUE(xyz);
	ret = GetActorLocation() + xyz;
	//ret.Z += pos.Z;

	//pGeoReferencingSystem->GeographicToEngine(pos, ret);

	return ret;
}

FVector AMapOrigin::ConvertUEXYZToLLH(FVector xyz)
{

	FVector diff_xyz = xyz - GetActorLocation();

	FVector llh = FVector::RightVector * diff_xyz.Y / 111e3 + FVector::ForwardVector / 111e3 * diff_xyz.X + FVector::UpVector * TOW(diff_xyz.Z);

	llh += CenterLLH_;

	//pGeoReferencingSystem->GeographicToEngine(pos, ret);

	return llh;
}

void AMapOrigin::ChangeCenterCoordinateOnce(FVector new_center_llh)
{
	if (!IsCenterChangedOnce) {
		if (!CMath::IsZero(new_center_llh, 1e-6)) {
			CenterLLH_ = new_center_llh;
			IsCenterChangedOnce = true;
		}
	}
}

void AMapOrigin::Save(ISaveLoader* p_save_loader)
{
	Super::Save(p_save_loader);

	FString line;
	line = p_save_loader->CreateCommand(CCLICommandManager::SetCommand);
	p_save_loader->AppendOption(line, CCLICommandManager::Name, GetName());
	p_save_loader->AppendOption(line, CCLICommandManager::MapOriginLat, CenterLLH_.X);
	p_save_loader->AddLine(line);

	line = p_save_loader->CreateCommand(CCLICommandManager::SetCommand);
	p_save_loader->AppendOption(line, CCLICommandManager::Name, GetName());
	p_save_loader->AppendOption(line, CCLICommandManager::MapOriginLon, CenterLLH_.Y);
	p_save_loader->AddLine(line);

	line = p_save_loader->CreateCommand(CCLICommandManager::SetCommand);
	p_save_loader->AppendOption(line, CCLICommandManager::Name, GetName());
	p_save_loader->AppendOption(line, CCLICommandManager::MapOriginHeight, CenterLLH_.Z);
	p_save_loader->AddLine(line);
	
}

void AMapOrigin::SaveJSON(CJsonDataContainer& data)
{
	Super::SaveJSON(data);

	data.Add(CCLICommandManager::MapOriginLat, CenterLLH_.X);
	data.Add(CCLICommandManager::MapOriginLon, CenterLLH_.Y);
	data.Add(CCLICommandManager::MapOriginHeight, CenterLLH_.Z);
}

