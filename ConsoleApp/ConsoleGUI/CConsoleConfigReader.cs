using System.Xml.Serialization;

public class CConsoleConfigReader
{
    public CConsoleConfig ReadXml(string filePath)
    {
        XmlSerializer serializer = new XmlSerializer(typeof(CConsoleConfig));
        using (FileStream fileStream = new FileStream(filePath, FileMode.Open))
        {
            return (CConsoleConfig)serializer.Deserialize(fileStream);
        }
    }
}