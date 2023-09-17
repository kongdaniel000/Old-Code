
/**
* @brief File name: ElectionEntity.cc. Description: Contains method definitons for the ElectionObject class,
* Author: Jacob.
*
*/
#include "ElectionEntity.h"
#include <iostream>

std::string ElectionEntity::getName() {
    return name;
}

void ElectionEntity::setName(std::string newName) {
    name = newName;
}