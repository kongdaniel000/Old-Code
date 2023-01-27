#include "camera.h"
#include "vec3.h"
pixelResolution::pixelResolution()
{
    PWIDTH = 400;
    PHEIGHT = 400;
    ASPECT_RATIO = 1.0;
}

pixelResolution::pixelResolution(int w, int h)
{
    PWIDTH = w;
    PHEIGHT = h;
    ASPECT_RATIO = static_cast<double>(PWIDTH) / static_cast<double>(PHEIGHT);
}

cameraType::cameraType()
{
    eye = vec3(0.0, 0.0, 0.0);
    viewdir = vec3(0.0, 0.0, 1.0);
    updir = vec3(0.0, 1.0, 0.0);
    vfov = 90.0;
}

//Sets all the values in camera
void cameraType::cameraSetup()
{
    w = normalize(viewdir*-1.0);
    u = normalize(cross(viewdir, updir));
    v = normalize(cross(u, viewdir));
    //d is arbitrarily selected as 1
    d = 1.0;
    height = 2 * d * tan((vfov / 180 * M_PI)/2);
    width = height * pix.ASPECT_RATIO;
    vec3 newW = viewdir * d;
    vec3 newU = u * (width / 2);
    vec3 newV = v * (height / 2);
    ul = eye + newW - newU + newV;
    ur = eye + newW + newU + newV;
    ll = eye + newW - newU - newV;
    lr = eye + newW + newU - newV;
}