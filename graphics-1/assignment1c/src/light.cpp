#include "light.h"

#include <iostream>

void lightWeightType::print()
{
    std::cout << ka << " "<< kd << " " << ks << "\n";
}

void lightType::print()
{
    intrinsic.print();
    specularH.print();
    weight.print();
    std::cout << exponent << "\n";
}