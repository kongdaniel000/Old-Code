#include "vec3.h"

#include <iostream>
#include <cmath>

vec3::vec3()
{
    this->x = 0.0;
    this->y = 0.0;
    this->z = 0.0;
}

vec3::vec3(double a, double b, double c)
{
    this->x = a;
    this->y = b;
    this->z = c;
}

vec3 vec3::operator+(const vec3& b)
{
    vec3 vec;
    vec.x = this->x + b.x;
    vec.y = this->y + b.y;
    vec.z = this->z + b.z;
    return vec; 
}

vec3 vec3::operator-(const vec3& b)
{
    vec3 vec;
    vec.x = this->x - b.x;
    vec.y = this->y - b.y;
    vec.z = this->z - b.z;
    return vec; 
}

vec3 vec3::operator*(const double& b)
{
    vec3 vec;
    vec.x = this->x * b;
    vec.y = this->y * b;
    vec.z = this->z * b;
    return vec;
}

bool vec3::operator==(const vec3& b) 
{
    bool x = this->x == b.x;
    bool y = this->y == b.y;
    bool z = this->z == b.z;
    return (x && y && z);
}

void vec3::print()
{
    std::cout << "(" << x << ", " << y << ", " << z << ")\n";
}

//Returns cross-product of two vectors
vec3 cross(vec3 fst, vec3 snd)
{
    vec3 crossProduct;
    crossProduct.x = (fst.y*snd.z) - (fst.z*snd.y);
    crossProduct.y = -((fst.x*snd.z) - (fst.z*snd.x));
    crossProduct.z = (fst.x*snd.y) - (fst.y*snd.x);
    return crossProduct;
}

//Gets the norm of a vector
double norm(vec3 vec)
{
    return sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

//Normalizes a vector
vec3 normalize(vec3 vec)
{
    return vec * (1 / norm(vec));
}

//Returns dot-product of two vectors
double dot(vec3 fst, vec3 snd)
{
    return fst.x * snd.x + fst.y * snd.y + fst.z * snd.z;
}

//Returns a normalized halfway vector
vec3 halfway(vec3 L, vec3 V)
{
    return normalize((L+V)*0.5);
}