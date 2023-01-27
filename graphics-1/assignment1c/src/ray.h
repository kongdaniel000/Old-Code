#ifndef RAY_H
#define RAY_H

#include "vec3.h"

struct rayType
{
    vec3 origin;
    vec3 direction;
};

rayType pointsToRay(vec3 start, vec3 stop);

#endif