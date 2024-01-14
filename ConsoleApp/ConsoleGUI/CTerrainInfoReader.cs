using System.Xml.Serialization;

public class CTerrainInfoReader
{
    public CTerrainInfo ReadXml(string filePath)
    {
        XmlSerializer serializer = new XmlSerializer(typeof(CTerrainInfo));
        using (FileStream fileStream = new FileStream(filePath, FileMode.Open))
        {
            return (CTerrainInfo)serializer.Deserialize(fileStream);
        }
    }
}
