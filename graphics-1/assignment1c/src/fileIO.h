#ifndef FILEIO_H
#define FILEIO_H

#include "camera.h"
#include "color.h"
#include "shape.h"
#include "light.h"

void readInput(cameraType *cam, colorType *bkgcolor, std::vector<std::string> collect);
shapes toShape(std::vector<std::string> collect);
lightSources toLightSources(std::vector<std::string> collect);

#endif