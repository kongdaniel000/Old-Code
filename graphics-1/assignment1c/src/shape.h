#ifndef SHAPE_H
#define SHAPE_H

#include "intersect.h"
#include "ray.h"
#include "light.h"
#include "vec3.h"


class shapeType
{
public:
    lightType L;
    int THEIGHT;
    int TWIDTH;
    colorType* texture;

    virtual vec3 surfaceNormal(vec3 coordinate) = 0;
    virtual intersectionPoint rayTrace(rayType ry, int index) = 0;
    virtual colorType findODLambda(vec3 coordinates) = 0;
    virtual void print() = 0;
};

struct textureCoordinate
{
    double u;
    double v;

    textureCoordinate();
    textureCoordinate(double u, double v);
    textureCoordinate operator+(textureCoordinate a);
    textureCoordinate operator*(double a);

    void print();
};

struct shapes
{
    ~shapes() 
    {
        SVector.clear(); 
        textureVec.clear();
    }
    shapes() { textureVec.push_back(NULL); }
    
    std::vector<shapeType*> SVector; //Stores shapes
    std::vector<vec3> triangleVectors;
    std::vector<textureCoordinate> triangleTextureCoordinates;
    std::vector<vec3> triangleNormalVectors;
    std::vector<colorType*> textureVec; //Needs this to delete memory taken by texture arrays
};

class sphereType : public shapeType
{
public:
    vec3 center;
    double radius = 1.0;

    vec3 surfaceNormal(vec3 coordinate);
    intersectionPoint rayTrace(rayType ry, int index);
    colorType findODLambda(vec3 coordinates);
    void print();
};

// Doesn't work for now
// class cylinderType : public shapeType
// {
// public:
//     vec3 center;
//     vec3 dir;
//     double length = 1.0;
//     double radius = 1.0;

//     vec3 surfaceNormal(vec3 coordinate);
//     intersectionPoint rayTrace(rayType ry, int index);
// };

class triangleType : public shapeType
{
public:
    bool isSmooth = false;
    vec3 v[3];
    textureCoordinate vt[3];
    vec3 vn[3];
    vec3 e[2]; //e[0] = e1, e[1] = e2
    vec3 N;
    double d11;
    double d12;
    double d22;
    double D;

    void setTriangle();
    vec3 surfaceNormal(vec3 coordinate);
    intersectionPoint rayTrace(rayType ry, int index);
    colorType findODLambda(vec3 coordinates);
    void print();
};

#endif