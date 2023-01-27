#include "shape.h"

#include <cmath>
#include <iostream>

shapeType::~shapeType()
{
    delete texture;
}

textureCoordinate::textureCoordinate()
{
    this->u = 1;
    this->v = 1;
}

textureCoordinate::textureCoordinate(double u, double v)
{
    this->u = u;
    this->v = v;
}

textureCoordinate textureCoordinate::operator+(textureCoordinate a)
{
    textureCoordinate coord;
    coord.u = this->u + a.u;
    coord.v = this->v + a.v;
    return coord;
}

textureCoordinate textureCoordinate::operator*(double a)
{
    textureCoordinate coord;
    coord.u = this->u * a;
    coord.v = this->v * a;
    return coord;
}

void textureCoordinate::print()
{
    std::cout << "(" << u << ", " << v << ")\n";
}

vec3 sphereType::surfaceNormal(vec3 coordinate)
{
    return normalize(coordinate - center);
}

intersectionPoint sphereType::rayTrace(rayType ry, int index)
{
    intersectionPoint p;
    double A = pow(ry.direction.x, 2)+pow(ry.direction.y, 2)+pow(ry.direction.z, 2);
    double B = 2*((ry.direction.x*(ry.origin.x-center.x))+(ry.direction.y*(ry.origin.y-center.y))+(ry.direction.z*(ry.origin.z-center.z)));
    double C = pow((ry.origin.x-center.x), 2)+pow((ry.origin.y-center.y), 2)+pow((ry.origin.z-center.z), 2)-pow(radius, 2);

    double disc = solveDiscriminant(A, B, C);
    if(disc > 0)
    {
        double minus = minusQuadratic(A, B, C);
        double plus = plusQuadratic(A, B, C);

        if(minus < plus && minus > 0)
        {
            p.time = minus;
        }
        else if (plus > 0)
        {
            p.time = plus;
        }
        p.coordinates = ry.origin + (ry.direction * p.time);
        p.shapeIndex = index;
    }
    return p;
}

colorType sphereType::findODLambda(vec3 coordinates)
{
    if(texture != NULL)
    {   
        vec3 N = surfaceNormal(coordinates);
        double theta = atan2(N.x, N.z);
        double phi = acos(N.y);
        textureCoordinate p(((theta + M_PI) / (2 * M_PI)), (phi / M_PI));
        double ufractional = p.u - static_cast<int>(p.u);
        double vfractional = p.v - static_cast<int>(p.v);
        double x = ufractional * (TWIDTH-1);
        double y = vfractional * (THEIGHT-1);
        int i = y;
        int j = x;
        double m = x - j;
        double n = y - i;

        return  texture[i*TWIDTH+j] * (1-m) * (1-n) +
                texture[(i+1)*TWIDTH+j] * (m) * (1-n) +
                texture[i*TWIDTH+(j+1)] * (1-m) * (n) +
                texture[(i+1)*TWIDTH+(j+1)] * (m) * (n);
    }
    return L.intrinsic;
}

void sphereType::print()
{
    center.print();
    std::cout << radius << "\n";
}

//General cylinder functions that don't work as of now, may implement later
// vec3 cylinderType::surfaceNormal(vec3 coordinate)
// {
//     vec3 v = normalize(dir);
//     vec3 p1 = center - (dir * 0.5 * length);
//     vec3 p2 = center + (dir * 0.5 * length);

//     if((coordinate == p1) || (dot((p1-center), (coordinate-p1)) == 0))
//     {
//         return (p1-center);
//     }
//     else if((coordinate == p2) || (dot((p2-center), (coordinate-p2)) == 0))
//     {
//         return (p2-center);
//     }
//     else
//     {
//         return ((coordinate-center) - (dir)));
//     }
// }
    

// intersectionPoint cylinderType::rayTrace(rayType ry, int index)
// {
//     intersectionPoint p;

//     vec3 v = normalize(dir);
//     vec3 p1 = center - (dir * 0.5 * length);
//     vec3 p2 = center + (dir * 0.5 * length);
//     vec delP = ry.origin-p1;

//     double A = pow(norm(ry.direction - (v * dot(ry.direction, v)), 2));
//     double B = 2 * dot(ry.direction - (v * dot(ry.direction, v)), (delP - (v * dot(delP, v))));
//     double C = pow(norm((delP - (v * dot(delP, v)))), 2) - pow(r, 2);
//     double disc = solveDiscriminant(A, B, C);

//     if(disc > 0)
//     {
//         double minus = minusQuadratic(A, B, C);
//         double plus = plusQuadratic(A, B, C);

//         if(minus < plus && minus > 0)
//         {
//             p.time = minus;
//         }
//         else if (plus > 0)
//         {
//             p.time = plus;
//         }
//         p.coordinates.x = ry.origin.x + (ry.direction.x * p.time);
//         p.coordinates.y = ry.origin.y + (ry.direction.y * p.time);
//         p.coordinates.z = ry.origin.z + (ry.direction.z * p.time);
//         p.shapeIndex = index;
//     }
//     return p;
// }
void triangleType::setTriangle()
{
    e[0] = v[1] - v[0];
    e[1] = v[2] - v[0];

    N = cross(e[0], e[1]);

    d11 = dot(e[0], e[0]);
    d12 = dot(e[0], e[1]);
    d22 = dot(e[1], e[1]);
    D = d11 * d22 - d12 * d12;
}

vec3 triangleType::surfaceNormal(vec3 coordinate)
{
    if(isSmooth)
    {
        vec3 ep = coordinate - v[0];
        double d1p = dot(e[0], ep);
        double d2p = dot(e[1], ep);

        double Dbeta = d22 * d1p - d12 * d2p;
        double Dgamma = d11 * d2p - d12 * d1p;

        double beta = Dbeta / D;
        double gamma = Dgamma / D;
        double alpha = 1 - beta - gamma;

        return normalize(vn[0] * alpha + vn[1] * beta + vn[2] * gamma);
    }
    else
    {
        return normalize(N);
    }
}

intersectionPoint triangleType::rayTrace(rayType ry, int index)
{
    intersectionPoint p;

    double A = N.x;
    double B = N.y;
    double C = N.z;
    double d = -(A * v[0].x + B * v[0].y + C * v[0].z); //Plane equation d, as opposed to capital D which is used to calculate beta and gamma

    double t = -1.0;
    double denom = ((A * ry.direction.x) + (B * ry.direction.y) + (C * ry.direction.z));
    if(denom != 0)
    {
        t = -(A * ry.origin.x + B * ry.origin.y + C * ry.origin.z + d) / denom;
    }

    if(t > 0)
    {
        vec3 ep = (ry.origin + (ry.direction * t)) - v[0];
        double d1p = dot(e[0], ep);
        double d2p = dot(e[1], ep);

        double Dbeta = d22 * d1p - d12 * d2p;
        double Dgamma = d11 * d2p - d12 * d1p;

        double beta = Dbeta / D;
        double gamma = Dgamma / D;
        double alpha = 1 - beta - gamma;

        if(((0 <= alpha) && (alpha <= 1)) && ((0 <= beta) && (beta <= 1)) && ((0 <= gamma) && (gamma <= 1)))
        {
            p.time = t;
            p.coordinates = ry.origin + (ry.direction * p.time);
            p.shapeIndex = index;
        }
    }
    return p;
}

colorType triangleType::findODLambda(vec3 coordinates)
{
    if(texture != NULL)
    {   
        vec3 ep = coordinates - v[0];
        double d1p = dot(e[0], ep);
        double d2p = dot(e[1], ep);

        double Dbeta = d22 * d1p - d12 * d2p;
        double Dgamma = d11 * d2p - d12 * d1p;

        double beta = Dbeta / D;
        double gamma = Dgamma / D;
        double alpha = 1 - beta - gamma;

        textureCoordinate p = vt[0] * alpha + vt[1] * beta + vt[2] * gamma;
        double ufractional = p.u - static_cast<int>(p.u);
        double vfractional = p.v - static_cast<int>(p.v);
        double x = ufractional * (TWIDTH-1);
        double y = vfractional * (THEIGHT-1);
        int i = y;
        int j = x;
        double m = x - j;
        double n = y - i;

        return  texture[i*TWIDTH+j] * (1-m) * (1-n) +
                texture[(i+1)*TWIDTH+j] * (m) * (1-n) +
                texture[i*TWIDTH+(j+1)] * (1-m) * (n) +
                texture[(i+1)*TWIDTH+(j+1)] * (m) * (n);
    }
    return L.intrinsic;
}

void triangleType::print()
{
    for(int i = 0; i < 3; i++)
    {
        v[i].print();
    }
    std::cout << "\n" << isSmooth << "\n\n";
    for(int i = 0; i < 3; i++)
    {
        vt[i].print();
    }
    std::cout << "\n";
    for(int i = 0; i < 3; i++)
    {
        vn[i].print();
    }
    std::cout << "\n";
    for(int i = 0; i < 2; i++)
    {
        e[i].print();
    }
    N.print();
    std::cout << "\n" << d11 << "\n\n"  << d12 << "\n\n"  << d22 << "\n\n" << D << "\n\n";
    L.print();
}