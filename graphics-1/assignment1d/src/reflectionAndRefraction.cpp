#include "reflectionAndRefraction.h"
#define MAXDEPTH 10

stack::~stack()
{
    while(topOfStack != NULL)
    {
        stackValue* deleteValue = topOfStack;
        topOfStack = topOfStack->previousStackValue;
        delete deleteValue;
    }
}

void stack::push(int shapeIndex)
{
    stackValue* prev = topOfStack;
    topOfStack = new stackValue(shapeIndex);
    topOfStack->previousStackValue = prev;
}

int stack::peek()
{

    if(topOfStack == NULL)
    {
        return -1;
    }
    return topOfStack->shapeIndex;
}

void stack::pop()
{
    stackValue* deleteValue = topOfStack;
    topOfStack = topOfStack->previousStackValue;
    delete deleteValue;
}

bool stack::isEmpty()
{
    return topOfStack == NULL;
}

colorType colorCalc(int depth,
                    vec3 N,
                    stack* shapeStack,
                    double F0,
                    double alpha,
                    double eta,
                    colorType bkg,
                    shapes shapeStore,                     // shapeStore
                    intersectionPoint currentPoint,        // cameraIntersection[i*PWIDTH+j]
                    lightSources Ls,                       // Ls                              
                    vec3 origin,                           // viewing.eye
                    rayType currentRay,                    // rays[i*PWIDTH+J]
                    int reflectFlag,
                    int refractFlag)
{
    colorType returnColor; //Set background color first
    if(depth >= MAXDEPTH)
    {
        return returnColor;
    }
    returnColor = bkg; //Set background color first
    if(currentPoint.isIntersect) //Checks if the ray intersects anything
    {
        lightType currentColor = shapeStore.SVector.at(currentPoint.shapeIndex)->L; //Colors taken from the shape found at this pixel
        returnColor = shapeStore.SVector.at(currentPoint.shapeIndex)->findODLambda(currentPoint.coordinates) * currentColor.weight.ka; //Set the ambient
        rayType VRay = pointsToRay(currentPoint.coordinates, origin); //Find direction from point to eye and set V equal to it
        vec3 V = normalize(VRay.direction); //This V is the vector that will be used in calculations
        vec3 L; //Declare L for later use
        rayType LRay; //Declare LRay for later use
        
        for(int k = 0; k < Ls.numberOfLights; k++) //Loops through each light source
        {
            lightSourceType currentLightSource = Ls.lightVector.at(k); //The light source we are currently looking at
            double S = 1; //Shadow flag, default to no shadow

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
                    S = S * (1-shapeStore.SVector.at(n)->L.alpha); //If LRay intersects an object between light source and surface, then something is in front and casts a shadow
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
            colorType diffuse =     (shapeStore.SVector.at(currentPoint.shapeIndex)->findODLambda(currentPoint.coordinates) * 
                                     currentColor.weight.kd) * colorDot1;
            colorType specular =    (currentColor.specularH * currentColor.weight.ks) * 
                                     pow(colorDot2, currentColor.exponent);

            //Finally, add the diffuse and specular terms multiplied by shadow flag
            returnColor = returnColor + ((diffuse + specular) * S);
        }
    }
    
    vec3 I = normalize(currentRay.direction * -1);
    double costhetai = dot(N, I);
    double Fr = F0 + (1-F0) * pow((1-costhetai), 5);
    
    double newAlpha = shapeStore.SVector.at(currentPoint.shapeIndex)->L.alpha;
    double newEta;
    if(shapeStack->peek() == currentPoint.shapeIndex && shapeStack->topOfStack->previousStackValue == NULL)
    {
        newEta = 1;
    }
    else
    {
        newEta = shapeStore.SVector.at(currentPoint.shapeIndex)->L.eta;
    }
    
    double sinthetai = sqrt(1-pow(costhetai, 2));
    if(sinthetai <= newEta/eta && refractFlag)
    {
        double costhetat = sqrt(1-(pow(eta/newEta, 2) * (1-pow(costhetai, 2))));
        vec3 T = (N * -1) * costhetat + (N * costhetai - I) * (eta/newEta); 
        rayType TRay;
        TRay.direction = T;
        TRay.origin = currentPoint.coordinates + (T * 0.000001); //Offset since rays have to enter and leave an object but I don't 
                                                                 //want the program to recognize the shape it's passing through multiple times

        intersectionPoint newIntersection1;
        bool isFirst = true; //Flag for if this is the first time an intersection is found at this pixel
        for(int k = 0; k < shapeStore.SVector.size(); k++)
        {
            intersectionPoint point = shapeStore.SVector.at(k)->rayTrace(TRay, k);
            double inter = point.time;
            //inter is only returned as -1.0 if the discriminant is negative, which implies the ray misses the shape
            //or if the ray intersects the shape behind the origin, at which point it doesn't matter
            
            //This checks to make sure that the color of the pixel is the shape closest to the eye
            if((isFirst || inter < newIntersection1.time) && inter > 0)
            {
                newIntersection1.isIntersect = true;
                newIntersection1.coordinates = point.coordinates;
                newIntersection1.time = inter;
                newIntersection1.shapeIndex = k;
                isFirst = false;
            }
        }
        if(newIntersection1.shapeIndex != -1 && newAlpha < 1) //If new intersections are made
        {
            //Refraction value
            if(newIntersection1.shapeIndex == shapeStack->peek()) //When a ray exits a shape into air
            {
                shapeStack->pop();
                vec3 newN = shapeStore.SVector.at(newIntersection1.shapeIndex)->surfaceNormal(newIntersection1.coordinates) * -1;
                returnColor = returnColor 
                            + colorCalc(depth, newN, shapeStack, F0, newAlpha, newEta, bkg, shapeStore, newIntersection1, Ls, currentPoint.coordinates, TRay, 0, 1) * (1-Fr) * (1-newAlpha);
            }   
            else
            {   
                if(shapeStore.SVector.at(newIntersection1.shapeIndex)->shapeName != "triangle")
                {
                    shapeStack->push(newIntersection1.shapeIndex);
                }
                returnColor = returnColor 
                            + colorCalc(depth, N, shapeStack, F0, newAlpha, newEta, bkg, shapeStore, newIntersection1, Ls, currentPoint.coordinates, TRay, 0, 1) * (1-Fr) * (1-newAlpha);
            }
        }
    }

    vec3 R = N * costhetai * 2 - I;
    
    rayType RRay;
    RRay.direction = R;
    RRay.origin = currentPoint.coordinates;

    intersectionPoint newIntersection2;
    bool isFirst = true; //Flag for if this is the first time an intersection is found at this pixel
    for(int k = 0; k < shapeStore.SVector.size(); k++)
    {
        if(k != currentPoint.shapeIndex) //Check that the ray doesn't include the shape it is originating from
        {
            intersectionPoint point = shapeStore.SVector.at(k)->rayTrace(RRay, k);
            double inter = point.time;
            //inter is only returned as -1.0 if the discriminant is negative, which implies the ray misses the shape
            //or if the ray intersects the shape behind the camera, at which point it doesn't matter
            
            //This checks to make sure that the color of the pixel is the shape closest to the eye
            if((isFirst || inter < newIntersection2.time) && inter > 0)
            {
                newIntersection2.isIntersect = true;
                newIntersection2.coordinates = point.coordinates;
                newIntersection2.time = inter;
                newIntersection2.shapeIndex = k;
                isFirst = false;
            }
        }
    }

    if(newIntersection2.shapeIndex == -1) //If no new intersections are made
    {
        returnColor.clamp();
        return returnColor;
    }

    if(reflectFlag)
    {
        returnColor = returnColor 
                //Reflection value
                + colorCalc(depth+1, N, shapeStack, F0, newAlpha, newEta, bkg, shapeStore, newIntersection2, Ls, currentPoint.coordinates, RRay, 1, 0) * Fr;
    }
    
    //Clamps light at pixel so that it cannot exceed max value
    returnColor.clamp();
    return returnColor; 
}