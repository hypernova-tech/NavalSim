public class CCommandFactroy
{
    public static string CreateCommands(string cmd, 
        string option1 ="", string value1="", 
        string option2="", string value2="", 
        string option3="", string value3="", 
        string option4="", string value4="", 
        string option5 = "", string value5 = "",
        string option6 = "", string value6 = "",
        string option7 = "", string value7 = "",
        string option8 = "", string value8 = "",
        string option9 = "", string value9 = "",
        string option10 = "", string value10 = "",
        string option11 = "", string value11 = "",
        string option12 = "", string value12 = "")
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

        if (option6 != "")
        {
            ret += " --" + option6;
        }
        if (value6 != "")
        {
            ret += " " + value6;
        }

        if (option7 != "")
        {
            ret += " --" + option7;
        }
        if (value7 != "")
        {
            ret += " " + value7;
        }

        if (option8 != "")
        {
            ret += " --" + option8;
        }
        if (value8 != "")
        {
            ret += " " + value8;
        }

        if (option9 != "")
        {
            ret += " --" + option9;
        }
        if (value9 != "")
        {
            ret += " " + value9;
        }

        if (option10 != "")
        {
            ret += " --" + option10;
        }
        if (value10 != "")
        {
            ret += " " + value10;
        }

        if (option11 != "")
        {
            ret += " --" + option11;
        }
        if (value11 != "")
        {
            ret += " " + value11;
        }
        if (option12 != "")
        {
            ret += " --" + option12;
        }
        if (value12 != "")
        {
            ret += " " + value12;
        }
        return ret;
    }
}
