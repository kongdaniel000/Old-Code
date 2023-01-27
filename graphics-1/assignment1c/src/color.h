#ifndef COLOR_H
#define COLOR_H

struct colorType
{
    double r;
    double g;
    double b;

    colorType();
    colorType(double c1, double c2, double c3);
    colorType operator+(const colorType& a);
    colorType operator-(const colorType& a);
    colorType operator*(const double& a);

    void print();
};

#endif