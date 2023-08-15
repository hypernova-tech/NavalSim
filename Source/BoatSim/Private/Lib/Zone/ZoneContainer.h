// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Lib/Types/Primitives.h"

/**
 * 
 */
enum EZoneAlarm
{
	ZoneAlarmEntry = 0x0,
	ZoneAlarmExit = 0x1,
	ZoneAlarmBoth = 0x2,
};

enum EZoneState
{
	Inactive,
	Active,
	Suppressed,
	Canceled,
};

struct SZoneInfo {
public:
	SZoneInfo()
	{
		memset(this, 0, sizeof(SZoneInfo));
	}

	INT8U ZoneId;
	EZoneState ZoneState;
	FLOAT64 ZoneStartBearingDeg;
	FLOAT64 ZoneEndBearingDeg;
	FLOAT32 ZoneSensitivity;
	EZoneAlarm ZoneAlarm;
};


class CZoneContainer
{

	TArray< SZoneInfo*> Zones;

public:
	void Init(INT32S count);


	bool SetArea(INT32S id, FLOAT64 start_bearing_deg, FLOAT64 end_bearing_deg);
	bool SetZoneState(INT32S id, EZoneState state);
	bool SetZoneAlarm(INT32S id, EZoneAlarm alarm);
	/// <summary>
	/// 
	/// </summary>
	/// <param name="id"></param>
	/// <param name="sensisitivty">0:min 1:max</param>
	/// <returns></returns>
	bool SetZoneSensitivity(INT32S id, FLOAT32 sensisitivty);
};
