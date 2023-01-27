#ifndef CAMERA_H
#define CAMERA_H

#include "vec3.h"

struct pixelResolution
{
    int PWIDTH;
    int PHEIGHT;
    double ASPECT_RATIO;

    pixelResolution();
    pixelResolution(int w, int h);
};

struct cameraType
{
    cameraType();

    vec3 eye;
    vec3 viewdir;
    vec3 updir;
    double vfov;
    pixelResolution pix;
    vec3 w;
    vec3 u;
    vec3 v;
    double d;
    double height;
    double width;
    vec3 ul;
    vec3 ur;
    vec3 ll;
    vec3 lr;

    void cameraSetup();
};

#endif