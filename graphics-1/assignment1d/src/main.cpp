#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "camera.h"
#include "color.h"
#include "fileIO.h"
#include "intersect.h"
#include "shape.h"
#include "light.h"
#include "ray.h"
#include "vec3.h"
#include "reflectionAndRefraction.h"

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

    lightSources Ls = toLightSources(collect); //Create struct with lights and light count
    
    //The final array, this array sets the color of the pixels that can be seen by the camera
    colorType* lightIntersection = new colorType[PHEIGHT*PWIDTH];
    for(int i = 0 ; i < PHEIGHT; i++) //Loop through both lightIntersection and cameraIntersection
    {
        for(int j = 0; j < PWIDTH; j++) //Pseudo 2d arrays, j loops through width while i loops through height
        {
            if(cameraIntersection[i*PWIDTH+j].shapeIndex != -1)
            {
                double alpha = shapeStore.SVector.at(cameraIntersection[i*PWIDTH+j].shapeIndex)->L.alpha;
                double eta = shapeStore.SVector.at(cameraIntersection[i*PWIDTH+j].shapeIndex)->L.eta;
                vec3 N = shapeStore.SVector.at(cameraIntersection[i*PWIDTH+j].shapeIndex)->surfaceNormal(cameraIntersection[i*PWIDTH+j].coordinates);
                stack* shapeStack = new stack;
                lightIntersection[i*PWIDTH+j] = colorCalc(  0,                               //Initial depth
                                                            N,                               //Surface normal of initial shape (N is normalized)
                                                            shapeStack,                      //Stack for checking if a shape is being entered or left
                                                            pow(((eta-1)/(eta+1)), 2),       //F0  
                                                            alpha,
                                                            1,                               //Current eta is presumed to be air, so starts at 1
                                                            bkg,
                                                            shapeStore,                      
                                                            cameraIntersection[i*PWIDTH+j],  //Initial intersection point
                                                            Ls, 
                                                            viewing.eye,                     //Initial origin, so the camera
                                                            rays[i*PWIDTH+j],                //Initial ray from the camera
                                                            1,
                                                            1);               
                delete shapeStack;
            }
            else
            {
                lightIntersection[i*PWIDTH+j] = bkg;
            }
        }
    }

    delete[] rays;
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