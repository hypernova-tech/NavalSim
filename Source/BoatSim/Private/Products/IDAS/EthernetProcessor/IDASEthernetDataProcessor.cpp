// Fill out your copyright notice in the Description page of Project Settings.


#include "Products/IDAS/EthernetProcessor/IDASEthernetDataProcessor.h"
#include "Lib/SOA/CSOAObserverArgs.h"
#include <Lib/SOACommonObserverArgs/PlatformKinematicData.h>
#include <Lib/SOA/SOAImplementor.h>
#include <Lib/SOA/CommonSOAObservers.h>
#pragma pack(push, 1)
struct  SIdasAsToIdasGERaw
{
	uint8 BodId;
	int32 BotPosLat;
	int32 BotPosLon;
	int16 VelNorthNed;
	int16 VelEastNed;
	int16 VelDownNed;
	int32 BotPosNorth;
	int32 BotPosEast;
	int32 BotPosDown;
	int16 EulerPhiDeg;
	int16 EulerThetaDeg;
	int16 EulerPsiDeg;
	int16 BodyVelSurge;
	int16 BodyVelSway;
	int16 BodyVelHeave;
	uint8 Checksum;
};
#pragma pack(pop)

#define RADTODEG							57.295779513082323 
#define DEGTORAD							0.017453292519943
#define LAT_SCALE_FACTOR					1e-7
#define LON_SCALE_FACTOR					(1e-7)
#define NED_VELOCITY_SCALE_FACTOR			(2e-15*1e-2)
#define BOT_POS_SCALE_FACTOR				(2e-31*1e-2)
#define BOT_EULER_ANGLE_SCALE_FACTOR		(1e-4*RADTODEG)
#define BOT_BODY_VEL_SCALE_FACTOR			(2e-15*1e-2)

void UIDASEthernetDataProcessor::OnReceivedData(const TArray<uint8>& received_data)
{
	Super::OnReceivedData(received_data);

	const SIdasAsToIdasGERaw* idas_raw = reinterpret_cast<const SIdasAsToIdasGERaw*>(received_data.GetData());
	//UE_LOG(LogTemp, Warning, TEXT("Receved byte count: %s %s %s"), *FString::FromInt(TotalReceivedCount), *FString::FromInt(MyStructArray->MessageId), *FString::FromInt(count));
	UPlatformKinematicData* p_kinematic_data_args = NewObject< UPlatformKinematicData>();

	p_kinematic_data_args->BotPosLatDeg = idas_raw->BotPosLat * LAT_SCALE_FACTOR;
	p_kinematic_data_args->BotPosLonDeg = idas_raw->BotPosLon * LAT_SCALE_FACTOR;

	p_kinematic_data_args->VelNorthNedMeterSec = idas_raw->VelNorthNed * NED_VELOCITY_SCALE_FACTOR;
	p_kinematic_data_args->VelEastNedMeterPerSec = idas_raw->VelEastNed * NED_VELOCITY_SCALE_FACTOR;
	

	p_kinematic_data_args->BotPosNorthMeter = idas_raw->BotPosNorth * BOT_POS_SCALE_FACTOR;
	p_kinematic_data_args->BotPosEastMeter = idas_raw->BotPosEast * BOT_POS_SCALE_FACTOR;
	p_kinematic_data_args->BotPosDownMeter = idas_raw->BotPosDown * BOT_POS_SCALE_FACTOR;

	p_kinematic_data_args->EulerPhiDeg = idas_raw->EulerPhiDeg * BOT_EULER_ANGLE_SCALE_FACTOR;
	p_kinematic_data_args->EulerThetaDeg = idas_raw->EulerThetaDeg * BOT_EULER_ANGLE_SCALE_FACTOR;
	p_kinematic_data_args->EulerPsiDeg = idas_raw->EulerPsiDeg * BOT_EULER_ANGLE_SCALE_FACTOR;

	p_kinematic_data_args->BodyVelSurgeMeterPerSec	= idas_raw->BodyVelSurge * BOT_BODY_VEL_SCALE_FACTOR;
	p_kinematic_data_args->BodyVelSwayMeterPerSec	= idas_raw->BodyVelSway * BOT_BODY_VEL_SCALE_FACTOR;
	p_kinematic_data_args->BodyVelHeaveMeterPerSec  = idas_raw->BodyVelHeave * BOT_BODY_VEL_SCALE_FACTOR;

	ASOAImplementor::GetInstance()->Publish(CommonSOAObservers::PlatformKinematicObserverId, p_kinematic_data_args);

	
	//UE_LOG(LogTemp, Warning, TEXT("pos data: %s %s"), *FString::SanitizeFloat(p_data->Lat), *FString::SanitizeFloat(p_data->Lon));



}
