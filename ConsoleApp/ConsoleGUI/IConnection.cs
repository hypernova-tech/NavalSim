public interface IConnection
{
    public void SendData(string cmd, bool add_to_cmd_list = true);
}
