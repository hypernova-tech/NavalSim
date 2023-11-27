// Fill out your copyright notice in the Description page of Project Settings.


#include "Products/IDAS/EthernetProcessor/IDASEthernetDataProcessor.h"
#include "Lib/SOA/CSOAObserverArgs.h"
#include <Lib/SOACommonObserverArgs/PlatformKinematicData.h>
#include <Lib/SOA/SOAImplementor.h>
#include <Lib/SOA/CommonSOAObservers.h>
#include <Lib/SOA/CommonSOAObservers.h>
#include <Lib/Utils/CUtil.h>



#define LAT_SCALE_FACTOR					1e-7
#define LON_SCALE_FACTOR					(1e-7)
#define NED_VELOCITY_SCALE_FACTOR			(2e-15*1e-2)
#define BOT_POS_SCALE_FACTOR				(2e-31*1e-2)
#define BOT_EULER_ANGLE_SCALE_FACTOR		(1e-4*RADTODEG)
#define BOT_BODY_VEL_SCALE_FACTOR			(2e-15*1e-2)
list< SIdasAsToIdasGERaw*>* pDataHist;
void UIDASEthernetDataProcessor::BeginPlay()
{
	Super::BeginPlay();
	
}

void UIDASEthernetDataProcessor::OnReceivedData(const TArray<uint8>& data, int32 bytes_read)
{
	if (bytes_read == 0) {
		return;
	}
	Super::OnReceivedData(data, bytes_read);



	const SIdasAsToIdasGERaw* idas_raw = reinterpret_cast<const SIdasAsToIdasGERaw*>(data.GetData());
	CritcalSection.Lock();
	SIdasAsToIdasGERaw* p_data = new SIdasAsToIdasGERaw();
	*p_data = *idas_raw;
	KinematicDataBuff.push_back(p_data);
	CritcalSection.Unlock();


}

void UIDASEthernetDataProcessor::ProcessReceivedData()
{
	while (true) {
		CritcalSection.Lock();
		if (KinematicDataBuff.size()> 0) {
			SIdasAsToIdasGERaw *raw = KinematicDataBuff[0];
			
			KinematicDataBuff.erase(KinematicDataBuff.begin());
			CritcalSection.Unlock();
			SIdasAsToIdasGERaw* idas_raw = raw;

			//UE_LOG(LogTemp, Warning, TEXT("Receved byte count: %s %s %s"), *FString::FromInt(TotalReceivedCount), *FString::FromInt(MyStructArray->MessageId), *FString::FromInt(count));
			UPlatformKinematicData* p_kinematic_data_args = NewObject< UPlatformKinematicData>();// &KinematicDataBuff[KinematicDataBuffInd];// NewObject< UPlatformKinematicData>();

			p_kinematic_data_args->BotPosLatDeg = idas_raw->BotPosLat * LAT_SCALE_FACTOR;
			p_kinematic_data_args->BotPosLonDeg = idas_raw->BotPosLon * LAT_SCALE_FACTOR;

			p_kinematic_data_args->VelNorthNedMeterSec = idas_raw->VelNorthNed * NED_VELOCITY_SCALE_FACTOR;
			p_kinematic_data_args->VelEastNedMeterPerSec = idas_raw->VelEastNed * NED_VELOCITY_SCALE_FACTOR;


			p_kinematic_data_args->BotPosNorthMeter = idas_raw->BotPosNorth * BOT_POS_SCALE_FACTOR;
			p_kinematic_data_args->BotPosEastMeter = idas_raw->BotPosEast * BOT_POS_SCALE_FACTOR;
			p_kinematic_data_args->BotPosDownMeter = idas_raw->BotPosDown * BOT_POS_SCALE_FACTOR;

			p_kinematic_data_args->EulerPhiDeg = idas_raw->EulerPhiDeg * BOT_EULER_ANGLE_SCALE_FACTOR; //roll
			p_kinematic_data_args->EulerThetaDeg = idas_raw->EulerThetaDeg * BOT_EULER_ANGLE_SCALE_FACTOR; //pitch
			p_kinematic_data_args->EulerPsiDeg = idas_raw->EulerPsiDeg * BOT_EULER_ANGLE_SCALE_FACTOR; //yaw

			p_kinematic_data_args->BodyVelSurgeMeterPerSec = idas_raw->BodyVelSurge * BOT_BODY_VEL_SCALE_FACTOR;
			p_kinematic_data_args->BodyVelSwayMeterPerSec = idas_raw->BodyVelSway * BOT_BODY_VEL_SCALE_FACTOR;
			p_kinematic_data_args->BodyVelHeaveMeterPerSec = idas_raw->BodyVelHeave * BOT_BODY_VEL_SCALE_FACTOR;

			delete raw;

			ASOAImplementor::GetInstance()->Publish(CommonSOAObservers::PlatformKinematicObserverId, p_kinematic_data_args);
			
			

		}
		else {
			CritcalSection.Unlock();
			break;
		}

	}

}

void UIDASEthernetDataProcessor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	ProcessReceivedData();
}
