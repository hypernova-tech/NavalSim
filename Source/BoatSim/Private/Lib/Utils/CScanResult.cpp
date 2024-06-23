// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/Utils/CScanResult.h"
#include <Lib/Utils/CUtil.h>
#include <Lib/Math/CMath.h>
#include <Lib/SystemManager/SystemManagerBase.h>
CScanResult::CScanResult()
{
}

CScanResult::~CScanResult()
{
}

void SSectorInfo::SetSectorWorldAngles(FLOAT64 start_azim_deg, FLOAT64 end_azim_deg)
{
	StartAzimuthDeg = start_azim_deg;
	EndAzimuthDeg = end_azim_deg;
}

void SSectorInfo::Init(INT32S scan_line_count, FLOAT64 start_azimuth_deg, FLOAT64 end_azimuth_deg)
{
	ScanLineCount = scan_line_count;
	StartAzimuthDeg = start_azimuth_deg;
	EndAzimuthDeg = end_azimuth_deg;
	AzimuthStepDeg = (EndAzimuthDeg - StartAzimuthDeg) / (scan_line_count);
	for (INT32S i = 0; i < scan_line_count; i++) {
		ScanLines.Add(new TArray<SScanLineEntry>());
	}
}
void SSectorInfo::Reset() {
	SectorData.Reset();
	ObjectType.Reset();
	NormalStrength.Reset();
	for (INT32S i = 0; i < ScanLines.Num(); i++) {
		ScanLines[i]->Reset();
	}

}

void SSectorInfo::Add(FVector& vec, EScanObjectType object_type, FLOAT32 NormalStrenght)
{
	if (vec.ContainsNaN()) {
		bool is_nan;
		is_nan = true;
	}
	SectorData.Add(vec);
	ObjectType.Add(object_type);
	NormalStrength.Add(NormalStrenght);
}
void SSectorInfo::Add(FVector& vec, INT32S azimuth_scan_ind, EScanObjectType object_type, FLOAT32 NormalStrenght)
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

void SSectorInfo::DepthSet(INT32S sample_ind, INT32S depth, FLOAT32 cell_size_meter, INT32S total_byte_count, INT8U* p_out)
{
	for (INT32S j = 1; j < depth; j++) {

		INT32S byte_ind;
		INT32S order;
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

void SSectorInfo::DepthSetPointCloud(FVector start_point, FVector dir, INT32S sample_ind, INT32S depth, FLOAT32 cell_size_meter, INT32S total_byte_count, TArray<FVector>& out)
{
	for (INT32S j = 1; j < depth; j++) {
		FVector pos = TOW(start_point) + dir * j * cell_size_meter;
		out.Add(pos);
	}
}

void SSectorInfo::DepthSetRangedPointCloud(FVector start_point, FVector end_point, FLOAT64 start, FLOAT64 end, FLOAT32 cell_size_meter, INT32S total_byte_count, TArray<FVector>& out)
{
	INT32S start_ind = start / cell_size_meter;
	INT32S end_ind = end / cell_size_meter + 0.5;
	FVector pos;
	FVector dir = (end_point - start_point);
	dir.Normalize();

	for (INT32S j = start_ind; j < end_ind; j++) {
		pos = TOW(start_point) + dir * (j - start_ind) * cell_size_meter;
		out.Add(pos);
	}
}


void SSectorInfo::DepthSetRanged(FLOAT64 start, FLOAT64 end, FLOAT32 cell_size_meter, INT32S total_byte_count, INT32U set_value, INT8U* p_out)
{
	INT32S start_ind = start / cell_size_meter;
	INT32S end_ind = end / cell_size_meter + 0.5;

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
			p_out[byte_ind] |= set_value;
		}
		else {
			p_out[byte_ind] |= set_value << 4;
		}

	}
}

void SSectorInfo::DepthSetRangedTerrainAwarePointCloud(FVector start_pos, FVector dir, FLOAT64 start, FLOAT64 end, FLOAT32 cell_size_meter, INT32S total_byte_count, TArray<FVector>& out)
{
	INT32S start_ind = start / cell_size_meter;
	INT32S end_ind = end / cell_size_meter + 0.5;
	ATerrainManager* p_terrain = ASystemManagerBase::GetInstance()->GetTerrainManager();
	bool ret = false;
	for (INT32S j = start_ind; j < end_ind; j++) {

		INT32S byte_ind;
		INT32S order;
		INT32S sample_ind = j;
		byte_ind = sample_ind / 2;
		order = sample_ind & 0x1;



		if (byte_ind >= total_byte_count) {
			break;
		}

		FVector pos = start_pos + dir * (j)*TOUE(cell_size_meter);
		pos = TOW(pos);
		auto elev = p_terrain->GetElevation((pos.X), (pos.Y), ret);
	
		if (elev > 0) {
			pos.Z = elev;
			out.Add(pos);
		}
	


	}
}

void SSectorInfo::DepthSetRangedTerrainAware(FVector start_pos, FVector dir,FLOAT64 start, FLOAT64 end, FLOAT32 cell_size_meter, INT32S total_byte_count, INT32U set_value, INT8U* p_out)
{
	INT32S start_ind = start / cell_size_meter;
	INT32S end_ind = end / cell_size_meter + 0.5;
	ATerrainManager* p_terrain = ASystemManagerBase::GetInstance()->GetTerrainManager();
	bool ret = false;
	for (INT32S j = start_ind; j < end_ind; j++) {

		INT32S byte_ind;
		INT32S order;
		INT32S sample_ind = j;
		byte_ind = sample_ind / 2;
		order = sample_ind & 0x1;

		

		if (byte_ind >= total_byte_count) {
			break;
		}

		FVector pos = start_pos + dir * (j) * TOUE(cell_size_meter);

		auto elev = p_terrain->GetElevation(TOW(pos.X), TOW(pos.Y), ret);


		if (order) {
			if (elev > 0) {
				p_out[byte_ind] |= set_value;
			}
			else {
				p_out[byte_ind] &= ~set_value;
			}

			
		}
		else {
			p_out[byte_ind] |= set_value << 4;

			if (elev > 0) {
				p_out[byte_ind] |= set_value << 4;
			}
			else {
				p_out[byte_ind] &= ~(0xF << 4);
			}
		}

	}
}


bool SSectorInfo::MapSpokePointCloud(FVector own_ship_pos, FLOAT32 azimuth_deg, FLOAT32 cell_size_meter, INT32S total_byte_count, TArray<FVector>& out)
{
	if (azimuth_deg > EndAzimuthDeg) {
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
	FVector min_dist_pt;
	FVector max_dist_pt;
	FQuat yaw(FVector::UpVector, azimuth_deg * DEGTORAD);
	FVector dir = yaw.RotateVector(FVector::ForwardVector);

	for (INT32S i = 0; i < p_data->Num(); i++) {
		SScanLineEntry* p_entry = &(*p_data)[i];
		FVector pos = p_entry->Pos;

		FLOAT64 h = TOW(FMath::Abs((pos - own_ship_pos).Z));
		FLOAT64 dist_xy = TOW(FVector::DistXY(pos, own_ship_pos));
		if (max_h < h) {
			max_h = h;
		}

		if (max_dist_xy < dist_xy) {
			max_dist_xy = dist_xy;
		}

		if (p_entry->ObjectType == EScanObjectType::ScanObjectTypeTerrain) {
			if (max_dist_xy_terr < dist_xy) {
				max_dist_xy_terr = dist_xy;
				max_dist_pt = pos;
			}
			if (min_dist_xy_terr > dist_xy) {
				min_dist_xy_terr = dist_xy;
				min_dist_pt = pos;
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

		DepthSetRangedPointCloud(min_dist_pt, max_dist_pt, min_dist_xy_terr, max_dist_xy_terr, cell_size_meter, total_byte_count, out);

		auto total_dist = total_byte_count * 2 * cell_size_meter;

		auto rem_dist = max_dist_xy_terr - total_dist;
		FLOAT64 curr_max_dist = max_dist_xy_terr;

		while (true) {
			DepthSetRangedTerrainAwarePointCloud(own_ship_pos, dir, curr_max_dist, total_dist, cell_size_meter, total_byte_count, out);
			break;

		}
	}

	for (INT32S i = 0; i < p_data->Num(); i++) {
		FVector pos = (*p_data)[i].Pos;
		out.Add(TOW(pos));
		
		dir = (pos - own_ship_pos);
		dir.Normalize();

		FLOAT32 dist = TOW(FVector::DistXY(pos, own_ship_pos));
		INT32S sample_ind = dist / cell_size_meter;

		INT32S byte_ind = sample_ind / 2;
		INT32S depth = (INT32S)(max_h / cell_size_meter + 0.5);



		DepthSetPointCloud(pos, dir, sample_ind, depth, cell_size_meter, total_byte_count, out);
	}



	return true;
}


bool SSectorInfo::MapSpoke4Bits(FVector own_ship_pos, FLOAT32 azimuth_deg, FLOAT32 cell_size_meter, INT32S total_byte_count, INT8U* p_out)
{
	if (azimuth_deg >= EndAzimuthDeg) {
		return false;
	}

	INT32S ind = (azimuth_deg - StartAzimuthDeg) / AzimuthStepDeg;

	if (ind >= ScanLines.Num()) {
		return false;
	}
	FQuat yaw(FVector::UpVector, azimuth_deg * DEGTORAD);
	FVector dir = yaw.RotateVector(FVector::ForwardVector);

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
		FLOAT64 dist_xy = TOW(FVector::DistXY(pos, own_ship_pos));
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

		DepthSetRanged(min_dist_xy_terr, max_dist_xy_terr, cell_size_meter, total_byte_count, EDepthSetClass::ObjectOrTerrain, p_out);

		auto total_dist = total_byte_count * 2 * cell_size_meter;

		auto rem_dist = max_dist_xy_terr - total_dist;
		FLOAT64 curr_max_dist = max_dist_xy_terr;

		while (true) {
			DepthSetRangedTerrainAware(own_ship_pos, dir, curr_max_dist, total_dist, cell_size_meter, total_byte_count, EDepthSetClass::TerrainBorder, p_out);
			break;
			
		}
	}

	for (INT32S i = 0; i < p_data->Num(); i++) {
		FVector pos = (*p_data)[i].Pos;

		FLOAT32 dist = TOW(FVector::DistXY(pos, own_ship_pos));
		INT32S sample_ind = dist / cell_size_meter;

		INT32S byte_ind = sample_ind / 2;
		INT32S order = sample_ind & 0x1;
		INT32S depth = (INT32S)(max_h / cell_size_meter + 0.5);

		if (order) {
			p_out[byte_ind] |= 0xF;
		}
		else {
			p_out[byte_ind] |= 0xF << 4;
		}

		DepthSet(sample_ind, depth, cell_size_meter, total_byte_count, p_out);
	}

	return true;
}
