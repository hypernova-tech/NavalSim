// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */

#pragma once
#define HORIZOTAL_SCAN_SIZE 1920
#define VERTICAL_SCAN_SIZE 1080

#include <Lib/Types/Primitives.h>
enum EScanObjectType
{
	ScanObjectTypeUnknown = -1,
	ScanObjectTypeTerrain = 0,
	ScanObjectTypeMovable = 1

};

struct SScanInfo
{
	
	
};

struct SScanLineEntry {
	FVector Pos;
	EScanObjectType  ObjectType;
	FLOAT32 NormalStrength;
};

struct SSectorInfo
{
	FLOAT64 StartAzimuthDeg;
	FLOAT64 EndAzimuthDeg;
	FLOAT64 AzimuthStepDeg;
	INT32S ScanLineCount;
	TArray<FVector> SectorData;
	TArray<EScanObjectType> ObjectType;
	TArray<FLOAT32> NormalStrength;
	TArray<TArray<SScanLineEntry>*> ScanLines;




public:

	void Init(INT32S scan_line_count, FLOAT64 start_azimuth_deg, FLOAT64 end_azimuth_deg)
	{
		ScanLineCount = scan_line_count;
		StartAzimuthDeg = start_azimuth_deg;
		EndAzimuthDeg = end_azimuth_deg;
		AzimuthStepDeg = (EndAzimuthDeg - StartAzimuthDeg)/(scan_line_count);
		for (INT32S i = 0; i < scan_line_count; i++) {
			ScanLines.Add(new TArray<SScanLineEntry>());
		}
	}
	void Reset() {
		SectorData.Reset();
		ObjectType.Reset();
		NormalStrength.Reset();
		for (INT32S i = 0; i < ScanLines.Num(); i++) {
			ScanLines[i]->Reset();
		}
	
	}

	void Add(FVector& vec, EScanObjectType object_type = EScanObjectType::ScanObjectTypeUnknown, FLOAT32 NormalStrenght = 1)
	{
		if (vec.ContainsNaN()) {
			bool is_nan;
			is_nan = true;
		}
		SectorData.Add(vec);
		ObjectType.Add(object_type);
		NormalStrength.Add(NormalStrenght);
	}
	void Add(FVector& vec, INT32S azimuth_scan_ind, EScanObjectType object_type = EScanObjectType::ScanObjectTypeUnknown, FLOAT32 NormalStrenght = 1)
	{
		if (vec.ContainsNaN()) {
			bool is_nan;
			is_nan = true;
		}
		SectorData.Add(vec);
		ObjectType.Add(object_type);

		if (azimuth_scan_ind < ScanLines.Num()) {
			SScanLineEntry entry;
			entry.Pos = vec;
			entry.ObjectType = object_type;
			entry.NormalStrength = NormalStrenght;
			ScanLines[azimuth_scan_ind]->Add(entry);
		}
	}
	void DepthSet(INT32S sample_ind, INT32S depth, FLOAT32 cell_size_meter, INT32S total_byte_count, INT8U* p_out)
	{
		for (INT32S j = 1; j < depth; j++) {

			INT32S byte_ind ;
			INT32S order ;
			auto depth_ind = (sample_ind + j);
			byte_ind = depth_ind / 2;
			order = depth_ind & 0x1;

			if (byte_ind >= total_byte_count) {
				break;
			}

			if (order) {
				p_out[byte_ind] |= 0xF;
			}
			else {
				p_out[byte_ind] |= 0xF << 4;
			}

		}
	}

	void DepthSetRanged(FLOAT64 start, FLOAT64 end, FLOAT32 cell_size_meter, INT32S total_byte_count, INT8U* p_out)
	{
		INT32S start_ind = start / cell_size_meter;
		INT32S end_ind   = end / cell_size_meter+0.5;

		for (INT32S j = start_ind; j < end_ind; j++) {

			INT32S byte_ind;
			INT32S order;
			INT32S sample_ind = j;
			byte_ind = sample_ind / 2;
			order = sample_ind & 0x1;

			if (byte_ind >= total_byte_count) {
				break;
			}

			if (order) {
				p_out[byte_ind] |= 0xF;
			}
			else {
				p_out[byte_ind] |= 0xF << 4;
			}

		}
	}

	bool MapSpoke4Bits(FVector own_ship_pos, FLOAT32 azimuth_deg, FLOAT32 cell_size_meter, INT32S total_byte_count, INT8U *p_out)
	{
		if (azimuth_deg >= EndAzimuthDeg) {
			return false;
		}

		INT32S ind = (azimuth_deg - StartAzimuthDeg) / AzimuthStepDeg;

		if (ind >= ScanLines.Num()) {
			return false;
		}

		TArray<SScanLineEntry>* p_data = ScanLines[ind];

		FLOAT64 max_h = -1;
		FLOAT64 max_dist_xy = -1;
		FLOAT64 min_dist_xy_terr = 1e38;
		FLOAT64 max_dist_xy_terr = -1;
		bool has_terrain = false;

		for (INT32S i = 0; i < p_data->Num(); i++) {
			SScanLineEntry* p_entry = &(*p_data)[i];
			FVector pos = p_entry->Pos;

			FLOAT64 h = TOW(FMath::Abs((pos - own_ship_pos).Z));
			FLOAT64 dist_xy = TOW(FVector::DistXY(pos,own_ship_pos));
			if (max_h < h) {
				max_h = h;
			}

			if (max_dist_xy < dist_xy) {
				max_dist_xy = dist_xy;
			}

			if (p_entry->ObjectType == EScanObjectType::ScanObjectTypeTerrain) {
				if (max_dist_xy_terr < dist_xy) {
					max_dist_xy_terr = dist_xy;
				}
				if (min_dist_xy_terr > dist_xy) {
					min_dist_xy_terr = dist_xy;
				}
				has_terrain = true;
			}

		}
		
		max_h *= 1.5;
		if (max_h > 0 && max_h < 2) {
			max_h = 2;
		}

		

		if (has_terrain) {
			if (max_dist_xy_terr - min_dist_xy_terr < 20) {
				max_dist_xy_terr = min_dist_xy_terr + 20;
			}

			DepthSetRanged(min_dist_xy_terr, max_dist_xy_terr, cell_size_meter, total_byte_count, p_out);
		}

		for (INT32S i = 0; i < p_data->Num(); i++) {
			FVector pos = (*p_data)[i].Pos;

			FLOAT32 dist = TOW( FVector::DistXY(pos, own_ship_pos));
			INT32S sample_ind = dist / cell_size_meter;

			INT32S byte_ind = sample_ind / 2;
			INT32S order = sample_ind & 0x1;
			INT32S depth = (INT32S)(max_h / cell_size_meter+0.5);

			if (order) {
				p_out[byte_ind] |= 0xF;
			}
			else {
				p_out[byte_ind] |= 0xF<<4;
			}

			DepthSet(sample_ind, depth, cell_size_meter, total_byte_count, p_out);
		}

		return true;
	}

};



struct SSectorContainer
{
	SSectorInfo* pSectors;
	int SectorCount;
	FLOAT64 AzimuthResolutionDeg;



public:
	void Init(int count, FLOAT64 azimuth_resolution_deg)
	{
		FLOAT64 each_sector_width_deg = 360.0 / count;

		pSectors = new SSectorInfo[count];
		SectorCount = count;
		AzimuthResolutionDeg = azimuth_resolution_deg;
		INT32S total_scan_line_per_sector = (INT32S) (each_sector_width_deg/ azimuth_resolution_deg + 0.5);
		
		for (int i = 0; i < count; i++) {
			FLOAT64 start_azimuth_deg = (each_sector_width_deg) * i;
			FLOAT64 end_azimuth_deg = start_azimuth_deg + each_sector_width_deg;
			pSectors[i].Init(total_scan_line_per_sector, start_azimuth_deg, end_azimuth_deg);
		}
	}

	SSectorInfo* GetSector(int no)
	{
		if (no < SectorCount) {
			return &pSectors[no];
		}
		else {
			return nullptr;
		}
		
	}

	void ResetSector(int no) {

		if (no < SectorCount) {
			pSectors[no].Reset();
		}
	}


	void AddToSector(int no, FVector& vec) {

		if (no < SectorCount) {
			pSectors[no].Add(vec);
		}
	}
};

struct SScanResult
{
	INT32S HorizontalCount;
	INT32S VeriticalCount;
	FVector ScanCenter;
	FVector ScanRPYWorld;
	FLOAT32 RangeMeter[HORIZOTAL_SCAN_SIZE][VERTICAL_SCAN_SIZE]; 
	FLOAT32 NormalStrength[HORIZOTAL_SCAN_SIZE][VERTICAL_SCAN_SIZE]; 
	FVector Point3D[HORIZOTAL_SCAN_SIZE][VERTICAL_SCAN_SIZE];

	INT32S SectorCount;
	FLOAT32 ScanAzimuthStepDeg;
	FLOAT32 ScanElevationStepDeg;
	FVector2D AzimuthRange;
	FVector2D ElevationRange;
		
	FVector Track3DWorld[HORIZOTAL_SCAN_SIZE * VERTICAL_SCAN_SIZE];
	FLOAT32 TrackRangeMeter[HORIZOTAL_SCAN_SIZE * VERTICAL_SCAN_SIZE]; 

	INT32U Track3DCount;

	SSectorContainer SectorContainer;
	INT32S CurrentSector = 0;
	
	FLOAT32 ScanRangeMeter;

	FLOAT64 TotalTimeSec;
	FLOAT64 TotalRaycastTimeSec;
	bool IsTrueNorth = false;

public:

	SScanResult() {

	}

	void Init(int sector_cnt, FLOAT64 azimuth_resolution = 0.087890625)
	{
		SectorCount = sector_cnt;
		SectorContainer.Init(SectorCount, azimuth_resolution);
	}


	SSectorContainer* GetSectorContainer()
	{
		return &SectorContainer;
	}

	 void CopyFrom(SScanResult* p_src) {
		memcpy(&Point3D, &p_src->Point3D, sizeof(p_src->Point3D));
	}

	 void CopyTrackPoint3DOnly(SScanResult* p_src)
	 {
		 memcpy(Track3DWorld, p_src->Track3DWorld, sizeof(FVector) * Track3DCount);
		 Track3DCount = p_src->Track3DCount;
	 }

	 FLOAT32 Interpolate(FLOAT32 azimuth_deg, FLOAT32 elevation_deg,FLOAT32 &intensity)
	{
		INT32U azimuth_ind = (azimuth_deg - AzimuthRange.X) / ScanAzimuthStepDeg;
		INT32U elevation_ind = (elevation_deg - ElevationRange.X) / ScanElevationStepDeg;

	
		intensity = NormalStrength[azimuth_ind][elevation_ind];
		return RangeMeter[azimuth_ind][elevation_ind];

	}
	 FLOAT32 GetElevIntensityAndReset(INT32S azimuth_ind, INT32S elevation_ind, FLOAT32& intensity)
	 {
		

		 intensity = NormalStrength[azimuth_ind][elevation_ind];
		 FLOAT32 range =  RangeMeter[azimuth_ind][elevation_ind];

		 NormalStrength[azimuth_ind][elevation_ind] = 0;
		 RangeMeter[azimuth_ind][elevation_ind] = 0;

		 return range;

	 }
     void AddTrackPoint3DList(FVector pos, FLOAT32 range_meter)
	{
		 Track3DWorld[Track3DCount] = pos;
		 TrackRangeMeter[Track3DCount] = range_meter;
		 Track3DCount++;
	}
	 void ResetTrackPoint3DList()
	{
		Track3DCount = 0;
	}


	 void ResetBuffers()
	 {
		 memset(RangeMeter, 0, sizeof(RangeMeter));
		 memset(NormalStrength, 0, sizeof(NormalStrength));
		 memset(Point3D, 0, sizeof(Point3D));
		 memset(Track3DWorld, 0, sizeof(Track3DWorld));
		 memset(TrackRangeMeter, 0, sizeof(TrackRangeMeter));
	 }

};


class CScanResultContainer
{
	TArray<SScanResult*> ScanResults;
	INT32U CircularAccessInd = 0;
	INT32U SectorCount;


public:
	void Init(INT32S buff_cnt, INT32U sector_cnt, FLOAT64 azimuth_res = 0.087890625)
	{

		for (INT32S i = 0; i < buff_cnt; i++) {
			SScanResult* p_res = new SScanResult();
			p_res->Init(sector_cnt, azimuth_res);
			ScanResults.Add(p_res);
			SectorCount = sector_cnt;
		}

		
	}

	INT32U GetSectorCount()
	{
		return SectorCount;
	}

	SScanResult* GetCircular()
	{
		auto p_res = ScanResults[CircularAccessInd];

		CircularAccessInd++;
		CircularAccessInd %= ScanResults.Num();

		return p_res;
	}

};
class CScanResult
{
public:
	CScanResult();
	~CScanResult();
};
