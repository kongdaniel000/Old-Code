#include "intersect.h"

#include <cmath>

double solveDiscriminant(double A, double B, double C)
{
    return (B*B) - (4*A*C);
}

double plusQuadratic(double A, double B, double C)
{
    return (-B + sqrt((B*B) - (4*A*C))) / (2*A);
}

double minusQuadratic(double A, double B, double C)
{
    return (-B - sqrt((B*B) - (4*A*C))) / (2*A);
}
