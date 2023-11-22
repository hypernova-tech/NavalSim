public class CSystemAPIImplementor
{
    IConnection Connection;
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
}
