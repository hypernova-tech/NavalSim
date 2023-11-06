public class FVector
{
    public double X;
    public double Y;
    public double Z;

    public override string ToString()
    {
        return "\""+X.ToString() + " " + Y.ToString() + " " + Z.ToString()+"\"";
    }
}
