// Fill out your copyright notice in the Description page of Project Settings.


#include "Products/IDAS/Sensors/Lidar/LidarRobosenseM1.h"
#include "RobosenseM1/RobosenseM1CommIF.h"

void ALidarRobosenseM1::OnScanCompleted(SScanResult* p_scan_result)
{
	Super::OnScanCompleted(p_scan_result);



}
 void ALidarRobosenseM1::InitSensor()
{
	 Super::InitSensor();
	 URobosenseM1CommIF* p_m1_comm = Cast< URobosenseM1CommIF>(GetCommCommIF());

	 p_m1_comm->GetConfigurationPacket()->SetVerticalFovStartaAngle(0)->SetVerticalFovEndAngle(FovVerticalDeg)->
		 SetHorizontalFovStartaAngle(0)->SetHorizontalFovEndAngle(FovHorizontalDeg);
	

	 p_m1_comm->SendConfig();

}

 void ALidarRobosenseM1::Save(ISaveLoader* p_save_load)
 {
	 Super::Save(p_save_load);
 }
