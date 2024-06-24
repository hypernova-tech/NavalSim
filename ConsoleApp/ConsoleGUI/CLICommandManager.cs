using System.Reflection;
using static System.Windows.Forms.VisualStyles.VisualStyleElement.ListView;

[AttributeUsage(AttributeTargets.Field, AllowMultiple = false)]
public class ModifiableAttribute : Attribute
{
    // Property to indicate if the field can be modified via CLI
    public bool IsProperty { get; set; }

    // Constructor
    public ModifiableAttribute(bool is_property = false)
    {
        IsProperty = is_property;
    }
}

public class COptionInfo
{
    public Dictionary<string, string> OptionNameContants = new Dictionary<string, string>();
    public HashSet<string> OptionWithPropertyEnabled = new HashSet<string>();
}

public static class CLICommandManager
{
    public const string WorkspaceCommand = "ws";
    public const string SaveFile = "save";
    public const string LoadFile = "load";
    public const string ProcessCommand = "process";
    public const string ProcessKill = "kill";
    public const string AnnotationCommand = "annotation";
    public const string Query = "query";
    public const string SaveEnabled = "saveen";
    public const string CreateCommand = "create";
    public const string Name = "name";
    public const string Bp = "bp";
    public const string Model = "model";
    public const string DestroyCommand = "destroy";
    public const string SetCommand = "set";
    [Modifiable] public const string Suppressed = "suppressed";
    [Modifiable] public const string Active = "active";
    [Modifiable] public const string Enabled = "enabled";
    [Modifiable] public const string ActorId = "actorid";
    [Modifiable] public const string StandaloneModeEnabled = "standalone";
    public const string Instance = "instance";
    public const string Position = "position";
    public const string RelPosition = "relposition";
    public const string Rotation = "rotation";
    public const string RelRotation = "relrotation";
    public const string Scale = "scale";
    [Modifiable] public const string Parent = "parent";
     public const string FullPath = "fullpath";
    public const string Controller = "controller";
    [Modifiable] public const string SensorFrequency = "sensorfreq";
    [Modifiable] public const string SensorSlotIndex = "slotindex";
    [Modifiable] public const string Beam = "beam";
    [Modifiable] public const string PointCloud3D = "pointcloud3d";
    [Modifiable] public const string PointCloud3DSizze = "pointcloud3dsize";
    [Modifiable] public const string RangeMin = "rangemin";
    [Modifiable] public const string RangeMax = "rangemax";
    public const string UIVisible = "uivisible";
    public const string Selected = "selected";
    public const string Focused = "focused";
    [Modifiable] public const string IPAddr1 = "ipaddr1";
    [Modifiable] public const string LocalPort1 = "localport1";
    [Modifiable] public const string RemotePort1 = "remoteport";
    [Modifiable] public const string IPAddr2 = "ipaddr2";
    [Modifiable] public const string LocalPort2 = "localport2";
    [Modifiable] public const string RemotePort2 = "remoteport2";
    [Modifiable] public const string VerticalFov = "vfov";
    [Modifiable] public const string HorizontalFov = "hfov";
    [Modifiable] public const string HorizontalScanStepAngleDeg = "hscanstepang";
    [Modifiable] public const string VerticalScanStepAngleDeg = "vscanstepang";
    [Modifiable] public const string MotionLogEnabled = "motionlogen";
    [Modifiable] public const string SharedMemName      = "sharedmemname";
    [Modifiable] public const string GStreamerIP        = "gstreamerip";
    [Modifiable] public const string GStreamerPort      = "gstreamerport";

    [Modifiable] public const string GimbalMode = "gimbalmode";
    [Modifiable] public const string GimbalAxisEnabled = "gimbalaxisen";
    [Modifiable] public const string GimbalCmd = "gimbalcmd";
    [Modifiable] public const string GimbalAng = "gimbalang";
    [Modifiable] public const string GimbalInitialAng = "gimbalinitialang";    
    [Modifiable] public const string GimbalRate = "gimbalrate";
    [Modifiable] public const string GimbalFixedRate = "gimbalfixedrate";
    [Modifiable] public const string GimbalMaxLimitAng = "gimbalmaxlimitang";
    [Modifiable] public const string GimbalMinLimitAng = "gimbalminlimitang";
    [Modifiable] public const string GimbalFreeRotateEnabled = "gimbalfreerotateen";

    [Modifiable] public const string IsNormalDistribution = "isnormaldistribution";
    [Modifiable] public const string MeasurementErrorMean = "merrormean";
    [Modifiable] public const string MeasurementErrorStd = "merrorstd";
    [Modifiable] public const string EnableSurfaceDetect = "surfacedetecten";
    [Modifiable] public const string EnableSubsurfaceDetect = "subsurfacedetecten";
    [Modifiable] public const string EnableFoamDetect = "foamen";
    [Modifiable] public const string SeaSurfaceDetectionProb = "seasurfdetectprob";
    [Modifiable] public const string MaxSurfacePenetration = "maxsurfpenet";
    [Modifiable] public const string RadarAutoDetectionTrackingEnabled = "radautodetecttracken";
    [Modifiable] public const string RadarScanLevel = "radscanlevel";
    [Modifiable] public const string RadarScannerRPM = "radscanrpm";
    [Modifiable] public const string RadarGainType = "radgaintype";
    [Modifiable] public const string RadarGainLevel = "radgainlevel";
    [Modifiable] public const string RadarSeaClutterType = "radseaclutleveltype";
    [Modifiable] public const string RadarSeaClutterLevel = "radseaclutlevel";
    [Modifiable] public const string RadarSeaClutterAutoOffset = "radseaclutautooffset";
    [Modifiable] public const string RadarRainClutterLevel = "radrainclutlevel";
    [Modifiable] public const string RadarMaxGuardZoneCount = "radmaxguardzonecount";
    [Modifiable] public const string RadarMaxSectorBlankingZoneCount = "radmaxsectorblankingzonecount";
    [Modifiable] public const string CamWidthPx = "widthpx";
    [Modifiable] public const string CamHeightPx = "heightpx";
    [Modifiable] public const string CamSpeed = "camspeed";
    [Modifiable] public const string PlatformSpeed = "pltspeed";
    [Modifiable] public const string PlatformTurnRate = "pltturnrate";
    [Modifiable] public const string CamFollow = "camfollow";
    [Modifiable] public const string CamView = "camview";
    [Modifiable] public const string IsBlockingObject = "blocking";
    [Modifiable] public const string AISUserId = "aisuserid"; 
    [Modifiable] public const string AISClassType = "aisclass";
    [Modifiable] public const string AISMessagePublishPeriodSecStationary = "aispublishperiodstatic";
    [Modifiable] public const string AISMessagePublishPeriodSecMoving = "aispublishperiodmoving";
    [Modifiable] public const string AISShouldPublishAton = "aispublishaton";
    [Modifiable] public const string AISVendorId = "aisvendorid";
    [Modifiable] public const string AISCallSign = "aiscallsign";
    [Modifiable] public const string AISShipOrCargoType = "aisshiporcargotype";
    [Modifiable] public const string AISReferencePointFromStarboard = "airrefptfromstarboard";
    [Modifiable] public const string ReferencePointPositionAftOfBow = "airrefptaftofbow";
    [Modifiable] public const string AISMMSI = "aismmsi";
    [Modifiable] public const string AISName = "aisname";
    [Modifiable] public const string AISDateOfArrival = "aisdateofarrival";
    [Modifiable] public const string AISTimeOfArrival = "aistimeofarrival";



    [Modifiable] public const string ThermalMode = "thermalmode";
    //[Modifiable(IsProperty = true)] public const string AnnotationId = "annotationid";
    [Modifiable] public const string AnnotationId = "annotationid";
    [Modifiable] public const string AnnotateChildren = "annotatechildren";
    [Modifiable] public const string TempratureKelvin = "tempkelvin";
    //[Modifiable(IsProperty = true)] public const string IsHeatSource = "heatsource";
    [Modifiable] public const string IsHeatSource = "heatsource";
    [Modifiable] public const string TimeOfDay = "timeofday";
    [Modifiable] public const string EnvTimeFlowScale = "envtimeflowscale";
    [Modifiable] public const string RainPercent = "rain";
    [Modifiable] public const string CloudPercent = "cloud";
    [Modifiable] public const string SnowPercent = "snow";
    [Modifiable] public const string DustPercent = "dust";
    [Modifiable] public const string FogPercent = "fog";
    [Modifiable] public const string ThunderPercent= "thunder";
    [Modifiable] public const string EnvSoundFxEnabled = "envsoundfx";
    [Modifiable] public const string WindSpeed = "wind";
    [Modifiable] public const string WindDirection = "winddir";

    [Modifiable] public const string TerrHMap = "terrhmap";
    [Modifiable] public const string TerrDMap = "terrdmap";
    [Modifiable] public const string TerrBaseTexture = "terrbasetexture";
    [Modifiable] public const string TerrHMapMinLvlMt = "terrhmapminlvl";
    [Modifiable] public const string TerrHMapMaxLvlMt = "terrhmapmaxlvl";
    [Modifiable] public const string TerrDMapMinLvlMt = "terrdmapminlvl";
    [Modifiable] public const string TerrDMapMaxLvlMt = "terrdmapmaxlvl";
    [Modifiable] public const string TerrTopLeftLLH = "terrtopleftllh";
    [Modifiable] public const string TerrBottomRightLLH = "terrbottomrightllh";

    public const string Wp = "wp";
    public const string WpPos = "wppos";
    public const string Closed = "closed";
    public const string Bake = "bake";
    public const string Speed = "speed";
    public const string Attach = "attach";
    public const string Detach = "detach";
    public const string Straight = "straight";
    public const string LineColor = "color";
    public const string SegmentCount = "segment";
    public const string TurnRate = "turnrate";

    [Modifiable] public const string MapOriginLat = "maporiginlat";
    [Modifiable] public const string MapOriginLon = "maporiginlon";
    [Modifiable] public const string MapOriginHeight = "maporiginh";
    public const string GetCommand = "get";
    public const string All = "all";
    
    public const string PrintCommand = "print";
    public const string Actors = "actors";
    public const string Sensors = "sensors";
    public const string SensorType = "sensortype";
    public const string SensorTypes = "sensortypes";
    public const string SimCommand = "sim";
    public const string Start = "start";
    public const string Pause = "pause";
    public const string Resume = "resume";
    public const string ExecCommand = "exec";
    public const string Property = "property";
    public const string Properties = "properties";
    public const string PropertyValue = "value";

    public const string Func = "func";
    public const string FuncCategory = "funccategory";
    public const string FuncParam1 = "param1";
    public const string FuncParam2 = "param2";
    public const string FuncParam3 = "param3";
    public const string FuncParam4 = "param4";
    public const string FuncParam5 = "param5";
    public const string FuncParam6 = "param6";
    public const string FuncParam7 = "param7";
    public const string FuncParam8 = "param8";
    public const string FuncParam9 = "param9";
    public const string FuncParam10 = "param10";

    public const string FuncValue1 = "value1";
    public const string FuncValue2 = "value2";
    public const string FuncValue3 = "value3";
    public const string FuncValue4 = "value4";
    public const string FuncValue5 = "value5";
    public const string FuncValue6 = "value6";
    public const string FuncValue7 = "value7";
    public const string FuncValue8 = "value8";
    public const string FuncValue9 = "value9";
    public const string FuncValue10 = "value10";

    public static Dictionary<string, string> GetModifiableConstants()
    {
        var modifiableFields = new Dictionary<string, string>();

        // Using reflection to get all public static fields
        var fields = typeof(CLICommandManager).GetFields(BindingFlags.Public | BindingFlags.Static);

        foreach (var field in fields)
        {
            var attributes = field.GetCustomAttributes(typeof(ModifiableAttribute), false);
            if (attributes.Length > 0)
            {
                // If the field has the ModifiableAttribute, we add it to the dictionary
                modifiableFields.Add(field.Name, (string)field.GetValue(null));
            }
        }

        return modifiableFields;
    }
    public static COptionInfo GetOptionNameContants()
    {
        COptionInfo option_info = new COptionInfo();
        var modifiableFields = new Dictionary<string, string>();

        var properties = new HashSet<string>();

        // Using reflection to get all public static fields
        var fields = typeof(CLICommandManager).GetFields(BindingFlags.Public | BindingFlags.Static);

        foreach (var field in fields)
        {
            var attributes = field.GetCustomAttributes(typeof(ModifiableAttribute), false);
            if (attributes.Length > 0)
            {
                var modifiable = (ModifiableAttribute)attributes[0];

                // If the field has the ModifiableAttribute, we add it to the dictionary
                modifiableFields.Add((string)field.GetValue(null), field.Name);
                if (modifiable.IsProperty)
                {
                    properties.Add(field.Name);
                }
            }
        }
        option_info.OptionNameContants = modifiableFields;
        option_info.OptionWithPropertyEnabled = properties;
        return option_info;
    }
}
