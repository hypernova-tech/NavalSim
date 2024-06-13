#pragma once


class IRadarHostIF:public IHostIF
{
public:
	IRadarHostIF() {};
	
	virtual char* GetSerial() = 0;
	virtual double GetGain() = 0;
	virtual double GetRainClutterLevel() = 0;
	virtual double GetSeaClutterLevel() = 0;
	virtual double GetTerrainBorderThreshold() = 0;


};

