public class CUtil
{
    public static string BoolToBinaryString(bool value)
    {
        return value ? "1" : "0";
    }

    internal static string MakeVectorString(double x, double y, double z)
    {
        return "\""+x.ToString() +" "+ y.ToString() + " "+z.ToString() + "\"" ;
    }

    internal static string MakeVectorString(FVector vec)
    {
        return "\"" + vec.X.ToString() + " " + vec.Y.ToString() + " " + vec.Z.ToString() + "\"";
    }
}