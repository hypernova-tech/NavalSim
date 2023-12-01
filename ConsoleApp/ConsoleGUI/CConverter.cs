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
