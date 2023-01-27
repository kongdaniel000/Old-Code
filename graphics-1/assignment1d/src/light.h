#ifndef LIGHT_H
#define LIGHT_H

#include <vector>

#include "color.h"
#include "vec3.h"

struct lightWeightType
{
    double ka;
    double kd;
    double ks;
    
    void print();
};

struct lightType
{
    colorType intrinsic;
    colorType specularH;
    lightWeightType weight;
    double exponent = 1;
    double alpha = 1;
    double eta = 0;
    
    void print();
};

struct lightSourceType
{
    vec3 vec;
    int w = 0;
    colorType color;
    vec3 L;
};

struct lightSources
{
    int numberOfLights = 0;
    std::vector<lightSourceType> lightVector;
};

#endif