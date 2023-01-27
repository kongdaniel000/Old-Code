#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "camera.h"
#include "color.h"
#include "fileIO.h"
#include "intersect.h"
#include "light.h"
#include "ray.h"
#include "shape.h"
#include "vec3.h"

int main(int argc, char* argv[])
{
    //Vector to collect file input
    std::vector<std::string> collect;

    //Declare necessary variables
    cameraType viewing;
    colorType bkg;

    //Open given input file
    std::string targetFile;
    if(argv[1] != NULL)
    {
        targetFile += argv[1];
        std::ifstream file;
        file.open(targetFile);

        if (file.fail()) //If input file cannot be read, stop program telling the user that the file failed to open
        {
            std::cout << "Input file failed to open, ending program\n";
            return 2;
        }
        else //Read input
        {
            std::string s; 
            while (file >> s) //Read word-by-word
            {
                collect.push_back(s); //Store words to collect vector
            }
        }
        file.close();
    }
    else //If input file cannot be found, stop program telling the user that the file failed to open
    {
        std::cout << "No input file found, normal usage is: ./raytracer <input file> <output name>\n";
        return 1;
    }

    readInput(&viewing, &bkg, collect); //Read input to necessary variables

    if(viewing.viewdir == viewing.updir) //Check that view direction and up direction are not the same
    {
        std::cout << "View direction cannot be equal to up direction!\n";
        exit(1);
    }

    //Setup variables for viewing window
    viewing.cameraSetup(); 
    const int PWIDTH = viewing.pix.PWIDTH;
    const int PHEIGHT = viewing.pix.PHEIGHT;

    //Calculate variables needed to position pixels
    vec3 deltaW = (viewing.ur - viewing.ul) * (1 / static_cast<double>(PWIDTH));
    vec3 deltaH = (viewing.ll - viewing.ul) * (1 / static_cast<double>(PHEIGHT));
    vec3 deltaCW = deltaW * 0.5;
    vec3 deltaCH = deltaH * 0.5;

    //Create an array representing the coordinates of the pixels
    vec3* pixel = new vec3[PHEIGHT*PWIDTH];
    for(int i = 0 ; i < PHEIGHT; i++)
    {
        for(int j = 0; j < PWIDTH; j++)
        {
            pixel[i*PWIDTH+j] = viewing.ul + (deltaW * j) + (deltaH * i) + deltaCW + deltaCH;
        }
    }

    //Create an array storing the direction and origins of rays
    rayType* rays = new rayType[PHEIGHT*PWIDTH];
    for(int i = 0 ; i < PHEIGHT; i++)
    {
        for(int j = 0; j < PWIDTH; j++)
        {
            rays[i*PWIDTH+j].origin = viewing.eye;
            rays[i*PWIDTH+j].direction = normalize(pixel[i*PWIDTH+j] - viewing.eye);
        }
    }

    delete[] pixel;

    shapes shapeStore = toShape(collect); //Setup shapes

    //Sets the pixels which can be seen by the eye and flags them for light checking
    intersectionPoint* cameraIntersection = new intersectionPoint[PHEIGHT*PWIDTH];
    for(int i = 0 ; i < PHEIGHT; i++)
    {
        for(int j = 0; j < PWIDTH; j++)
        {
            bool isFirst = true; //Flag for if this is the first time an intersection is found at this pixel
            for(int k = 0; k < shapeStore.SVector.size(); k++)
            {
                intersectionPoint point = shapeStore.SVector.at(k)->rayTrace(rays[i*PWIDTH+j], k);
                double inter = point.time;
                //inter is only returned as -1.0 if the discriminant is negative, which implies the ray misses the shape
                //or if the ray intersects the shape behind the camera, at which point it doesn't matter
                
                //This checks to make sure that the color of the pixel is the shape closest to the eye
                if((isFirst || inter < cameraIntersection[i*PWIDTH+j].time) && inter > 0)
                {
                    cameraIntersection[i*PWIDTH+j].isIntersect = true;
                    cameraIntersection[i*PWIDTH+j].coordinates = point.coordinates;
                    cameraIntersection[i*PWIDTH+j].time = inter;
                    cameraIntersection[i*PWIDTH+j].shapeIndex = k;
                    isFirst = false;
                }
            }
        }
    }

    delete[] rays;

    lightSources Ls = toLightSources(collect); //Create struct with lights and light count

    //The final array, this array sets the color of the pixels that can be seen by the camera
    colorType* lightIntersection = new colorType[PHEIGHT*PWIDTH];
    for(int i = 0 ; i < PHEIGHT; i++) //Loop through both lightIntersection and cameraIntersection
    {
        for(int j = 0; j < PWIDTH; j++) //Pseudo 2d arrays, j loops through width while i loops through height
        {
            lightIntersection[i*PWIDTH+j] = bkg; //Set background color first
            intersectionPoint currentPoint = cameraIntersection[i*PWIDTH+j]; //The current pixel being looked at
            if(currentPoint.isIntersect) //Checks if the pixel is seen by camera
            {
                lightType currentColor = shapeStore.SVector.at(currentPoint.shapeIndex)->L; //Colors taken from the shape found at this pixel
                lightIntersection[i*PWIDTH+j] = shapeStore.SVector.at(currentPoint.shapeIndex)->findODLambda(currentPoint.coordinates) * currentColor.weight.ka; //Set the ambient
                rayType VRay = pointsToRay(currentPoint.coordinates, viewing.eye); //Find direction from point to eye and set V equal to it
                vec3 N = shapeStore.SVector.at(currentPoint.shapeIndex)->surfaceNormal(currentPoint.coordinates); //Find surface normal of current shape (N is normalized)
                vec3 V = normalize(VRay.direction); //This V is the vector that will be used in calculations
                vec3 L; //Declare L for later use
                rayType LRay; //Declare LRay for later use
                
                for(int k = 0; k < Ls.numberOfLights; k++) //Loops through each light source
                {
                    lightSourceType currentLightSource = Ls.lightVector.at(k); //The light source we are currently looking at
                    int S = 1; //Shadow flag, default to no shadow

                    if(currentLightSource.w == 1) //if lightsource is a point
                    {
                        LRay = pointsToRay(currentPoint.coordinates, currentLightSource.vec); //Ray from surface to eye location
                        L = normalize(LRay.direction);
                    }
                    else //if NOT a point -> if direcitonal
                    {
                        vec3 defaultVec;
                        if(currentLightSource.L == defaultVec) //If L is not different from the default, then change L
                        {
                            currentLightSource.L = normalize(currentLightSource.vec * -1); //if so, calculate L once
                        }
                        L = currentLightSource.L;
                        LRay.direction = L;
                        LRay.origin = currentPoint.coordinates;
                    }

                    for(int n = 0; n < shapeStore.SVector.size(); n++) //Loops through each shape
                    {
                        intersectionPoint LInter = shapeStore.SVector.at(n)->rayTrace(LRay, currentPoint.shapeIndex); //Checks if LRay intersects shape at index n

                        //This is a ray opposite from every manner, starts where LRay stops, stops where LRay starts.
                        //This is used to error check shadows, to make sure that the program only generates shadows
                        //if there is an object in the way going both ways.
                        rayType nLRay = LRay;
                        nLRay.direction = nLRay.direction * -1;
                        nLRay.origin = currentLightSource.vec;
                        intersectionPoint nLInter = shapeStore.SVector.at(n)->rayTrace(nLRay, currentPoint.shapeIndex);

                        //LInter will return positive if it intersects a shape from surface to light
                        //nLInter will return positive if it intersects a shape from light to surface
                        //Also makes sure that it is not returning a false positive by intersecting itself
                        if(LInter.time > 0 && nLInter.time > 0 && (n != currentPoint.shapeIndex))
                        {
                            S = 0; //If LRay intersects an object between light source and surface, then something is in front and casts a shadow
                        }
                    }

                    L = normalize(LRay.direction); //L vector is just direction of LRay normalized
                    vec3 H = normalize((L+V)*0.5); //Find halfway vector
                    double colorDot1 = dot(N, L); //Find N.L (N dot L)
                    double colorDot2 = dot(N, H); //Find N.L (N dot H)

                    //Clamps values so that they cannot be negative
                    if(colorDot1 < 0.0) 
                    {
                        colorDot1 = 0.0;
                    }
                    if(colorDot2 < 0.0) 
                    {
                        colorDot2 = 0.0;
                    }

                    //Set diffuse and specular terms
                    colorType diffuse = (shapeStore.SVector.at(currentPoint.shapeIndex)->findODLambda(currentPoint.coordinates) * currentColor.weight.kd) * colorDot1;
                    colorType specular = (currentColor.specularH * currentColor.weight.ks) * 
                                         pow(colorDot2, currentColor.exponent);

                    //Finally, add the diffuse and specular terms multiplied by shadow flag
                    lightIntersection[i*PWIDTH+j] = lightIntersection[i*PWIDTH+j] + ((diffuse + specular) * S);
                }
                //Clamps light at pixel so that it cannot exceed max value
                if(lightIntersection[i*PWIDTH+j].r > 1.0)
                {
                    lightIntersection[i*PWIDTH+j].r = 1.0;
                }
                if(lightIntersection[i*PWIDTH+j].g > 1.0)
                {
                    lightIntersection[i*PWIDTH+j].g = 1.0;
                }
                if(lightIntersection[i*PWIDTH+j].b > 1.0)
                {
                    lightIntersection[i*PWIDTH+j].b = 1.0;
                }
            }
        }
    }
    

    delete[] cameraIntersection;
    
    //Output filename
    std::string ofile;
    if(argv[2] != NULL)
    {
        ofile = argv[2];
    }
    else
    {
        std::cout << "No output name found! Outputting to output.ppm\n";
        ofile = "output.ppm";
    }
    std::ofstream output_stream(ofile, std::ios::out);

    //Header
    output_stream << "P3\n"
                  << PWIDTH << "\n"
                  << PHEIGHT << "\n"
                  << 255 << "\n";

    //Output pixel colors
    for(int i = 0; i < PHEIGHT; i++)
    {
        for(int j = 0; j < PWIDTH; j++)
        {
            output_stream << static_cast<int>(lightIntersection[i*PWIDTH+j].r*255) << " " 
                          << static_cast<int>(lightIntersection[i*PWIDTH+j].g*255) << " " 
                          << static_cast<int>(lightIntersection[i*PWIDTH+j].b*255) << "\n";
        }
    }

    output_stream.close();

    delete[] lightIntersection;

    std::cout << "File output to: " << ofile << std::endl;

    return 0;
}