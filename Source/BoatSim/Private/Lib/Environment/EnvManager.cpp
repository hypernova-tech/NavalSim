// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/Environment/EnvManager.h"
#include <Lib/Console/CCLICommandManager.h>

AEnvManager::AEnvManager()
{

	
}

void AEnvManager::BeginPlay()
{
	Super::BeginPlay();
	Rename((TEXT("env")));
}

void AEnvManager::SetStartingDate(int year, int month, int day, int hour, int min, double sec)
{
}

void AEnvManager::SetEnvSoundFxEnabled(bool val)
{
	EnvSoundFxEnabled = val;
}

bool AEnvManager::GetEnvSoundFxEnabled()
{
	return EnvSoundFxEnabled;
}

FDateTime AEnvManager::GetStartingDateTime()
{
	return FDateTime();
}

FDateTime AEnvManager::GetCurrentDateTime()
{
	return FDateTime();
}

void AEnvManager::SetFogLevelPercent(double level)
{
	FogPercent = level;
}

double AEnvManager::GetFogLevelPercent()
{
	return FogPercent;
}

void AEnvManager::SeRainLevelPercent(double level)
{
	RainPercent = level;
}

double AEnvManager::GetRainLevelPercent()
{
	return RainPercent;
}

void AEnvManager::SetSnowLevelPercent(double level)
{
	SnowPercent = level;
}

double AEnvManager::GetSnowLevelPercent()
{
	return SnowPercent;
}

void AEnvManager::SetWindSpeedMetersPerSec(double level)
{
	WindSpeedMetersPerSec = level;
}

double AEnvManager::GetWindSpeedMetersPerSec()
{
	return WindSpeedMetersPerSec;
}

void AEnvManager::SetWindDirectionDeg(double val)
{
	WindDirectionDeg = val;
}

double AEnvManager::GetWindDirectionDeg()
{
	return WindDirectionDeg;
}

void AEnvManager::SetDustLevelPercent(double level)
{
	DustPercent = level;
}

double AEnvManager::GetDustLevelPercent()
{
	return DustPercent;
}

void AEnvManager::SetCloudLevelPercent(double level)
{
	CloudPercent = level;
}

double AEnvManager::GetCloudLevelPercent()
{
	return CloudPercent;
}

void AEnvManager::SetThunderLevelPercent(double level)
{
	ThunderLevelPercent = level;
}

double AEnvManager::GetThunderLevelPercent()
{
	return ThunderLevelPercent;
}

void AEnvManager::SetTimeOfDayHr(double val)
{
	TimeOfDayHour = val;
}

double AEnvManager::GetTimeOfDayHr()
{
	return TimeOfDayHour;
}

void AEnvManager::SaveJSON(CJsonDataContainer& data)
{
	Super::SaveJSON(data);
	data.Add(CCLICommandManager::TimeOfDayHour, TimeOfDayHour);
	data.Add(CCLICommandManager::RainPercent, RainPercent);
	data.Add(CCLICommandManager::CloudPercent, CloudPercent);
	data.Add(CCLICommandManager::SnowPercent, SnowPercent);
	data.Add(CCLICommandManager::DustPercent, DustPercent);
	data.Add(CCLICommandManager::FogPercent, FogPercent);
	data.Add(CCLICommandManager::ThunderPercent, ThunderLevelPercent);
	data.Add(CCLICommandManager::EnvSoundFxEnabled, EnvSoundFxEnabled);
	data.Add(CCLICommandManager::WindSpeed, WindSpeedMetersPerSec);
	data.Add(CCLICommandManager::WindDirection, WindDirectionDeg);
}

void AEnvManager::Save(ISaveLoader* p_save_loader)
{
	Super::Save(p_save_loader);

	FString line;

	line = p_save_loader->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
	p_save_loader->AppendOption(line, CCLICommandManager::TimeOfDayHour, TimeOfDayHour);
	p_save_loader->AddLine(line);

	line = p_save_loader->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
	p_save_loader->AppendOption(line, CCLICommandManager::RainPercent, RainPercent);
	p_save_loader->AddLine(line);

	line = p_save_loader->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
	p_save_loader->AppendOption(line, CCLICommandManager::CloudPercent, CloudPercent);
	p_save_loader->AddLine(line);

	line = p_save_loader->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
	p_save_loader->AppendOption(line, CCLICommandManager::SnowPercent, SnowPercent);
	p_save_loader->AddLine(line);

	line = p_save_loader->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
	p_save_loader->AppendOption(line, CCLICommandManager::DustPercent, DustPercent);
	p_save_loader->AddLine(line);

	line = p_save_loader->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
	p_save_loader->AppendOption(line, CCLICommandManager::FogPercent, FogPercent);
	p_save_loader->AddLine(line);

	line = p_save_loader->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
	p_save_loader->AppendOption(line, CCLICommandManager::ThunderPercent, ThunderLevelPercent);
	p_save_loader->AddLine(line);

	line = p_save_loader->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
	p_save_loader->AppendOption(line, CCLICommandManager::EnvSoundFxEnabled, EnvSoundFxEnabled);
	p_save_loader->AddLine(line);

	line = p_save_loader->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
	p_save_loader->AppendOption(line, CCLICommandManager::WindSpeed, WindSpeedMetersPerSec);
	p_save_loader->AddLine(line);

	line = p_save_loader->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
	p_save_loader->AppendOption(line, CCLICommandManager::WindDirection, WindDirectionDeg);
	p_save_loader->AddLine(line);


}


