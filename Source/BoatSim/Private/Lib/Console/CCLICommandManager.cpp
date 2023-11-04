// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/Console/CCLICommandManager.h"
#include <Lib/Utils/CUtil.h>
#include <Lib/SystemManager/SystemManagerBase.h>

FString CCLICommandManager::WorkspaceCommand = ("ws");
FString CCLICommandManager::SaveFile = ("save");
FString CCLICommandManager::LoadFile = ("load");

FString CCLICommandManager::ProcessCommand = "process";
FString CCLICommandManager::ProcessKill = "kill";

FString CCLICommandManager::CreateCommand = "create";
FString CCLICommandManager::Name = "name";
FString CCLICommandManager::Bp = "bp";
FString CCLICommandManager::Model = "model";

FString CCLICommandManager::DestroyCommand = "destroy";

FString CCLICommandManager::SetCommand = "set";
FString CCLICommandManager::Active = "active";
FString CCLICommandManager::Enabled = "enabled";
FString CCLICommandManager::Instance = "instance";
FString CCLICommandManager::Position = "position";
FString CCLICommandManager::RelPosition = "relposition";
FString CCLICommandManager::Rotation = "rotation";
FString CCLICommandManager::RelRotation = "relrotation";
FString CCLICommandManager::Scale = "scale";
FString CCLICommandManager::Parent = "parent";
FString CCLICommandManager::Controller = "controller";
FString CCLICommandManager::SensorSlotIndex = "slotindex";
FString CCLICommandManager::Beam = "beam";
FString CCLICommandManager::Range = "range";
FString CCLICommandManager::Fov = "fov";
FString CCLICommandManager::Selected = "selected";
FString CCLICommandManager::Focused = "focused";


FString CCLICommandManager::VericalFov = "vfov";
FString CCLICommandManager::HorizontalFov = "hfov";
FString CCLICommandManager::HorizontalScanStepAngleDeg = "hscanstepang";
FString CCLICommandManager::VerticalScanStepAngleDeg = "vscanstepang";
FString CCLICommandManager::MeasurementErrorMean= "merrormean";
FString CCLICommandManager::MeasurementErrorStd= "merrorstd";
FString CCLICommandManager::EnableSurfaceDetect = "surfacedetecten";
FString CCLICommandManager::EnableSubsurfaceDetect = "subsurfacedetecten";
FString CCLICommandManager::EnableFoamDetect = "foamen";
FString CCLICommandManager::SeaSurfaceDetectionProb = "seasurfdetectprob";
FString CCLICommandManager::MaxSurfacePenetration = "maxsurfpenet";

FString CCLICommandManager::RadarScanLevel = "radscanlevel";
FString CCLICommandManager::RadarScannerRPM = "radscanrpm";
FString CCLICommandManager::RadarGainType = "radgaintype";
FString CCLICommandManager::RadarGainLevel = "radgainlevel";
FString CCLICommandManager::RadarSeaClutterType = "radseaclutleveltype";
FString CCLICommandManager::RadarSeaClutterLevel = "radseaclutlevel";
FString CCLICommandManager::RadarSeaClutterAutoOffset = "radseaclutautooffset";
FString CCLICommandManager::RadarRainClutterLevel = "radrainclutlevel";
FString CCLICommandManager::RadarMaxGuardZoneCount = "radmaxguardzonecount";
FString CCLICommandManager::RadarMaxSectorBlankingZoneCount = "radmaxsectorblankingzonecount";
FString CCLICommandManager::CamWidthPx = "widthpx";
FString CCLICommandManager::CamHeightPx = "heightpx";

FString CCLICommandManager::Wp = "wp";
FString CCLICommandManager::WpPos = "wppos";
FString CCLICommandManager::Closed = "closed";
FString CCLICommandManager::Bake = "bake";
FString CCLICommandManager::Speed = "speed";
FString CCLICommandManager::Attach = "attach";
FString CCLICommandManager::Straight = "straight";
FString CCLICommandManager::LineColor = "color";
FString CCLICommandManager::SegmentCount = "segment";
FString CCLICommandManager::TurnRate = "turnrate";



FString CCLICommandManager::GetCommand = "get";
FString CCLICommandManager::All = "all";
FString CCLICommandManager::PrintCommand = "print";
FString CCLICommandManager::Actors = "actors";
FString CCLICommandManager::ActorBases = "actorbases";

FString CCLICommandManager::Sensors = "sensors";
FString CCLICommandManager::SensorType = "sensortype";
FString CCLICommandManager::SensorTypes = "sensortypes";

FString CCLICommandManager::SimCommand = "sim";
FString CCLICommandManager::Start = "start";
FString CCLICommandManager::Pause = "pause";
FString CCLICommandManager::Resume = "resume";



CCLICommandManager::CCLICommandManager()
{
	PrepareProcessCommandInfo();
	PrepareWorkspaceCommandInfo();
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
void CCLICommandManager::PrepareWorkspaceCommandInfo()
{
	TArray<SCommandOptionInfo> options;
	// create command
	SCommandOptionInfo info;

	info.Option = "--load";
	info.Description = "loads workspace from the given file";
	info.OptionType = EOptionType::TypeString;
	options.Add(info);

	info.Option = "--save";
	info.Description = "saves the workspace to given file";
	info.OptionType = EOptionType::TypeString;
	options.Add(info);



	CommandInfo.Add(WorkspaceCommand, options);
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

	info.Option = "--sensors";
	info.Description = "prints all sensors";
	options.Add(info);

	info.Option = "--name";
	info.Description = "prints actor or component with given name";
	options.Add(info);

	info.Option = "--sensortypes";
	info.Description = "prints available sensors types";
	options.Add(info);

	info.Option = "--sensortype";
	info.Description = "prints all sensors with given type";
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

	info.Option = "--enabled";
	info.Description = "get the set enable object of name, 0: pasive, 1:active";
	options.Add(info);

	info.Option = "--instance";
	info.Description = "sets the instance of  the process that actor will run";
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

	info.Option = "--" + CCLICommandManager::Selected;
	info.Description = "selected the actor given name";
	options.Add(info);

	info.Option = "--" + CCLICommandManager::HorizontalFov ;
	info.Description = "sets the horizontal fov of sensor";
	options.Add(info);

	info.Option = "--" + CCLICommandManager::VericalFov;
	info.Description = "sets the vertical fov of sensor";
	options.Add(info);

	info.Option = "--" + CCLICommandManager::HorizontalScanStepAngleDeg;
	info.Description = "sets the horizontal scan step angle";
	options.Add(info);

	info.Option = "--" + CCLICommandManager::VerticalScanStepAngleDeg;
	info.Description = "sets the vertical scan step angle";
	options.Add(info);

	info.Option = "--" + CCLICommandManager::MeasurementErrorMean;
	info.Description = "sets the measurment error mean";
	options.Add(info);

	info.Option = "--" + CCLICommandManager::MeasurementErrorStd;
	info.Description = "sets the measurement error standard deviation";
	options.Add(info);

	info.Option = "--" + CCLICommandManager::EnableSurfaceDetect;
	info.Description = "enable or disable sensor surface detect";
	options.Add(info);

	info.Option = "--" + CCLICommandManager::EnableSurfaceDetect;
	info.Description = "enable or disable sensor subsurface detect";
	options.Add(info);

	info.Option = "--" + CCLICommandManager::EnableFoamDetect;
	info.Description = "enable or disable foam detect";
	options.Add(info);

	info.Option = "--" + CCLICommandManager::SeaSurfaceDetectionProb;
	info.Description = "sets the water surface detection probablity";
	options.Add(info);

	info.Option = "--" + CCLICommandManager::MaxSurfacePenetration;
	info.Description = "sets the sensor surface penetration distance";
	options.Add(info);

	info.Option = "--" + CCLICommandManager::RadarScanLevel;
	info.Description = "sets radar scan level";
	options.Add(info);

	info.Option = "--" + CCLICommandManager::RadarScannerRPM;
	info.Description = "sets the radar scanner rpm";
	options.Add(info);

	info.Option = "--" + CCLICommandManager::RadarGainType;
	info.Description = "sets the radar gain type";
	options.Add(info);

	info.Option = "--" + CCLICommandManager::RadarGainLevel;
	info.Description = "sets the radar gain level";
	options.Add(info);

	info.Option = "--" + CCLICommandManager::RadarSeaClutterType;
	info.Description = "sets the radar sea clutter tpye";
	options.Add(info);

	info.Option = "--" + CCLICommandManager::RadarSeaClutterLevel;
	info.Description = "sets the radar sea clutter level";
	options.Add(info);

	info.Option = "--" + CCLICommandManager::RadarSeaClutterAutoOffset;
	info.Description = "sets the radar sea clutter auto offset";
	options.Add(info);

	info.Option = "--" + CCLICommandManager::RadarRainClutterLevel;
	info.Description = "sets the radar rain clutter level";
	options.Add(info);

	info.Option = "--" + CCLICommandManager::RadarMaxGuardZoneCount;
	info.Description = "sets the radar max guard zone count";
	options.Add(info);

	info.Option = "--" + CCLICommandManager::RadarMaxSectorBlankingZoneCount;
	info.Description = "sets the radar max. sektor blanking zone count";
	options.Add(info);

	info.Option = "--" + CCLICommandManager::Wp;
	info.Description = "sets the index of the waypoint of given path of name";
	options.Add(info);

	info.Option = "--" + CCLICommandManager::WpPos;
	info.Description = "sets the position of wp with index of path of name";
	options.Add(info);

	info.Option = "--" + CCLICommandManager::Closed;
	info.Description = "make path closed(1) or open(0)";
	options.Add(info);

	info.Option = "--" + CCLICommandManager::Bake;
	info.Description = "bake the given object";
	options.Add(info);

	info.Option = "--" + CCLICommandManager::Speed;
	info.Description = "sets the speed of the attached object";
	options.Add(info);

	info.Option = "--" + CCLICommandManager::Attach;
	info.Description = "attach the object given actor name";
	options.Add(info);

	info.Option = "--" + CCLICommandManager::SegmentCount;
	info.Description = "set the segment couunt of drawn path the object given actor name";
	options.Add(info);

	info.Option = "--" + CCLICommandManager::LineColor;
	info.Description = "set the color of path the object given actor name";
	options.Add(info);

	info.Option = "--" + CCLICommandManager::Straight;
	info.Description = "set the path straight of given path of name";
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

	info.Option = "--enabled";
	info.Description = "get the  object of name enabled or disabled, 0: disabled, 1:enabled";
	options.Add(info);

	info.Option = "--instance";
	info.Description = "gets the instance of  the process that actor will run";
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

	info.Option = "--" + CCLICommandManager::Selected;
	info.Description = "get selected actor";
	options.Add(info);


	info.Option = "--" + CCLICommandManager::HorizontalFov;
	info.Description = "gets the horizontal fov of sensor";
	options.Add(info);

	info.Option = "--" + CCLICommandManager::VericalFov;
	info.Description = "gets the vertical fov of sensor";
	options.Add(info);

	info.Option = "--" + CCLICommandManager::HorizontalScanStepAngleDeg;
	info.Description = "gets the horizontal scan step angle";
	options.Add(info);

	info.Option = "--" + CCLICommandManager::VerticalScanStepAngleDeg;
	info.Description = "gets the vertical scan step angle";
	options.Add(info);

	info.Option = "--" + CCLICommandManager::MeasurementErrorMean;
	info.Description = "gets the measurment error mean";
	options.Add(info);

	info.Option = "--" + CCLICommandManager::MeasurementErrorStd;
	info.Description = "gets the measurement error standard deviation";
	options.Add(info);

	info.Option = "--" + CCLICommandManager::EnableSurfaceDetect;
	info.Description = "gets enable or disable sensor surface detect";
	options.Add(info);

	info.Option = "--" + CCLICommandManager::EnableSurfaceDetect;
	info.Description = "gets enable or disable sensor subsurface detect";
	options.Add(info);

	info.Option = "--" + CCLICommandManager::EnableFoamDetect;
	info.Description = "enable or disable foam detect";
	options.Add(info);

	info.Option = "--" + CCLICommandManager::SeaSurfaceDetectionProb;
	info.Description = "gets the water surface detection probablity";
	options.Add(info);

	info.Option = "--" + CCLICommandManager::MaxSurfacePenetration;
	info.Description = "gets the sensor surface penetration distance";
	options.Add(info);

	info.Option = "--" + CCLICommandManager::RadarScanLevel;
	info.Description = "gets radar scan level";
	options.Add(info);

	info.Option = "--" + CCLICommandManager::RadarScannerRPM;
	info.Description = "gets the radar scanner rpm";
	options.Add(info);

	info.Option = "--" + CCLICommandManager::RadarGainType;
	info.Description = "gets the radar gain type";
	options.Add(info);

	info.Option = "--" + CCLICommandManager::RadarGainLevel;
	info.Description = "gets the radar gain level";
	options.Add(info);

	info.Option = "--" + CCLICommandManager::RadarSeaClutterType;
	info.Description = "gets the radar sea clutter tpye";
	options.Add(info);

	info.Option = "--" + CCLICommandManager::RadarSeaClutterLevel;
	info.Description = "gets the radar sea clutter level";
	options.Add(info);

	info.Option = "--" + CCLICommandManager::RadarSeaClutterAutoOffset;
	info.Description = "gets the radar sea clutter auto offset";
	options.Add(info);

	info.Option = "--" + CCLICommandManager::RadarRainClutterLevel;
	info.Description = "gets the radar rain clutter level";
	options.Add(info);

	info.Option = "--" + CCLICommandManager::RadarMaxGuardZoneCount;
	info.Description = "gets the radar max guard zone count";
	options.Add(info);

	info.Option = "--" + CCLICommandManager::RadarMaxSectorBlankingZoneCount;
	info.Description = "gets the radar max. sektor blanking zone count";
	options.Add(info);

	info.Option = "--" + CCLICommandManager::Wp;
	info.Description = "gets the waypoint info at index given path of name";
	options.Add(info);

	info.Option = "--" + CCLICommandManager::WpPos;
	info.Description = "gets the position of wp with index of path of name";
	options.Add(info);

	info.Option = "--" + CCLICommandManager::Closed;
	info.Description = "gets path closed(1) or not (0)";
	options.Add(info);

	info.Option = "--" + CCLICommandManager::Attach;
	info.Description = "gets the attached actor to path of name";
	options.Add(info);


	info.Option = "--" + CCLICommandManager::SegmentCount;
	info.Description = "get the segment count of drawn path the object given actor name";
	options.Add(info);

	info.Option = "--" + CCLICommandManager::LineColor;
	info.Description = "get the color of path the object given actor name";
	options.Add(info);

	info.Option = "--" + CCLICommandManager::Straight;
	info.Description = "get the path straight (1) or not (0) of given path of name";
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
bool CCLICommandManager::HasSensors()
{
	auto ret = pOptions->Find(Sensors);
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
SCommandOptionInfo* CCLICommandManager::FindCommandOptionInfo(FString option)
{
	return nullptr;
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
bool CCLICommandManager::HasEnabled()
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

bool CCLICommandManager::HasA(FString str) {

	auto pstr = pOptions->Find(str);
	
	if (pstr != nullptr) {
		return true;
	}
	else {
		return false;
	}
}

bool CCLICommandManager::GetValue(FString &name, FVector& vec)
{
	bool ret = false;
	auto pstr = pOptions->Find(name);
	if (pstr != nullptr) {

		ret = CUtil::ParseVector3D(*pstr, vec);
		return ret;
	}
	return false;
}

bool CCLICommandManager::GetValue(FString& name, FColor& vec)
{
	bool ret = false;
	auto pstr = pOptions->Find(name);
	if (pstr != nullptr) {

		ret = CUtil::ParseColor(*pstr, vec);
		return ret;
	}
	return false;
}

bool CCLICommandManager::GetValue(FString& name, FVector2D& val)
{
	bool ret = false;
	auto pstr = pOptions->Find(name);
	if (pstr != nullptr) {

		ret = CUtil::ParseVector2D(*pstr, val);
		return ret;
	}
	return false;
}
bool CCLICommandManager::GetValue(FString& name, FLOAT64& val)
{
	bool ret = false;
	auto pstr = pOptions->Find(name);
	if (pstr != nullptr) {

		val = CUtil::StringToFloat64(*pstr);
		return true;
	}
	return false;
}
bool CCLICommandManager::GetValue(FString& name, INT32S& val)
{
	bool ret = false;
	auto pstr = pOptions->Find(name);
	if (pstr != nullptr) {

		val = CUtil::StringToInt32S(*pstr);
		return true;
	}
	return false;
}



bool CCLICommandManager::GetValue(FString& name, BOOLEAN& val)
{
	bool ret = false;
	auto pstr = pOptions->Find(name);
	if (pstr != nullptr) {

		val = (*pstr == TEXT("1"))?true:false;
		return true;
	}
	return false;
}
bool CCLICommandManager::GetValue(FString& name, FString& val)
{
	bool ret = false;
	auto pstr = pOptions->Find(name);
	if (pstr != nullptr) {

		val = (*pstr);
		return true;
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
bool CCLICommandManager::GetEnabled(bool& val)
{
	bool ret = false;
	auto pstr = pOptions->Find(Enabled);
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


bool CCLICommandManager::GetInstance(int& val)
{
	bool ret = false;
	auto pstr = pOptions->Find(Instance);
	if (pstr != nullptr) {

		val = CUtil::StringToInt(*pstr);
		return true;
	}


	return false;
}

bool CCLICommandManager::GetSensorType(FString& val)
{
	bool ret = false;
	auto pstr = pOptions->Find(SensorType);
	if (pstr != nullptr) {

		val = (*pstr);
		return true;
	}


	return false;
}
bool CCLICommandManager::HasSensorTypes()
{
	bool ret = false;
	auto pstr = pOptions->Find(SensorTypes);
	if (pstr != nullptr) {

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