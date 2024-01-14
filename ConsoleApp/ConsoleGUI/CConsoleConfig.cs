using System.Xml.Serialization;

[XmlRoot("consoleconfig")]
public class CConsoleConfig
{
    [XmlElement("rootfolder")]
    public string RootFolder { get; set; }

    
}
