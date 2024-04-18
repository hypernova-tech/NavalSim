#include "CMath.h"
#include "math.h"

bool CMath::IsZero(double val, double tolerance)
{
    return fabs(val)<= tolerance;
}
