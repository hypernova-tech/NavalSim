public enum ECoordSystem{
    XYZ,
    LLH

}


public class CSystemAPIImplementor
{
    IConnection Connection;
    string Env = "env";
    public void SetUIVisible(bool visible)
    {
        CreateAndSendCommand(CLICommandManager.SetCommand, CLICommandManager.UIVisible, CUtil.BoolToBinaryString(visible));
    }

    internal void SetConnection(IConnection connection)
    {
        Connection = connection;
    }

    void CreateAndSendCommand(string cmd, 
        
        string option1 = "", string value1 = "", 
        string option2 = "", string value2 = "", 
        string option3 = "", string value3 = "", 
        string option4 = "", string value4 = "", 
        string option5 = "", string value5 = "",
        string option6 = "", string value6 = "",
        string option7 = "", string value7 = "",
        string option8 = "", string value8 = "",
        string option9 = "", string value9 = "",
        string option10 = "", string value10 = "",
        string option11 = "", string value11 = "")
    {
        string ret = CCommandFactroy.CreateCommands(cmd, 
                                                    option1, value1, 
                                                    option2, value2, 
                                                    option3, value3, 
                                                    option4, value4, 
                                                    option5, value5,
                                                    option6, value6,
                                                    option7, value7,
                                                    option8, value8,
                                                    option9, value9,
                                                    option10, value10,
                                                    option11, value11);
        Connection.SendData(ret);
    }

    public void SetCloudPercent(double val)
    {
        CreateAndSendCommand(CLICommandManager.SetCommand, CLICommandManager.Name, Env, CLICommandManager.CloudPercent, val.ToString());
    }
    public void SetRainPercent(double val)
    {
        CreateAndSendCommand(CLICommandManager.SetCommand, CLICommandManager.Name, Env, CLICommandManager.RainPercent, val.ToString());
    }
    public void SetSnowPercent(double val)
    {
        CreateAndSendCommand(CLICommandManager.SetCommand, CLICommandManager.Name, Env, CLICommandManager.SnowPercent, val.ToString());
    }
    public void SetFogPercent(double val)
    {
        CreateAndSendCommand(CLICommandManager.SetCommand, CLICommandManager.Name, Env, CLICommandManager.FogPercent, val.ToString());
    }
    public void SetDustPercent(double val)
    {
        CreateAndSendCommand(CLICommandManager.SetCommand, CLICommandManager.Name, Env, CLICommandManager.DustPercent, val.ToString());
    }
    public void SetThunderPercent(double val)
    {
        CreateAndSendCommand(CLICommandManager.SetCommand, CLICommandManager.Name, Env, CLICommandManager.ThunderPercent, val.ToString());
    }

    internal void SetTimeOfDayHr(double percent)
    {

        double tof = GetTimeOfDayHour(percent);
        
        CreateAndSendCommand(CLICommandManager.SetCommand, CLICommandManager.Name, Env, CLICommandManager.TimeOfDay, tof.ToString());
    }
    internal void SetWindSpeed(double percent)
    { 
        CreateAndSendCommand(CLICommandManager.SetCommand, CLICommandManager.Name, Env, CLICommandManager.WindSpeed, (0.5*percent).ToString());
    }

    internal void SetWindDirection(double percent)
    {
        double ang = percent * 0.01 * 360;
        CreateAndSendCommand(CLICommandManager.SetCommand, CLICommandManager.Name, Env, CLICommandManager.WindDirection, ang.ToString());
    }


    internal double GetTimeOfDayHour(double percent)
    {
        double tof = percent / 100.0 * 24;
        return tof;
    }

    internal void CreateTerrain(string terrain_name, 
                                string hmap_name, string depth_texture_name, string imagery_texture_name, 
                                double hmap_min_level, double hmap_max_level,
                                double dmap_min_level, double dmap_max_level,
                                double width, double len,
                                double x, double y, double z, ECoordSystem coord_system )
    {

        string coord = CUtil.MakeVectorString(x, y, z);
        string coord_cmd = CLICommandManager.TerrLowerLeftCornerXYZ;

        if (coord_system == ECoordSystem.LLH)
        {
            coord_cmd = CLICommandManager.TerrLowerLeftCornerLLH;
        }

        CreateAndSendCommand(CLICommandManager.CreateCommand, CLICommandManager.Name, terrain_name, CLICommandManager.Bp, "TERRAIN");

        CreateAndSendCommand(CLICommandManager.SetCommand,
                             CLICommandManager.Name, terrain_name,
                             CLICommandManager.TerrHMap, hmap_name,
                             CLICommandManager.TerrDMap, depth_texture_name,
                             CLICommandManager.TerrBaseTexture, imagery_texture_name,
                             CLICommandManager.TerrHMapMinLvlMt, hmap_min_level.ToString(),
                             CLICommandManager.TerrHMapMaxLvlMt, hmap_max_level.ToString(),
                             CLICommandManager.TerrDMapMinLvlMt, dmap_min_level.ToString(),
                             CLICommandManager.TerrDMapMaxLvlMt, dmap_max_level.ToString(),
                             CLICommandManager.TerrWidthMt, width.ToString(),
                             CLICommandManager.TerrLengthMt, len.ToString(),
                             coord_cmd, coord

                             );
        ;

        CreateAndSendCommand(CLICommandManager.SetCommand, CLICommandManager.Name, terrain_name, CLICommandManager.Bake);
    }
}
