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

    void CreateAndSendCommand(string cmd, string option1 = "", string value1 = "", string option2 = "", string value2 = "", string option3 = "", string value3 = "", string option4 = "", string value4 = "", string option5 = "", string value5 = "")
    {
        string ret = CCommandFactroy.CreateCommands(cmd, option1, value1, option2, value2, option3, value3, option4, value4, option5, value5);
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
}
