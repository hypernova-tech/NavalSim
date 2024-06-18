// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/Sensor/GenericSensor/GenericFLS3D.h"
#include <Lib/SystemManager/SystemManagerBase.h>
#include <Lib/Utils/CUtil.h>
#include "Windows/MinWindows.h" // Include this header for Windows-specific functions
#include <Lib/Math/CMath.h>

void AGenericFLS3D::InitSensor()
{
	Super::InitSensor();
}

void AGenericFLS3D::OnDataReady()
{
	Super::OnDataReady();
}

void AGenericFLS3D::Run(float delta_time_sec)
{
	Super::Run(delta_time_sec);
}

void AGenericFLS3D::OnPreTrace(STraceArgs& args)
{
	//args.elevation_start_deg = -10;
	//args.elevation_end_deg = FovVerticalDeg - 10;
	Super::OnPreTrace(args);
}

void AGenericFLS3D::Save(ISaveLoader* p_save_loader)
{
	Super::Save(p_save_loader);
}

void AGenericFLS3D::SaveJSON(CJsonDataContainer& data)
{
	Super::SaveJSON(data);
}
