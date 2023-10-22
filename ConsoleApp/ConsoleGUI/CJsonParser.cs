using ConsoleGUI;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using System.Collections.Generic;
using static System.Net.Mime.MediaTypeNames;

public class CConverter
{
    
    public static double UEToWorld(double val)
    {
        return val * 0.01;
    }

    public static double WorldToUE(double val)
    {
        return val * 100;
    }

    public static void UEToWorld(FVector val)
    {
        val.X *=  0.01;
        val.Y *= 0.01;
        val.Z *= 0.01;
    }

    public static void WorldToUE(FVector val)
    {
        val.X *= 100;
        val.Y *= 100;
        val.Z *= 100;
    }
}

public class CJsonParser
{
    MainForm mMainForm;
    public CJsonParser( ) 
    {
        
    }

    public void SetMainForm(MainForm form)
    {
        mMainForm = form;
    }
    bool IsJsonValid(string strInput)
    {
        // Trim whitespace from the start and end of the string
        strInput = strInput.Trim();

        // Check for obvious signs that the string is not JSON
        if ((!strInput.StartsWith("{") || !strInput.EndsWith("}")) && // For object
            (!strInput.StartsWith("[") || !strInput.EndsWith("]"))) // For array
        {
            return false;
        }

        try
        {
            // Try to parse the string as JSON
            var obj = JToken.Parse(strInput);
            return true;
        }
        catch (JsonReaderException) // Exception thrown by Newtonsoft.Json for invalid JSON
        {
            return false;
        }
        catch (Exception) // Any other exception
        {
            return false;
        }
    }
    public void TryParseJson(string message)
    {
        if (!IsJsonValid(message))
        {
            return;
        }
        var json_obj = JObject.Parse(message);

        if (json_obj != null)
        {
            var name = json_obj["name"];
            var option = json_obj["option"];
            var value = json_obj["value"];

            if (name != null)
            {
                ProcessOptions(name, option, value);
            }

        }
    }


    bool ParseVector(string str, FVector vec)
    {
        var strings = str.Split(' ');

        if (strings.Length != 3)
        {
            return false;
        }

        bool ret = double.TryParse(strings[0], out vec.X);
        if (!ret)
        {
            return false;
        }
        ret = double.TryParse(strings[1], out vec.Y);
        if (!ret)
        {
            return false;
        }
        ret = double.TryParse(strings[2], out vec.Z);
        if (!ret)
        {
            return false;
        }

        return true;

    }

    private void ProcessOptions(JToken name, JToken? option, JToken? value)
    {
        if (name.Value<string>() == mMainForm.GetSelectedSensor())
        {
            string option_str = option.Value<string>();
            string option_value = value.Value<string>();

            if (option_str != null)
            {
                if (option_str == "position")
                {
                    FVector vec = new FVector();
                    if(ParseVector(option_value, vec))
                    {
                        CConverter.UEToWorld(vec);
                        mMainForm.SetPosition(vec.X, vec.Y, vec.Z);
                    }
                }
                else if (option_str == "rotation")
                {
                    FVector vec = new FVector();
                    if (ParseVector(option_value, vec))
                    {
                        mMainForm.SetRotation(vec.X, vec.Y, vec.Z);
                    }
                }
                else if (option_str == "scale")
                {
                    FVector vec = new FVector();
                    if (ParseVector(option_value, vec))
                    {
                        mMainForm.SetScale(vec.X, vec.Y, vec.Z);
                    }
                }

            }
        }
    }


}
