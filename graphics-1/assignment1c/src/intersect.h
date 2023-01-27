#ifndef INTERSECT_H
#define INTERSECT_H

#include "vec3.h"

struct intersectionPoint
{
    bool isIntersect = false;
    vec3 coordinates;
    int shapeIndex = -1;
    double time = -1.0;
};

double solveDiscriminant(double A, double B, double C);
double plusQuadratic(double A, double B, double C);
double minusQuadratic(double A, double B, double C);

#endif