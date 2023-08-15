// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/Zone/ZoneContainer.h"


void CZoneContainer::Init(INT32S count) {
	Zones.Add(new SZoneInfo());
}


bool CZoneContainer::SetArea(INT32S id, FLOAT64 start_bearing_deg, FLOAT64 end_bearing_deg)
{
	if (id >= Zones.Num()) {
		return false;
	}

	auto* p_zone = Zones[id];
	p_zone->ZoneStartBearingDeg = start_bearing_deg;
	p_zone->ZoneEndBearingDeg = end_bearing_deg;
	return true;
}

bool CZoneContainer::SetZoneState(INT32S id, EZoneState state)
{
	if (id >= Zones.Num()) {
		return false;
	}

	auto* p_zone = Zones[id];
	p_zone->ZoneState = state;
	return true;
}
bool CZoneContainer::SetZoneAlarm(INT32S id, EZoneAlarm alarm)
{
	if (id >= Zones.Num()) {
		return false;
	}

	auto* p_zone = Zones[id];
	p_zone->ZoneAlarm = alarm;
	return true;
}
/// <summary>
/// 
/// </summary>
/// <param name="id"></param>
/// <param name="sensisitivty">0:min 1:max</param>
/// <returns></returns>
bool CZoneContainer::SetZoneSensitivity(INT32S id, FLOAT32 sensisitivty)
{
	if (id >= Zones.Num()) {
		return false;
	}

	auto* p_zone = Zones[id];
	p_zone->ZoneSensitivity = sensisitivty;
	return true;
}
