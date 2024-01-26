// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/ActorBase/ActorBase.h"
#include <Lib/SystemManager/SystemManagerBase.h>
#include <Lib/Utils/CUtil.h>
#include <Lib/Math/CMath.h>

// Sets default values
AActorBase::AActorBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//SetBaseActor(this);
	
	

}

void AActorBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	if (UsePrefixName) {
		Rename(*(PrefixName + GetName()));
	}
}

// Called when the game starts or when spawned
void AActorBase::BeginPlay()
{
	Super::BeginPlay();
	AActor *p_owner = CUtil::GetTopParent(this);

	if (IsRegisterEnabled) {
		if (p_owner == nullptr) {
			ASystemManagerBase::GetInstance()->RegisterActor(this);
		}
		else {

			ASystemManagerBase::GetInstance()->RegisterActor(this); //todo fixme

		}
	}
	

	pCommIF = GetComponentByClass<UGenericCommIF>();

	
	
}
void AActorBase::AddManuallyAttach(USceneComponent* p_comp)
{
	p_comp->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	ManuallyAttachComponents.Add(p_comp);
}

void AActorBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
}
void AActorBase:: OnPreStep(float DeltaTime)
{

}
void AActorBase::OnStep(float DeltaTime)
{


}
void AActorBase::OnPreStepScenarioMode(float DeltaTime)
{

}
void AActorBase::OnStepScenarioMode(float DeltaTime)
{
}

bool AActorBase::CheckAffinity()
{

	return ASystemManagerBase::GetInstance()->GetInstanceNo() == AffinityInstanceId;
	


}

void AActorBase::OnActorDisabled()
{
}

void AActorBase::OnActorEnabled()
{
}



// Called every frame
void AActorBase::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);
	if (!IsExternalUpdate) {
		if (CheckAffinity() && !Suppressed) {
			
			OnStep(DeltaTime);
		}
	}
	
}

void AActorBase::SetEnabled(bool val)
{
	Enabled = val;
	if (val) {
		OnActorEnabled();
	}else {
		OnActorDisabled();
	}
}
bool AActorBase::GetEnabled()
{
	return Enabled;
}

void AActorBase::SetSuppressed(bool val)
{
	Suppressed = val;
}

bool AActorBase::GetSuppressed()
{
	return Suppressed;
}

void AActorBase::SetStandaloneModeEnabled(bool val)
{
	StandaloneModeEnabled = val;
}

bool AActorBase::GetStandaloneModeEnabled()
{
	return StandaloneModeEnabled;
}

bool AActorBase::GetIsExternalUpdateScenarioMode()
{
	return IsExternalUpdateScenarioMode;
}

void AActorBase::ExternalUpdate(float DeltaTime)
{
	
	if (CheckAffinity() && !Suppressed) {
		if (ExternalUpdateCount == 0) {
			OnPreStep(DeltaTime);
		}
		else {
			OnStep(DeltaTime);
		}
		ExternalUpdateCount++;
		
	}
	
}

void AActorBase::ExternalUpdateScenarioMode(float DeltaTime)
{
	if (CheckAffinity()) {
		if (ExternalUpdateCountScenarioMode == 0) {
			OnPreStepScenarioMode(DeltaTime);
		}
		else {
			OnStepScenarioMode(DeltaTime);
		}
		ExternalUpdateCountScenarioMode++;
	}
}

void AActorBase::SetAffinityInstanceId(int val)
{
	AffinityInstanceId = val;
}

int AActorBase::GetAffinityInstanceId()
{
	return AffinityInstanceId;
}

FString AActorBase::GetBlueprintName()
{
	return BlueprintName;
}

void AActorBase::Save(ISaveLoader* p_save_loader)
{
	FString line;
	if (IsSaveLoadCreateEnabled) {
		line = p_save_loader->CreateCommand(CCLICommandManager::CreateCommand);
		p_save_loader->AppendOption(line, CCLICommandManager::Name, GetName());
		p_save_loader->AppendOption(line, CCLICommandManager::Bp, GetBlueprintName());
		p_save_loader->AppendOption(line, CCLICommandManager::Tick, TickRequiredForCreation);

		p_save_loader->AddLine(line);
	}

	auto parent = CUtil::GetParentActor(this);

	if (parent != nullptr) {
		line = p_save_loader->CreateCommand(CCLICommandManager::SetCommand);
		p_save_loader->AppendOption(line, CCLICommandManager::Name, GetName());
		p_save_loader->AppendOption(line, CCLICommandManager::Parent, parent->GetName());
		p_save_loader->AddLine(line);
	}

	line = p_save_loader->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
	p_save_loader->AppendOption(line, CCLICommandManager::Instance, AffinityInstanceId);
	p_save_loader->AddLine(line);

	line = p_save_loader->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
	p_save_loader->AppendOption(line, CCLICommandManager::Suppressed, Suppressed);
	p_save_loader->AddLine(line);

	line = p_save_loader->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
	p_save_loader->AppendOption(line, CCLICommandManager::StandaloneModeEnabled, StandaloneModeEnabled);
	p_save_loader->AddLine(line);

	line = p_save_loader->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
	p_save_loader->AppendOption(line, CCLICommandManager::Position, TOW(GetActorLocation()));
	p_save_loader->AddLine(line);

	line = p_save_loader->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
	p_save_loader->AppendOption(line, CCLICommandManager::Rotation, CMath::GetActorEulerAnglesRPY(this));
	p_save_loader->AddLine(line);

	line = p_save_loader->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
	p_save_loader->AppendOption(line, CCLICommandManager::Scale, GetActorScale3D());
	p_save_loader->AddLine(line);

	line = p_save_loader->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
	p_save_loader->AppendOption(line, CCLICommandManager::TempratureKelvin, TempratureKelvin);
	p_save_loader->AddLine(line);

	line = p_save_loader->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
	p_save_loader->AppendOption(line, CCLICommandManager::Property, GET_MEMBER_NAME_CHECKED(AActorBase, IsHeatSource_).ToString());
	p_save_loader->AppendOption(line, CCLICommandManager::PropertyValue, IsHeatSource_);
	p_save_loader->AddLine(line);

	line = p_save_loader->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
	p_save_loader->AppendOption(line, CCLICommandManager::Property, GET_MEMBER_NAME_CHECKED(AActorBase, AnnotationId_).ToString());
	p_save_loader->AppendOption(line, CCLICommandManager::PropertyValue, AnnotationId_);
	p_save_loader->AddLine(line);


	if (pCommIF) {
		auto connections = pCommIF->GetConnectionsInfo();
		int ind = 0;
		for (auto conn : connections) {
			if (ind == 0) {
				SaveConnection(line, CCLICommandManager::IPAddr1, CCLICommandManager::LocalPort1, CCLICommandManager::RemotePort1, p_save_loader, conn.ConnectionInfo);

			}
			else if (ind == 1) {
				SaveConnection(line, CCLICommandManager::IPAddr2, CCLICommandManager::LocalPort2, CCLICommandManager::RemotePort2, p_save_loader, conn.ConnectionInfo);
			}
			ind++;
		}
	}
}

void AActorBase::SaveJSON(CJsonDataContainer& data)
{
	data.Add(CCLICommandManager::Name, GetName());
	data.Add(CCLICommandManager::Instance, (AffinityInstanceId));
	data.Add(CCLICommandManager::Suppressed, (Suppressed));
	data.Add(CCLICommandManager::Enabled, (Enabled));
	data.Add(CCLICommandManager::StandaloneModeEnabled, (StandaloneModeEnabled));
	data.Add(CCLICommandManager::Active, CUtil::GetIsActorTickEnabled(this));
	data.Add(CCLICommandManager::Bp, BlueprintName);
	auto parent = CUtil::GetParentActor(this);
	if (parent != nullptr) {
		data.Add(CCLICommandManager::Parent, parent->GetName());
	}
	else {
		data.Add(CCLICommandManager::Parent, FString(""));
	}
	
	data.Add(CCLICommandManager::Position, TOW(GetActorLocation()));
	data.Add(CCLICommandManager::Rotation, (CMath::GetActorEulerAnglesRPY(this)));
	data.Add(CCLICommandManager::Scale, (GetActorScale3D()));
	data.Add(CCLICommandManager::TempratureKelvin, TempratureKelvin);
	data.Add(CCLICommandManager::IsHeatSource, IsHeatSource_);
	data.Add(CCLICommandManager::AnnotationId, AnnotationId_);
	if (pCommIF) {
		auto connections = pCommIF->GetConnectionsInfo();
		int ind = 0;
		for (auto conn : connections) {
			if (ind == 0) {
				data.Add(CCLICommandManager::IPAddr1, conn.ConnectionInfo.IpAddr);
				data.Add(CCLICommandManager::LocalPort1, conn.ConnectionInfo.LocalPort);
				data.Add(CCLICommandManager::RemotePort1, conn.ConnectionInfo.RemotePort);
			}
			else	if (ind == 1) {
				data.Add(CCLICommandManager::IPAddr2, conn.ConnectionInfo.IpAddr);
				data.Add(CCLICommandManager::LocalPort2, conn.ConnectionInfo.LocalPort);
				data.Add(CCLICommandManager::RemotePort2, conn.ConnectionInfo.RemotePort);
			}
			ind++;
		}
	}
}

void AActorBase::ShowActorGizmo(bool val)
{
	if (ActorGizmo) {
		CUtil::SetActorActive(ActorGizmo, val);
	}
	
}

void AActorBase::Bake()
{
}

void AActorBase::OnActorPredestroy()
{
}

void AActorBase::SetIsSaveEnabled(bool val)
{
	IsSaveEnabled = val;
}

bool AActorBase::GetIsSaveEnabled()
{
	return IsSaveEnabled;
}

void AActorBase::SetActorPosition(FVector pos)
{
	SetActorLocation(pos);
}

void AActorBase::SetActorRelPosition(FVector pos)
{
	SetActorRelativeLocation(pos);
}

void AActorBase::SetActorRot(FVector rpy_deg)
{
	CMath::SetActorRotation(this, rpy_deg);
}

void AActorBase::SetActorRelRot(FVector rpy_deg)
{
	CMath::SetActorRelativeRotation(this, rpy_deg);
}

bool AActorBase::GetConnnectionInfo(INT32S ind, SConnectionInfo& info)
{
	if (pCommIF) {
		auto connections = pCommIF->GetConnectionsInfo();
		if (ind < connections.Num()) {
			info = connections[ind].ConnectionInfo;
			return true;
		}
	}
	else {
		return false;
	}

	return false;
}

bool AActorBase::SetConnnectionInfo(INT32S ind, SConnectionInfo info)
{
	if (pCommIF) {
		auto connections = pCommIF->GetConnectionsInfo();
		if (ind < connections.Num()) {
			pCommIF->SetConnectionInfo(ind, info);
			return true;
		}
	}
	else {
		return false;
	}

	return false;
}
void AActorBase::SaveConnection(FString& line, FString ip_addr_param, FString local_port_param, FString remote_port_param, ISaveLoader* p_save_loader, SConnectionInfo& conn)
{
	line = p_save_loader->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
	p_save_loader->AppendOption(line, ip_addr_param, conn.IpAddr);
	p_save_loader->AddLine(line);

	line = p_save_loader->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
	p_save_loader->AppendOption(line, local_port_param, conn.LocalPort);
	p_save_loader->AddLine(line);

	line = p_save_loader->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
	p_save_loader->AppendOption(line, remote_port_param, conn.RemotePort);
	p_save_loader->AddLine(line);
}

void AActorBase::AddChild(USceneComponent* p_comp)
{
	// Assuming MyComponent is a valid UActorComponent pointer and MyActor is a valid AActor pointer
	if (p_comp && GetRootComponent()) {
		p_comp->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	}
	
	

}

bool AActorBase::GetIsHeatSource()
{
	return IsHeatSource_;
}

double AActorBase::GetTempratureKelvin()
{
	return TempratureKelvin;
}

void AActorBase::SetTempratureKelvin(double val)
{
	TempratureKelvin = val;
}

int AActorBase::GetAnnotationId_()
{
	return AnnotationId_;
}

void AActorBase::SetAnnotationId_(int val)
{
	AnnotationId_ = val;
}
void AActorBase::HandleAnnotation(AActor* p_actor, bool is_enabled, int annotation_id)
{
	TArray<UMeshComponent*> mesh_components;
	p_actor->GetComponents<UMeshComponent>(mesh_components);

	for (UMeshComponent* pcomp : mesh_components)
	{
		if (is_enabled) {

			pcomp->SetCustomDepthStencilValue(annotation_id);
			pcomp->SetRenderCustomDepth(true);

		}
		else {
			pcomp->SetRenderCustomDepth(false);
			pcomp->SetCustomDepthStencilValue(annotation_id);
		}


	}
}
void AActorBase::UpdateAnnotation(bool is_enabled)
{
	AActorBase::HandleAnnotation(this, is_enabled, AnnotationId_);
}
