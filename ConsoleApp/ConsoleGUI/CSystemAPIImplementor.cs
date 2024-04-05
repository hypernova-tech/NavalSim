public enum ECoordSystem{
    XYZ,
    LLH

}
public enum ECameraView
{
    Top,
    Right,
    Left,
    Focus,
    Back,
    Front,

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
        string option11 = "", string value11 = "",
        string option12 = "", string value12 = "")
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
                                                    option11, value11,
                                                    option12, value12);
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
        CreateAndSendCommand(CLICommandManager.SetCommand, CLICommandManager.Name, Env, CLICommandManager.WindSpeed, (0.5 * percent).ToString());
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
                                ECoordSystem coord_system,
                                FVector top_left, FVector top_right)
    {

        string top_left_str = CUtil.MakeVectorString(top_left);
        string bottom_right_str = CUtil.MakeVectorString(top_right);

      

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
                            CLICommandManager.TerrTopLeftLLH, top_left_str,
                            CLICommandManager.TerrBottomRightLLH, bottom_right_str

                             );
        ;

        CreateAndSendCommand(CLICommandManager.SetCommand, CLICommandManager.Name, terrain_name, CLICommandManager.Bake);
    }

    internal void SetCameraSpeed(double multiplier)
    {
        double speed = multiplier * 100;
        CreateAndSendCommand(CLICommandManager.SetCommand, CLICommandManager.CamSpeed, speed.ToString());
    }

    internal void SetCameraView(ECameraView view, string selected)
    {
        string cmd = "";
        if (view == ECameraView.Top)
        {
            cmd = "top";
        }
        else if (view == ECameraView.Left)
        {
            cmd = "left";
        }
        else if (view == ECameraView.Right)
        {
            cmd = "right";
        }
        else if (view == ECameraView.Front)
        {
            cmd = "front";
        }
        else if (view == ECameraView.Back)
        {
            cmd = "back";
        }
        else if (view == ECameraView.Focus)
        {
            if(selected != "")
            {
                CreateAndSendCommand(CLICommandManager.SetCommand, CLICommandManager.Name, selected, CLICommandManager.Focused);
            }
            else
            {
                MessageBox.Show("First Select an Actor");
            }
            
            return;
        }

        CreateAndSendCommand(CLICommandManager.SetCommand, CLICommandManager.CamView, cmd);
    }

    internal void SetParent(string child, string parent_node)
    {
        CreateAndSendCommand(CLICommandManager.SetCommand, CLICommandManager.Name, child, CLICommandManager.Parent, parent_node);
    }

    internal void EnableAnnotation(bool v)
    {
        CreateAndSendCommand(CLICommandManager.AnnotationCommand, CLICommandManager.Enabled, v?"1":"0");
        QueryAnnotation();
    }

    internal void QueryAnnotation()
    {
        CreateAndSendCommand(CLICommandManager.AnnotationCommand, CLICommandManager.Query);
       
    }
    internal void EnableAnnotationSave(bool v)
    {
        CreateAndSendCommand(CLICommandManager.AnnotationCommand, CLICommandManager.SaveEnabled, v ? "1" : "0");
        QueryAnnotation();
    }

    internal void CameraAttach(string selected)
    {
        CreateAndSendCommand(CLICommandManager.AnnotationCommand, CLICommandManager.Query);

    }
    internal void SetCamFollow(string selected)
    {
        CreateAndSendCommand(CLICommandManager.SetCommand, CLICommandManager.CamFollow, selected);

    }
    


}
