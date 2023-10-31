public class CCommandFactroy
{
    public static string CreateCommands(string cmd, string option1 ="", string value1="", string option2="", string value2="", string option3="", string value3="", string option4="", string value4="", string option5 = "", string value5 = "")
    {
        string ret = cmd;
        if(option1 != "")
        {
            ret += " --" + option1;
        }
        if (value1 != "")
        {
            ret += " " + value1;
        }

        if (option2 != "")
        {
            ret += " --" + option2;
        }
        if (value2 != "")
        {
            ret += " " + value2;
        }

        if (option3 != "")
        {
            ret += " --" + option3;
        }
        if (value3 != "")
        {
            ret += " " + value3;
        }

        if (option4 != "")
        {
            ret += " --" + option4;
        }
        if (value4 != "")
        {
            ret += " " + value4;
        }


        if (option5 != "")
        {
            ret += " --" + option5;
        }
        if (value5 != "")
        {
            ret += " " + value5;
        }

        return ret;
    }
}
