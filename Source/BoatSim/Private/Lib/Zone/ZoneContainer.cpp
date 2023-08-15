// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/Zone/ZoneContainer.h"


void CZoneContainer::Init(INT32S count) {
	for (int i = 0; i < count; i++) {
		Zones.Add(new SZoneInfo());
	}
	
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
	p_zone->IsZoneAlarmEnabled = true;
	p_zone->ZoneAlarm = alarm;
	return true;
}
bool CZoneContainer::CancelZoneAlarm(INT32S id)
{
	if (id >= Zones.Num()) {
		return false;
	}

	auto* p_zone = Zones[id];
	p_zone->IsZoneAlarmEnabled = false;

	return true;
}
/// <summary>
/// 
/// </summary>
/// <param name="id"></param>
/// <param name="sensisitivty">0:min 1:max</param>
/// <returns></returns>
bool CZoneContainer::SetZoneSensitivity(FLOAT32 sensisitivty)
{
	for (auto* p_zone : Zones) {
		p_zone->ZoneSensitivity = (sensisitivty);
	}

	Sensitivity = sensisitivty;

	return true;
}

bool CZoneContainer::CheckAnyActiveZone(FLOAT32 start_bearing_deg, FLOAT32 end_bearing_deg)
{
	for (auto* p_zone : Zones) {
		if (!p_zone->ZoneState == EZoneState::Active) {
			continue;
		}
		if (p_zone->ZoneStartBearingDeg <= start_bearing_deg && p_zone->ZoneEndBearingDeg >= end_bearing_deg) {
			return true;
		}
	}

	return false;
}
