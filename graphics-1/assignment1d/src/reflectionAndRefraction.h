#ifndef REFLECTIONANDREFRACTION_H
#define REFLECTIONANDREFRACTION_H

#include "shape.h"
#include "light.h"

struct stack 
{
    ~stack();
    struct stackValue
    {
        stackValue(int a) {shapeIndex = a;}
        int shapeIndex;
        stackValue* previousStackValue = NULL;
    };
    stackValue* topOfStack = NULL;

    void push(int shapeIndex);
    int peek();
    void pop();
    bool isEmpty();
};

colorType colorCalc(int depth,
                    vec3 N,
                    stack* shapeStack,
                    double F0,
                    double alpha,
                    double eta,
                    colorType bkg,
                    shapes shapeStore,                     
                    intersectionPoint currentPoint,        // cameraIntersection[i*PWIDTH+j]
                    lightSources Ls,                       // Ls                              
                    vec3 origin,                           // viewing.eye
                    rayType currentRay,                    // rays[i*PWIDTH+J]
                    int reflectFlag,
                    int refractFlag);

#endif