// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/Console/CCLICommandManager.h"
#include <Lib/Utils/CUtil.h>
#include <Lib/SystemManager/SystemManagerBase.h>

CCLICommandManager::CCLICommandManager()
{
	PrepareProcessCommandInfo();
	PrepareSimCommandInfo();
	PrepareCreateCommandInfo();
	PrepareDestroyCommandInfo();
	PreparePrintCommandInfo();
	PrepareSetCommandInfo();
	PrepareGetCommandInfo();
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
void CCLICommandManager::PrepareDestroyCommandInfo()
{
	TArray<SCommandOptionInfo> options;
	// create command
	SCommandOptionInfo info;

	info.Option = "--name";
	info.Description = "sets the name of actor tobe destroyed";
	options.Add(info);


	CommandInfo.Add(DestroyCommand, options);
}


void CCLICommandManager::PrepareProcessCommandInfo()
{
	TArray<SCommandOptionInfo> options;
	// create command
	SCommandOptionInfo info;

	info.Option = "--run";
	info.Description = "instance count";
	options.Add(info);


	info.Option = "--kill";
	info.Description = "kills specified instance id";
	options.Add(info);

	

	CommandInfo.Add(ProcessCommand, options);
}
void CCLICommandManager::PreparePrintCommandInfo()
{
	TArray<SCommandOptionInfo> options;
	// create command
	SCommandOptionInfo info;
	info.Option = "--bp";
	info.Description = "prints all bp info";
	options.Add(info);

	info.Option = "--actors";
	info.Description = "prints all actors";
	options.Add(info);

	info.Option = "--name";
	info.Description = "prints actor or component with given name";
	options.Add(info);

	CommandInfo.Add(PrintCommand, options);
}

void CCLICommandManager::PrepareSimCommandInfo()
{
	TArray<SCommandOptionInfo> options;
	// create command
	SCommandOptionInfo info;
	info.Option = "--start";
	info.Description = "starts simulation";
	options.Add(info);

	info.Option = "--pause";
	info.Description = "pauses simulation";
	options.Add(info);

	info.Option = "--resume";
	info.Description = "resume simulation";
	options.Add(info);

	CommandInfo.Add(SimCommand, options);
}



void CCLICommandManager::PrepareSetCommandInfo()
{
	TArray<SCommandOptionInfo> options;
	// create command
	SCommandOptionInfo info;
	info.Option = "--name";
	info.Description = "sets the name of object";
	options.Add(info);

	info.Option = "--parent";
	info.Description = "sets the scale of object of name";
	options.Add(info);

	info.Option = "--active";
	info.Description = "sets the activity of object of name, 0: pasive, 1:active";
	options.Add(info);

	info.Option = "--position";
	info.Description = "sets the position of object of name";
	options.Add(info);

	info.Option = "--relposition";
	info.Description = "sets the rel position of object of name";
	options.Add(info);

	info.Option = "--rotation";
	info.Description = "sets the rotation (rpy deg) of object of name";
	options.Add(info);

	info.Option = "--relrotation";
	info.Description = "sets the relative rotation (rpy deg) of object of name";
	options.Add(info);

	info.Option = "--scale";
	info.Description = "sets the scale of object of name";
	options.Add(info);

	info.Option = "--controller";
	info.Description = "sets the possesed pawn";
	options.Add(info);

	CommandInfo.Add(SetCommand, options);
}

void CCLICommandManager::PrepareGetCommandInfo()
{
	TArray<SCommandOptionInfo> options;
	// create command
	SCommandOptionInfo info;


	info.Option = "--name";
	info.Description = "the name of object, specify name as  * to get all actors ";
	options.Add(info);

	info.Option = "--parent";
	info.Description = "gets the scale of object of name";
	options.Add(info);

	info.Option = "--active";
	info.Description = "gets the activity of object of name, 0: pasive, 1:active";
	options.Add(info);

	info.Option = "--position";
	info.Description = "gets the position of object of name";
	options.Add(info);

	info.Option = "--relposition";
	info.Description = "gets the rel position of object of name";
	options.Add(info);

	info.Option = "--rotation";
	info.Description = "gets the rotation (rpy deg) of object of name";
	options.Add(info);

	info.Option = "--relrotation";
	info.Description = "gets the relative rotation (rpy deg) of object of name";
	options.Add(info);

	info.Option = "--scale";
	info.Description = "gets the scale of object of name";
	options.Add(info);

	info.Option = "--scale";
	info.Description = "gets the scale of object of name";
	options.Add(info);

	info.Option = "--controller";
	info.Description = "gets the possesed pawn";
	options.Add(info);

	CommandInfo.Add(GetCommand, options);
}

bool CCLICommandManager::HasName()
{
	auto ret = pOptions->Find(Name);
	if (ret != nullptr) {

		return true;
	}

	return false;
}
bool CCLICommandManager::HasBP()
{
	auto ret = pOptions->Find(Bp);
	if (ret != nullptr) {

		return true;
	}

	return false;
}
bool CCLICommandManager::HasActors()
{
	auto ret = pOptions->Find(Actors);
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


bool CCLICommandManager::HasPosition()
{
	bool ret = false;
	auto pstr = pOptions->Find(Position);
	return  (pstr != nullptr);
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

bool CCLICommandManager::HasRelposition()
{
	bool ret = false;
	auto pstr = pOptions->Find(RelPosition);
	return  (pstr != nullptr);
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
bool CCLICommandManager::HasRotation()
{
	bool ret = false;
	auto pstr = pOptions->Find(Rotation);
	return  (pstr != nullptr);
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
bool CCLICommandManager::HasRelrotation()
{
	bool ret = false;
	auto pstr = pOptions->Find(RelRotation);
	return  (pstr != nullptr);
}
bool CCLICommandManager::HasParent()
{
	bool ret = false;
	auto pstr = pOptions->Find(Parent);
	return  (pstr != nullptr);
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

bool CCLICommandManager::HasScale()
{
	bool ret = false;
	auto pstr = pOptions->Find(Scale);
	return  (pstr != nullptr);
}


bool CCLICommandManager::HasController()
{
	bool ret = false;
	auto pstr = pOptions->Find(Controller);
	return  (pstr != nullptr);
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
bool CCLICommandManager::GetActive(bool& val)
{
	bool ret = false;
	auto pstr = pOptions->Find(Active);
	if (pstr != nullptr) {

		if (*pstr == "1") {
			val = true;
		}
		else {
			val = false;
		}
		return true;
	}


	return false;
}

bool CCLICommandManager::GetStart(bool& is_start)
{
	auto ret = pOptions->Find(Start);
	if (ret) {
		is_start = true;
		return true;
	}
	else {
		is_start = false;
	}
	return false;
}

bool CCLICommandManager::GetPause(bool& is_pause)
{
	auto ret = pOptions->Find(Pause);
	if (ret) {
		is_pause = true;
		return true;
	}
	else {
		is_pause = false;
	}
	return false;
}
bool CCLICommandManager::GetResume(bool& is_resume)
{
	auto ret = pOptions->Find(Resume);
	if (ret) {
		is_resume = true;
		return true;
	}
	else {
		is_resume = false;
	}
	return false;
}


bool CCLICommandManager::GetParent(FString &val)
{

	auto ret = pOptions->Find(Parent);
	if (ret != nullptr) {
		val = *ret;
		return true;
	}

	return false;
}

bool CCLICommandManager::GetContoroller(FString& val)
{
	auto ret = pOptions->Find(Controller);

	if (ret != nullptr) {
		val = *ret;
		return true;
	}

	return false;
}