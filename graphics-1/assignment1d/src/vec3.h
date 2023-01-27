#ifndef VEC3_H
#define VEC3_H

#include <iostream>
#include <cmath>

struct vec3
{
    double x;
    double y;
    double z;

    vec3();
    vec3(double a, double b, double c);
    vec3 operator+(const vec3& b);
    vec3 operator-(const vec3& b);
    vec3 operator*(const double& b);
    bool operator==(const vec3& b);

    void print();
};

vec3 cross(vec3 fst, vec3 snd);
double norm(vec3 vec);
vec3 normalize(vec3 vec);
double dot(vec3 fst, vec3 snd);
vec3 halfway(vec3 L, vec3 V);

#endif