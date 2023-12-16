using System.Reflection;
using static System.Windows.Forms.VisualStyles.VisualStyleElement.ListView;

[AttributeUsage(AttributeTargets.Field, AllowMultiple = false)]
public class ModifiableAttribute : Attribute
{
}

public static class CLICommandManager
{
    public const string WorkspaceCommand = "ws";
    public const string SaveFile = "save";
    public const string LoadFile = "load";
    public const string ProcessCommand = "process";
    public const string ProcessKill = "kill";
    public const string CreateCommand = "create";
    public const string Name = "name";
    public const string Bp = "bp";
    public const string Model = "model";
    public const string DestroyCommand = "destroy";
    public const string SetCommand = "set";
    public const string Active = "active";
    public const string Enabled = "enabled";
    public const string Instance = "instance";
    public const string Position = "position";
    public const string RelPosition = "relposition";
    public const string Rotation = "rotation";
    public const string RelRotation = "relrotation";
    public const string Scale = "scale";
    [Modifiable] public const string Parent = "parent";
    public const string Controller = "controller";
    [Modifiable] public const string SensorSlotIndex = "slotindex";
    [Modifiable] public const string Beam = "beam";
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
    [Modifiable] public const string MeasurementErrorMean = "merrormean";
    [Modifiable] public const string MeasurementErrorStd = "merrorstd";
    [Modifiable] public const string EnableSurfaceDetect = "surfacedetecten";
    [Modifiable] public const string EnableSubsurfaceDetect = "subsurfacedetecten";
    [Modifiable] public const string EnableFoamDetect = "foamen";
    [Modifiable] public const string SeaSurfaceDetectionProb = "seasurfdetectprob";
    [Modifiable] public const string MaxSurfacePenetration = "maxsurfpenet";
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
    [Modifiable] public const string TerrWidthMt = "terrwidth";
    [Modifiable] public const string TerrLengthMt = "terrlen";
    [Modifiable] public const string TerrLowerLeftCornerXYZ = "terrlowerleftcornerxyz";
    [Modifiable] public const string TerrLowerLeftCornerLLH = "terrlowerleftcornerllh";

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
    public static Dictionary<string, string> GetOptionNameContants()
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
                modifiableFields.Add((string)field.GetValue(null), field.Name);
            }
        }

        return modifiableFields;
    }
}
