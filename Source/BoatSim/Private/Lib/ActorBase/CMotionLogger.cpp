// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/ActorBase/CMotionLogger.h"



CMotionLogger::CMotionLogger()
{
}

CMotionLogger::~CMotionLogger()
{
}

CMotionLogEntry* CMotionLogger::AddEntry(double time, const FVector& pos)
{
	CMotionLogEntry* p_entry = new CMotionLogEntry();
	p_entry->TimeTagSec = time;
	p_entry->Position = pos;
	Entries.push_back(p_entry);
	return p_entry;
}

void CMotionLogger::Log(double time, const FVector& pos, FLOAT32 min_distance_meter)
{
	if (pLastEntry != nullptr) {
		if (FVector::Distance(pos, pLastEntry->Position) > TOUE(min_distance_meter)) {
			pLastEntry = AddEntry(time, pos);
		}
	}
	else {
		pLastEntry = AddEntry(time, pos);
	}
}

void CMotionLogger::Update(double curr_time_sec, double life_time_duration)
{
	if ((curr_time_sec - LastMotionLogUpdateTimeSec) < MotionLogUpdateIntervalSec) {
		return;
	}
	for (std::list<CMotionLogEntry*>::iterator it = Entries.begin(); it != Entries.end(); ++it) {
		CMotionLogEntry* entry = *it;
		if ((curr_time_sec - entry->TimeTagSec) > life_time_duration) {
			delete entry; // Assuming you need to deallocate the memory			
			it = Entries.erase(it);
		}
	}

	LastMotionLogUpdateTimeSec = MotionLogUpdateIntervalSec;

}

void CMotionLogger::GetPoints(TArray<FVector>& vecs)
{
	for (std::list<CMotionLogEntry*>::iterator it = Entries.begin(); it != Entries.end(); ++it) {
		vecs.Add((*it)->Position);
	}
}
