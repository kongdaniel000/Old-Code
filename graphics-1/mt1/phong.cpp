#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>

struct vec3
{
    double x;
    double y;
    double z;

    vec3()
    {
        this->x = 0.0;
        this->y = 0.0;
        this->z = 0.0;
    }

    vec3(double a, double b, double c)
    {
        this->x = a;
        this->y = b;
        this->z = c;
    }

    vec3 operator+(const vec3& b)
    {
        vec3 vec;
        vec.x = this->x + b.x;
        vec.y = this->y + b.y;
        vec.z = this->z + b.z;
        return vec; 
    }

    vec3 operator-(const vec3& b)
    {
        vec3 vec;
        vec.x = this->x - b.x;
        vec.y = this->y - b.y;
        vec.z = this->z - b.z;
        return vec; 
    }

    vec3 operator*(const double& b)
    {
        vec3 vec;
        vec.x = this->x * b;
        vec.y = this->y * b;
        vec.z = this->z * b;
        return vec;
    }

    bool operator==(const vec3& b) 
    {
        bool x = this->x == b.x;
        bool y = this->y == b.y;
        bool z = this->z == b.z;
        return (x && y && z);
    }
};

vec3 normalize(vec3 vec)
{
    vec3 normalizedVector;
    double norm = sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
    normalizedVector.x = vec.x / norm;
    normalizedVector.y = vec.y / norm;
    normalizedVector.z = vec.z / norm;
    return normalizedVector;
}

void printVec(vec3 vec)
{
    std::cout << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")\n";
}

double dot(vec3 fst, vec3 snd)
{
    return fst.x * snd.x + fst.y * snd.y + fst.z * snd.z;
}

int main()
{
    vec3 eye(0,0,10);
    vec3 spherec(0,0,2);
    vec3 intersectpoint(3,0,6);
    vec3 lightlocation(5,2,7);
    vec3 L_ = lightlocation-intersectpoint;
    vec3 L = normalize(L_);
    printVec(L_);
    printVec(L);
    std::cout << "\n";
    vec3 N_ = intersectpoint - spherec;
    vec3 N = normalize(N_);
    printVec(N_);
    printVec(N);
    std::cout << "\n";
    vec3 V_ = eye - intersectpoint;
    vec3 V = normalize(V_);
    printVec(V_);
    printVec(V);
    std::cout << "\n";
    vec3 H_ = (L + V) * 0.5;
    vec3 H = normalize(H_);
    printVec(H_);
    printVec(H);
    std::cout << "\n";

    double ka = 0.2;
    double kd = 0.4;
    double ks = 0.2;
    double n = 20;
    vec3 Od(0.5, 1.0, 0.0);
    vec3 Os(1.0, 0.0, 0.0);

    double difvec = dot(N, L);
    if(difvec < 0)
    {
        difvec = 0;
    }
    std::cout << difvec << "\n";

    double specvec = dot(N, H);
    if(specvec < 0)
    {
        specvec = 0;
    }

    std::cout << specvec << "\n";

    specvec = pow(specvec, n);

    std::cout << specvec << "\n\n";

    vec3 ambient = Od * ka;
    vec3 diffuse = Od * kd;
    vec3 specular = Os * ks;
    printVec(ambient);
    printVec(diffuse);
    printVec(specular);
    std::cout << "\n";

    diffuse = diffuse * difvec;
    specular = specular * specvec;
    printVec(diffuse);
    printVec(specular);
    std::cout << "\n";

    vec3 I = ambient + diffuse + specular;
    printVec(I);
    std::cout << "\n";

    I = I * 255;
    int holdx = I.x;
    int holdy = I.y;
    int holdz = I.z;
    I.x = holdx;
    I.y = holdy;
    I.z = holdz;
    printVec(I);
    std::cout << "\n";

    return 0;
}