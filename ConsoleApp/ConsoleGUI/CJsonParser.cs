﻿using ConsoleGUI;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using System.Collections.Generic;
using static System.Net.Mime.MediaTypeNames;

public class CJsonParser
{
    MainForm mMainForm;
    Dictionary<string, string> Options;
    Dictionary<string, string> OptionParamMap = new Dictionary<string, string>();
    public CJsonParser( ) 
    {
        Options = CLICommandManager.GetModifiableConstants();
        OptionParamMap = CLICommandManager.GetOptionNameContants();
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
            var class_val = json_obj["class"];
            
            if(class_val != null)
            {
                if(class_val.Value<string>() == "bp")
                {
                    ParseModels(json_obj);
                }

            }

            else if (name != null && option != null)
            {
                var value = json_obj["value"];
                ProcessOptions(name, option, value);
            }
            else
            {
                foreach (var pair in json_obj)
                {
                    string key = pair.Key;
                    JToken value = pair.Value;

                    ProcessOptions(name, key, value);
                }
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
    private void ParseModels(JObject? obj)
    {

        if (obj != null)
        {
            CObjectInfo info = new CObjectInfo();
            var cls = obj["class"];
            var name = obj["name"];
            var main_category = obj["maincategory"];

            if (name != null && main_category != null)
            {
                
                info.Name = name.Value<string>();
                info.MainCategory = main_category.Value<string>();
                mMainForm.RegisterModel(info);
            }
        }
            
        
    }

    private void ProcessOptions(JToken name, string key, JToken? value)
    {
        string option_value = "";
        if (name.Value<string>() == mMainForm.GetSelectedSensor())
        {
            string option_str = key;
            option_value = value.Value<string>();

            if (option_str != null)
            {
                if (option_str == "position")
                {
                    FVector vec = new FVector();
                    if (ParseVector(option_value, vec))
                    {
                        //CConverter.UEToWorld(vec);
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
                else
                {
                    if (OptionParamMap.TryGetValue(option_str, out string param_name))
                    {
                        mMainForm.SetOptionValue(param_name, option_value);
                    }

                }

            }
        }
    }

    private void ProcessOptions(JToken name, JToken? option, JToken? value)
    {
        string option_value = "";
        if (name.Value<string>() == mMainForm.GetSelectedSensor())
        {
            string option_str = option.Value<string>();
            option_value = value.Value<string>();

            if (option_str != null)
            {
                if (option_str == "position")
                {
                    FVector vec = new FVector();
                    if(ParseVector(option_value, vec))
                    {
                        //CConverter.UEToWorld(vec);
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
                else
                {
                    if(OptionParamMap.TryGetValue(option_str, out string param_name))
                    {
                        mMainForm.SetOptionValue(param_name, option_value);
                    }

                }

            }
        }
    }
}
