// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/ActorBase/ActorBase.h"
#include <Lib/SystemManager/SystemManagerBase.h>
#include <Lib/Utils/CUtil.h>
#include <Lib/Math/CMath.h>
#include "GameFramework/Actor.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

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
	auto pSys = ASystemManagerBase::GetInstance();
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


	if (KeepPhysicsEnabledComponentRelativeTransformConstant) {
		TArray<UStaticMeshComponent*> Components;
		
		GetComponents<UStaticMeshComponent>(Components);

		for (UStaticMeshComponent* Component : Components)
		{
			if (Component->IsSimulatingPhysics()) {
				// Do something with each component
				CUtil::ReattachComponentToActor(Component, this);
			}
			
		}
	}
	
	
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
	FVector size = CUtil::GetActorSizeInLocalAxes(this);
	SetActorComputedActorSizeMeter(TOW(size));
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
	UpdateActorVelocityMetersPerSec();
	UpdateActorAngularVelocityRPYDegPerSec();
	UpdateMotionLog();
	if (!IsExternalUpdate) {
		if (CheckAffinity() && !Suppressed) {
			
			OnStep(DeltaTime);
		}
	}
	
}

void AActorBase::SetActorId(int val)
{
	ActorId = val;
}
int AActorBase::GetActorId()
{
	return ActorId;
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
	p_save_loader->AppendOption(line, CCLICommandManager::ActorId, GetActorId());
	p_save_loader->AddLine(line);

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
	p_save_loader->AppendOption(line, CCLICommandManager::IsBlockingObject, GetIsBlockingObject());
	p_save_loader->AddLine(line);

	
	line = p_save_loader->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
	p_save_loader->AppendOption(line, CCLICommandManager::TempratureKelvin, TempratureKelvin);
	p_save_loader->AddLine(line);

	line = p_save_loader->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
	p_save_loader->AppendOption(line, CCLICommandManager::IsHeatSource, IsHeatSource_);
	p_save_loader->AddLine(line);

	line = p_save_loader->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
	p_save_loader->AppendOption(line, CCLICommandManager::AnnotationId, AnnotationId_);
	p_save_loader->AddLine(line);

	line = p_save_loader->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
	p_save_loader->AppendOption(line, CCLICommandManager::AnnotateChildren, AnnotateChildrenActors_);
	p_save_loader->AddLine(line);

	line = p_save_loader->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
	p_save_loader->AppendOption(line, CCLICommandManager::AnnotateChildren, AnnotateChildrenActors_);
	p_save_loader->AddLine(line);


	/*
	line = p_save_loader->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
	p_save_loader->AppendOption(line, CCLICommandManager::Property, GET_MEMBER_NAME_CHECKED(AActorBase, IsHeatSource_).ToString());
	p_save_loader->AppendOption(line, CCLICommandManager::PropertyValue, IsHeatSource_);
	p_save_loader->AddLine(line);

	line = p_save_loader->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
	p_save_loader->AppendOption(line, CCLICommandManager::Property, GET_MEMBER_NAME_CHECKED(AActorBase, AnnotationId_).ToString());
	p_save_loader->AppendOption(line, CCLICommandManager::PropertyValue, AnnotationId_);
	p_save_loader->AddLine(line);
	*/

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
	line = p_save_loader->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
	p_save_loader->AppendOption(line, CCLICommandManager::SharedMemName, ProtocolConverterSharedMemoryName);
	p_save_loader->AddLine(line);

	line = p_save_loader->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
	p_save_loader->AppendOption(line, CCLICommandManager::GStreamerIP, GStreamerIP);
	p_save_loader->AddLine(line);

	line = p_save_loader->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
	p_save_loader->AppendOption(line, CCLICommandManager::GStreamerPort, GStreamerPort);
	p_save_loader->AddLine(line);

	line = p_save_loader->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
	p_save_loader->AppendOption(line, CCLICommandManager::MotionLogEnabled, IsMotionLogEnabled);
	p_save_loader->AddLine(line);
}

void AActorBase::SaveJSON(CJsonDataContainer& data)
{
	data.Add(CCLICommandManager::Name, GetName());
	data.Add(CCLICommandManager::ActorId, (GetActorId()));
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
	data.Add(CCLICommandManager::IsBlockingObject, GetIsBlockingObject());


	data.Add(CCLICommandManager::TempratureKelvin, TempratureKelvin);
	data.Add(CCLICommandManager::IsHeatSource, IsHeatSource_);
	data.Add(CCLICommandManager::AnnotationId, AnnotationId_);
	data.Add(CCLICommandManager::AnnotateChildren, AnnotateChildrenActors_);
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

	data.Add(CCLICommandManager::SharedMemName, ProtocolConverterSharedMemoryName);
	data.Add(CCLICommandManager::GStreamerIP, GStreamerIP);
	data.Add(CCLICommandManager::GStreamerPort, GStreamerPort);

	data.Add(CCLICommandManager::MotionLogEnabled, IsMotionLogEnabled);
	


}

CMotionLogger* AActorBase:: GetMotionLogger()
{
	return &MotionLogger;
}

void AActorBase::SetIsMotionLogEnabled(bool val)
{
	IsMotionLogEnabled = val;
}
bool AActorBase::GetIsMotionLogEnabled()
{
	return IsMotionLogEnabled;
}

void AActorBase::SetMotionLogLifeTimeSec(double val)
{
	MotionLogLifeTimeSec = val;
}

double AActorBase::GetMotionLogLifeTimeSec()
{
	return MotionLogLifeTimeSec;
}

void AActorBase::SetMotionLogMinDistanceMeter(double val)
{
	MotionLogMinDistanceMeter = val;
}
double AActorBase::GetMotionLogMinDistanceMeter()
{
	return MotionLogMinDistanceMeter;
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

void AActorBase::SetActorComputedActorSizeMeter(FVector val)
{
	ActorComputedSizeMeter = val;
}
FVector AActorBase::GetActorComputedActorSizeMeter()
{
	return ActorComputedSizeMeter;
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

bool AActorBase::GetIsHeatSource_()
{
	return IsHeatSource_;
}

void AActorBase::SetIsHeatSource_(bool val)
{
	IsHeatSource_ = val;
}

int AActorBase::GetAnnotationId_()
{
	return AnnotationId_;
}

void AActorBase::SetAnnotationId_(int val)
{
	AnnotationId_ = val;
}


void  AActorBase::SetAnnotateChildrenActors_(bool val)
{
	AnnotateChildrenActors_ = val;
}

bool AActorBase:: GetAnnotateChildrenActors_()
{
	return AnnotateChildrenActors_;
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
			pcomp->SetCustomDepthStencilValue(0);
		}


	}
}
void AActorBase::UpdateAnnotation(bool is_enabled)
{
	AActorBase::HandleAnnotation(this, is_enabled, AnnotationId_);
}

bool AActorBase::CanMoveTowards(FVector dir)
{
	return true;
}
bool AActorBase::IsColldingWithBlockingObject()
{

	return false;

#if false
	FHitResult HitResult;
	FVector Origin, BoxExtent;
	GetActorBounds(false, Origin, BoxExtent);

	// Define start and end points of the ray
	FVector Start = Origin + BoxExtent.UnitX;
	FVector End = Start + (WorldDirection * TOUE(20000));  // MaxTraceDistance is up to you, e.g., 10000.0f

	//CUtil::DrawDebugRay(GetWorld(), Start, End, FColor::Red, 10, 0.75);

	// Perform the line trace
	// The ECC_Visibility channel will typically work, but you can change this if necessary
	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		Start,
		End,
		ECC_GameTraceChannel1
	);

	if (bHit)
	{
		// HitResult now contains information about what was hit
		AActor* ClickedActor = HitResult.GetActor();
		bool gizmo_tag = ClickedActor->ActorHasTag("Gizmo");


		if (ClickedActor && !gizmo_tag)
		{
			CurrAxis = pGizmoActor->GetAxis(HitResult.GetComponent());
			CUtil::DebugLogScreen(ClickedActor->GetName());
			return ClickedActor;


		}


	}
	else {

	}

#endif
}

FVector AActorBase::GetPositionLatLongHeightMSL()
{
	auto pos = GetActorLocation();
	FVector llh = ASystemManagerBase::GetInstance()->GetMapOrigin()->ConvertUEXYZToLLH(pos);

	return llh;
}
FVector AActorBase:: GetPositionXYZMeters()
{
	auto pos = GetActorLocation();
	return TOW(pos);
}
FVector AActorBase::GetActorVelocityMetersPerSec()
{
	return ActorVelocityMetersPerSec;
}
bool AActorBase::GetIsBlockingObject()
{
	return IsBlockingObject;
}
void AActorBase::SetIsBlockingObject(bool val)
{
	IsBlockingObject = val;
}
FVector AActorBase::UpdateActorVelocityMetersPerSec()
{
	FVector Velocity(0.0f, 0.0f, 0.0f);

	// Check for physics-based movement
	UPrimitiveComponent* PrimaryPhysComponent = Cast<UPrimitiveComponent>(this->GetRootComponent());
	if (PrimaryPhysComponent && PrimaryPhysComponent->IsSimulatingPhysics())
	{
		ActorVelocityMetersPerSec = PrimaryPhysComponent->GetComponentVelocity();
		return TOW(ActorVelocityMetersPerSec);
	}

	// Check for a character movement component
	ACharacter* Character = Cast<ACharacter>(this);
	if (Character && Character->GetCharacterMovement())
	{
		ActorVelocityMetersPerSec = Character->GetCharacterMovement()->Velocity;
		return TOW(ActorVelocityMetersPerSec);
	}

	
	double CurrentTime = FApp::GetCurrentTime();
	FVector CurrentPosition = this->GetActorLocation();
	if (CurrentTime != VelPreviousTime) // Avoid division by zero
	{
			ActorVelocityMetersPerSec = TOW((CurrentPosition - PreviousPosition) / (CurrentTime - VelPreviousTime));
		    PreviousPosition = CurrentPosition;
			VelPreviousTime = CurrentTime;
		}

	return ActorVelocityMetersPerSec;
	
}

void AActorBase::UpdateMotionLog()
{
	if (!IsMotionLogEnabled) {
		return;
	}
	double curr_time_sec = FApp::GetCurrentTime();
	MotionLogger.Log(curr_time_sec, GetActorLocation(), MotionLogMinDistanceMeter);
	MotionLogger.Update(curr_time_sec, MotionLogLifeTimeSec);
	
}

FVector AActorBase::GetActorAngularVelocityRPYDegPerSec()
{
	return ActorAngularVelocityRPYDegPerSec;
}


FVector AActorBase::UpdateActorAngularVelocityRPYDegPerSec()
{

	// Check for physics-based rotation
	UPrimitiveComponent* PrimaryPhysComponent = Cast<UPrimitiveComponent>(this->GetRootComponent());
	if (PrimaryPhysComponent && PrimaryPhysComponent->IsSimulatingPhysics())
	{
		ActorAngularVelocityRPYDegPerSec = PrimaryPhysComponent->GetPhysicsAngularVelocityInDegrees();
		return ActorAngularVelocityRPYDegPerSec;
	}


	 double CurrentTime = FApp::GetCurrentTime();
	 FRotator CurrentRotation = this->GetActorRotation();
	 if (CurrentTime != AngVelPreviousTime) // Avoid division by zero
	 {
	     FRotator DeltaRotation = CurrentRotation - PreviousRotation;
	     // Convert rotation delta to angular velocity (degrees per second)
		 ActorAngularVelocityRPYDegPerSec = DeltaRotation.GetNormalized().Euler() / (CurrentTime - AngVelPreviousTime);
	     PreviousRotation = CurrentRotation;
		 AngVelPreviousTime = CurrentTime;
	 }

	return ActorAngularVelocityRPYDegPerSec;
}
