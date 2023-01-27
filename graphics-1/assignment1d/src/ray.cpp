#include "ray.h"

rayType pointsToRay(vec3 start, vec3 stop)
{
    rayType newRay;
    newRay.origin = start;
    newRay.direction = stop - start;

    return newRay;
}