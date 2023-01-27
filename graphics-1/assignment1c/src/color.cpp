#include "color.h"

#include <iostream>

colorType::colorType()
{
    this->r = 0.0;
    this->g = 0.0;
    this->b = 0.0;
}

colorType::colorType(double c1, double c2, double c3)
{
    this->r = c1;
    this->g = c2;
    this->b = c3;
}

colorType colorType::operator+(const colorType& a)
{
    colorType col;
    col.r = this->r + a.r;
    col.g = this->g + a.g;
    col.b = this->b + a.b;
    return col; 
}

colorType colorType::operator-(const colorType& a)
{
    colorType col;
    col.r = this->r - a.r;
    col.g = this->g - a.g;
    col.b = this->b - a.b;
    return col; 
}

colorType colorType::operator*(const double& a)
{
    colorType col;
    col.r = this->r * a;
    col.g = this->g * a;
    col.b = this->b * a;
    return col; 
}

void colorType::print()
{
    std::cout << "(" << r << ", " << g << ", " << b << ")\n";
}