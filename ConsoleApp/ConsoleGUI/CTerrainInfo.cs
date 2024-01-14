using System.Xml.Serialization;

[XmlRoot("heightmapData")]
public class CTerrainInfo
{
    [XmlElement("heightmapName")]
    public string HeightmapName { get; set; }

    [XmlElement("satelliteName")]
    public string SatelliteName { get; set; }

    [XmlElement("longitude")]
    public double Longitude { get; set; }

    [XmlElement("latitude")]
    public double Latitude { get; set; }

    [XmlElement("minHeight")]
    public double MinHeight { get; set; }

    [XmlElement("maxHeight")]
    public double MaxHeight { get; set; }

    [XmlElement("waterContours")]
    public bool WaterContours { get; set; }

    [XmlElement("heightContours")]
    public bool HeightContours { get; set; }

    [XmlElement("zoom")]
    public double Zoom { get; set; }

    [XmlElement("topLeftLat")]
    public double TopLeftLat { get; set; }

    [XmlElement("topLeftLon")]
    public double TopLeftLon { get; set; }

    [XmlElement("bottomRightLat")]
    public double BottomRightLat { get; set; }

    [XmlElement("bottomRightLon")]
    public double BottomRightLon { get; set; }
}
