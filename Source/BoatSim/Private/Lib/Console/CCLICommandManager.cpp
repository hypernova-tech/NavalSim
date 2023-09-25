// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/Console/CCLICommandManager.h"
#include <Lib/Utils/CUtil.h>

CCLICommandManager::CCLICommandManager()
{
	PrepareCreateCommandInfo();
	PrepareProcessCommandInfo();
}


void CCLICommandManager::SetCommandOptions(TMap<FString, FString>* p_opt)
{
	pOptions = p_opt;
}

TMap<FString, TArray<SCommandOptionInfo>>* CCLICommandManager::GetCommandInfo()
{
	return &CommandInfo;
}

void CCLICommandManager::PrepareCreateCommandInfo()
{
	TArray<SCommandOptionInfo> options;
	// create command
	SCommandOptionInfo info;

	info.Option = "--name";
	info.Description = "sets the name of actor tobe created";
	options.Add(info);

	info.Option = "--bp";
	info.Description = "sets the predefined blueprint of the object";
	options.Add(info);

	info.Option = "--model";
	info.Description = "sets the 3d model of the object";
	options.Add(info);

	CommandInfo.Add(CreateCommand, options);
}

void CCLICommandManager::PrepareProcessCommandInfo()
{
	TArray<SCommandOptionInfo> options;
	// create command
	SCommandOptionInfo info;
	info.Option = "--kill";
	info.Description = "kills specified instance id";
	options.Add(info);

	

	CommandInfo.Add(ProcessCommand, options);
}

void CCLICommandManager::PrepareSetCommandInfo()
{
	TArray<SCommandOptionInfo> options;
	// create command
	SCommandOptionInfo info;
	info.Option = "--name";
	info.Description = "sets the name of object";
	options.Add(info);



	CommandInfo.Add(SetCommand, options);
}
bool CCLICommandManager::HasName()
{
	auto ret = pOptions->Find(Name);
	if (ret != nullptr) {

		return true;
	}

	return false;
}

FString CCLICommandManager::GetName()
{
	auto ret = pOptions->Find(Name);
	if (ret != nullptr) {
		return *ret;
	}

	return "";
}
int CCLICommandManager::GetProcessKillInstanceCount()
{
	auto arg = pOptions->Find(ProcessKill);
	if (arg != nullptr) {
		INT32S ret = CUtil::StringToInt(*arg);
		return ret;
	}

	return -1;
}
TArray<FString> CCLICommandManager::GetRelativeName()
{
	TArray<FString> names;
	auto ret = pOptions->Find(Name);
	if (ret != nullptr) {
		(*ret).ParseIntoArray(names, TEXT("/"), true);
		return names;

	}

	return names;
}
FString CCLICommandManager::GetBP()
{
	auto ret = pOptions->Find(Bp);
	if (ret != nullptr) {
		return *ret;
	}

	return "";
}

FString CCLICommandManager::GetModel()
{
	auto ret = pOptions->Find(Model);
	if (ret != nullptr) {
		return *ret;
	}

	return "";
}

bool CCLICommandManager::GetPosition(FVector& vec)
{
	bool ret = false;
	auto pstr = pOptions->Find(Position);
	if (pstr != nullptr) {

		ret = CUtil::ParseVector3D(*pstr, vec);
		return ret;
	}


	return false;
}

bool CCLICommandManager::GetRelPosition(FVector& vec)
{
	bool ret = false;
	auto pstr = pOptions->Find(RelPosition);
	if (pstr != nullptr) {

		ret = CUtil::ParseVector3D(*pstr, vec);
		return ret;
	}


	return false;
}

bool CCLICommandManager::GetRotation(FVector& vec)
{
	bool ret = false;
	auto pstr = pOptions->Find(Rotation);
	if (pstr != nullptr) {

		ret = CUtil::ParseVector3D(*pstr, vec);
		return ret;
	}


	return false;
}

bool CCLICommandManager::GetRelRotation(FVector& vec)
{
	bool ret = false;
	auto pstr = pOptions->Find(RelRotation);
	if (pstr != nullptr) {

		ret = CUtil::ParseVector3D(*pstr, vec);
		return ret;
	}


	return false;
}

bool CCLICommandManager::GetScale(FVector& vec)
{
	bool ret = false;
	auto pstr = pOptions->Find(Scale);
	if (pstr != nullptr) {

		ret = CUtil::ParseVector3D(*pstr, vec);
		return ret;
	}


	return false;
}

