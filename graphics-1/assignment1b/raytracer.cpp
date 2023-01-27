#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>

//Yeah I'm not separating the files in 1b either, maybe 1c

struct vec3
{
    double x;
    double y;
    double z;

    vec3()
    {
        this->x = 0.0;
        this->y = 0.0;
        this->z = 0.0;
    }

    vec3(double a, double b, double c)
    {
        this->x = a;
        this->y = b;
        this->z = c;
    }

    vec3 operator+(const vec3& b)
    {
        vec3 vec;
        vec.x = this->x + b.x;
        vec.y = this->y + b.y;
        vec.z = this->z + b.z;
        return vec; 
    }

    vec3 operator-(const vec3& b)
    {
        vec3 vec;
        vec.x = this->x - b.x;
        vec.y = this->y - b.y;
        vec.z = this->z - b.z;
        return vec; 
    }

    vec3 operator*(const double& b)
    {
        vec3 vec;
        vec.x = this->x * b;
        vec.y = this->y * b;
        vec.z = this->z * b;
        return vec;
    }

    bool operator==(const vec3& b) 
    {
        bool x = this->x == b.x;
        bool y = this->y == b.y;
        bool z = this->z == b.z;
        return (x && y && z);
    }
};

//Returns cross-product of two vectors
vec3 cross(vec3 fst, vec3 snd)
{
    vec3 crossProduct;
    crossProduct.x = (fst.y*snd.z) - (fst.z*snd.y);
    crossProduct.y = -((fst.x*snd.z) - (fst.z*snd.x));
    crossProduct.z = (fst.x*snd.y) - (fst.y*snd.x);
    return crossProduct;
}

//Normalizes a vector
vec3 normalize(vec3 vec)
{
    vec3 normalizedVector;
    double norm = sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
    normalizedVector.x = vec.x / norm;
    normalizedVector.y = vec.y / norm;
    normalizedVector.z = vec.z / norm;
    return normalizedVector;
}

//Returns dot-product of two vectors
double dot(vec3 fst, vec3 snd)
{
    return fst.x * snd.x + fst.y * snd.y + fst.z * snd.z;
}

//Returns a normalized halfway vector
vec3 halfway(vec3 L, vec3 V)
{
    return normalize((L+V)*0.5);
}

void printVec(vec3 vec)
{
    std::cout << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")\n";
}

struct pixelResolution
{
    int PWIDTH;
    int PHEIGHT;
    double ASPECT_RATIO;

    pixelResolution()
    {
        PWIDTH = 400;
        PHEIGHT = 400;
        ASPECT_RATIO = 1.0;
    }

    pixelResolution(int w, int h)
    {
        this->PWIDTH = w;
        this->PHEIGHT = h;
        this->ASPECT_RATIO = static_cast<double>(PWIDTH) / static_cast<double>(PHEIGHT);
    }
};

struct cameraType
{
    vec3 eye;
    vec3 viewdir;
    vec3 updir;
    double vfov;
    pixelResolution pix;
    vec3 w_;
    vec3 w;
    vec3 u_;
    vec3 u;
    vec3 v_;
    vec3 v;
    double d;
    double height;
    double width;
    vec3 ul;
    vec3 ur;
    vec3 ll;
    vec3 lr;

    cameraType()
    {
        this->eye.x = 0.0;
        this->eye.y = 0.0;
        this->eye.z = 0.0;
        this->viewdir.x = 0.0;
        this->viewdir.y = 0.0;
        this->viewdir.z = 1.0;
        this->updir.x = 0.0;
        this->updir.y = 1.0;
        this->updir.z = 0.0;
        this->vfov = 90.0;
    }
};

struct colorType
{
    double r;
    double g;
    double b;

    colorType()
    {
        this->r = 0.0;
        this->g = 0.0;
        this->b = 0.0;
    }

    colorType(double c1, double c2, double c3)
    {
        this->r = c1;
        this->g = c2;
        this->b = c3;
    }

    colorType operator+(const colorType& a)
    {
        colorType col;
        col.r = this->r + a.r;
        col.g = this->g + a.g;
        col.b = this->b + a.b;
        return col; 
    }

    colorType operator-(const colorType& a)
    {
        colorType col;
        col.r = this->r - a.r;
        col.g = this->g - a.g;
        col.b = this->b - a.b;
        return col; 
    }

    colorType operator*(const double& a)
    {
        colorType col;
        col.r = this->r * a;
        col.g = this->g * a;
        col.b = this->b * a;
        return col; 
    }
};

void printCol(colorType col)
{
    std::cout << "(" << col.r << ", " << col.g << ", " << col.b << ")\n";
}

void cameraSetup(cameraType* cam)
{
    cam->w_ = cam->viewdir*-1.0;
    cam->w = normalize(cam->w_);
    cam->u_ = cross(cam->viewdir, cam->updir);
    cam->u = normalize(cam->u_);
    cam->v_ = cross(cam->u, cam->viewdir);
    cam->v = normalize(cam->v_);
    //d is arbitrarily selected as 1
    cam->d = 1.0;
    cam->height = 2 * cam->d * tan((cam->vfov / 180 * M_PI)/2);
    cam->width = cam->height * cam->pix.ASPECT_RATIO;
    vec3 w = cam->viewdir * cam->d;
    vec3 u = cam->u * (cam->width / 2);
    vec3 v = cam->v * (cam->height / 2);
    cam->ul = cam->eye + w - u + v;
    cam->ur = cam->eye + w + u + v;
    cam->ll = cam->eye + w - u - v;
    cam->lr = cam->eye + w + u - v;
}

//Replaces vectorToArray(), arrayToVec(), arrayToColor(), arrayToSphere(), and settings of PWIDTH and PHEIGHT
void readInput(cameraType *cam, colorType *bkgcolor, std::vector<std::string> collect, bool dontPrint)
{
    //Keyword array for if statements and error checking
    std::string keyword[6] = {"eye", "viewdir", "updir", "vfov", "imsize", "bkgcolor"};

    //Flag array for if values were found for that keyword
    bool found[6] = {false, false, false, false, false, false};

    for(int i = 0; i < collect.size(); i++) //Scan through collect for keywords
    {
        //If "eye" is found at index i in collect, and has not been found before
        if(collect.at(i) == keyword[0] && !found[0])
        {
            //The strings following the keyword should be the values of that keyword, 
            //so assign the corresponding values following the keyword

            vec3 eye;
            eye.x = std::stod(collect.at(i+1));
            eye.y = std::stod(collect.at(i+2));
            eye.z = std::stod(collect.at(i+3));
            cam->eye = eye;

            found[0] = true; //Keyword has been found, so no need to search through rest of file
        }

        if(collect.at(i) == keyword[1] && !found[1])
        {
            vec3 viewdir;
            viewdir.x = std::stod(collect.at(i+1));
            viewdir.y = std::stod(collect.at(i+2));
            viewdir.z = std::stod(collect.at(i+3));
            cam->viewdir = viewdir;

            found[1] = true;
        }

        if(collect.at(i) == keyword[2] && !found[2])
        {
            vec3 updir;
            updir.x = std::stod(collect.at(i+1));
            updir.y = std::stod(collect.at(i+2));
            updir.z = std::stod(collect.at(i+3));
            cam->updir = updir;

            found[2] = true;
        }

        if(collect.at(i) == keyword[3] && !found[3])
        {
            cam->vfov = std::stod(collect.at(i+1));

            found[3] = true;
        }

        if(collect.at(i) == keyword[4] && !found[4])
        {
            pixelResolution pix;
            pix.PWIDTH = std::stod(collect.at(i+1));
            pix.PHEIGHT = std::stod(collect.at(i+2));
            pix.ASPECT_RATIO = static_cast<double>(pix.PWIDTH) / static_cast<double>(pix.PHEIGHT);
            cam->pix = pix;

            found[4] = true;
        }

        if(collect.at(i) == keyword[5] && !found[5])
        {
            colorType bkg;
            bkg.r = std::stod(collect.at(i+1));
            bkg.g = std::stod(collect.at(i+2));
            bkg.b = std::stod(collect.at(i+3));
            *bkgcolor = bkg;

            found[5] = true;
        }
    }

    //Tell the user which keyword has not been found, so it will be set to default
    for(int i = 0; i < 6; i++)
    {
        if(!found[i] && !dontPrint)
        {
            std::cout << "Keyword " << keyword[i] << " not found, setting to default\n";
        }
    }
}

struct rayType
{
    vec3 origin;
    vec3 direction;
};

rayType pointsToRay(vec3 start, vec3 stop)
{
    rayType newRay;
    newRay.origin = start;
    newRay.direction = stop - start;

    return newRay;
}

struct lightWeightType
{
    double ka;
    double kd;
    double ks;
};

struct lightType
{
    colorType intrinsic;
    colorType specularH;
    lightWeightType weight;
    int exponent = 1;
};

struct sphereType
{
    vec3 center;
    double radius = 1.0;
};

//Doesn't work for now
// struct cylinderType
// {

// };

struct shapeType
{
    std::string shape;
    sphereType sphere;
    //Again, doesn't work for now
    // cylinderType cylinder;
};

vec3 surfaceNormal(shapeType object, vec3 coordinate)
{
    vec3 vec;
    if(object.shape == "sphere")
    {
        vec = normalize(coordinate - object.sphere.center);
        return vec;
    }

    std::cout << "Shape not found by surfaceNormal()! Your output will not be what is expected!\n";
    return vec;
}

double solveDiscriminant(double A, double B, double C)
{
    return (B*B) - (4*A*C);
}

double plusQuadratic(double A, double B, double C)
{
    return (-B + sqrt((B*B) - (4*A*C))) / (2*A);
}

double minusQuadratic(double A, double B, double C)
{
    return (-B - sqrt((B*B) - (4*A*C))) / (2*A);
}

struct intersectionPoint
{
    bool isIntersect = false;
    vec3 coordinates;
    int shapeIndex = -1;
    double time = -1.0;
};

intersectionPoint rayTrace(rayType ry, shapeType object, int index)
{
    intersectionPoint p;
    if(object.shape == "sphere")
    {
        double A = pow(ry.direction.x, 2)+pow(ry.direction.y, 2)+pow(ry.direction.z, 2);
        double B = 2*((ry.direction.x*(ry.origin.x-object.sphere.center.x))+(ry.direction.y*(ry.origin.y-object.sphere.center.y))+(ry.direction.z*(ry.origin.z-object.sphere.center.z)));
        double C = pow((ry.origin.x-object.sphere.center.x), 2)+pow((ry.origin.y-object.sphere.center.y), 2)+pow((ry.origin.z-object.sphere.center.z), 2)-pow(object.sphere.radius, 2);

        double disc = solveDiscriminant(A, B, C);
        if(disc > 0)
        {
            double minus = minusQuadratic(A, B, C);
            double plus = plusQuadratic(A, B, C);

            if(minus < plus && minus > 0)
            {
                p.time = minus;
            }
            else if (plus > 0)
            {
                p.time = plus;
            }
            p.coordinates.x = ry.origin.x + (ry.direction.x * p.time);
            p.coordinates.y = ry.origin.y + (ry.direction.y * p.time);
            p.coordinates.z = ry.origin.z + (ry.direction.z * p.time);
            p.shapeIndex = index;
        }
        return p;
    }

    std::cout << " Shape not found by rayTrace()! Your output will not be what is expected!\n";
    return p;
}

struct shapesAndColors
{
    int numberOfShapes = 0;
    int numberOfColors = 0;
    std::vector<shapeType> SVector;
    std::vector<lightType> LVector;
};

//Transfers input to relevant shape struct, and returns number of shapes and amterial colors detected at input
shapesAndColors toShape(std::vector<std::string> collect)
{
    shapesAndColors SandC;
    //Scan through collect for the shape and mtlcolor
    for(int i = 0; i < collect.size(); i++)
    {
        //If keyword is found at index i in collect, then set variables according to syntax
        if(collect.at(i) == "sphere")
        {
            SandC.numberOfShapes++;
            shapeType sph;

            sph.shape = "sphere";
            sph.sphere.center.x = std::stod(collect.at(i+1));
            sph.sphere.center.y = std::stod(collect.at(i+2));
            sph.sphere.center.z = std::stod(collect.at(i+3));
            sph.sphere.radius = std::stod(collect.at(i+4));

            SandC.SVector.push_back(sph);
        }
        //Find topmost material color (first found color is assumed to be corresponding to the shape, I'm not dealing with formatting)
        if(collect.at(i) == "mtlcolor")
        {
            SandC.numberOfColors++;
            lightType lit;

            lit.intrinsic.r = std::stod(collect.at(i+1));
            lit.intrinsic.g = std::stod(collect.at(i+2));
            lit.intrinsic.b = std::stod(collect.at(i+3));

            lit.specularH.r = std::stod(collect.at(i+4));
            lit.specularH.g = std::stod(collect.at(i+5));
            lit.specularH.b = std::stod(collect.at(i+6));

            lit.weight.ka = std::stod(collect.at(i+7));
            lit.weight.kd = std::stod(collect.at(i+8));
            lit.weight.ks = std::stod(collect.at(i+9));

            lit.exponent = std::stoi(collect.at(i+10));

            SandC.LVector.push_back(lit);
        }
    }
    if(SandC.numberOfShapes > SandC.numberOfColors)
    {
        std::cout << "Number of colors is less than number of shapes, " << SandC.numberOfShapes - SandC.numberOfColors << " shape(s) will be dropped\n";
    }
    else if(SandC.numberOfShapes < SandC.numberOfColors)
    {
        std::cout << "Number of shapes is less than number of colors, " << SandC.numberOfColors - SandC.numberOfShapes << " color(s) will be dropped\n";
    }
    return SandC;
}

struct lightSourceType
{
    vec3 vec;
    int w = 0;
    colorType color;
    vec3 L;
};

struct lightSources
{
    int numberOfLights = 0;
    std::vector<lightSourceType> lightVector;
};

lightSources toLightSources(std::vector<std::string> collect)
{
    lightSources Ls;
    //Scan through collect for the shape and mtlcolor
    for(int i = 0; i < collect.size(); i++)
    {
        //If keyword is found at index i in collect, then set variables according to syntax
        if(collect.at(i) == "light")
        {
            Ls.numberOfLights++;
            lightSourceType lightVariable;

            lightVariable.vec.x = std::stod(collect.at(i+1));
            lightVariable.vec.y = std::stod(collect.at(i+2));
            lightVariable.vec.z = std::stod(collect.at(i+3));
            lightVariable.w = std::stoi(collect.at(i+4));
            lightVariable.color.r = std::stod(collect.at(i+5));
            lightVariable.color.g = std::stod(collect.at(i+6));
            lightVariable.color.b = std::stod(collect.at(i+7));

            Ls.lightVector.push_back(lightVariable);
        }
    }
    return Ls;
}

int main(int argc, char* argv[])
{
    //Vector to collect file input
    std::vector<std::string> collect;

    //Declare necessary variables
    cameraType viewing;
    shapesAndColors spheres;
    colorType bkg;
    bool dontPrint = false;

    //Open given input file
    std::string targetFile;
    if(argv[1] != NULL)
    {
        targetFile = argv[1];
        std::ifstream file;
        file.open(targetFile);

        if (file.fail()) //If input file cannot be read, stop program telling the user that the file failed to open
        {
            std::cout << "Input file failed to open, setting to defaults\n";
            dontPrint = true; //Don't print errors if no file is found
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
    else
    {
        std::cout << "Normal usage is: ./raytracer <input file> <output name>\n";
        std::cout << "No input file found, setting to defaults\n";
        dontPrint = true; //Don't print errors if no file is found
    }

    readInput(&viewing, &bkg, collect, dontPrint); //Read input to necessary variables

    if(viewing.viewdir == viewing.updir) //Check that view direction and up direction are not the same
    {
        std::cout << "View direction cannot be equal to up direction!\n";
        exit(1);
    }

    //Setup variables for viewing window
    cameraSetup(&viewing); 
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

    shapesAndColors SandC = toShape(collect); //Setup shapes and colors

    //Min is the smallest number out of colors and shapes, so that the excess is dropped
    int min;
    if(SandC.numberOfColors > SandC.numberOfShapes)
    {
        min = SandC.numberOfShapes;
    }
    else
    {
        min = SandC.numberOfColors;
    }

    //Sets the pixels which can be seen by the eye and flags them for light checking
    intersectionPoint* cameraIntersection = new intersectionPoint[PHEIGHT*PWIDTH];
    for(int i = 0 ; i < PHEIGHT; i++)
    {
        for(int j = 0; j < PWIDTH; j++)
        {
            bool isFirst = true; //Flag for if this is the first time an intersection is found at this pixel
            for(int k = 0; k < min; k++)
            {
                intersectionPoint point = rayTrace(rays[i*PWIDTH+j], SandC.SVector.at(k), k);
                double inter = point.time;
                //inter is only returned as -1.0 if the discriminant is negative, which implies the ray misses the sphere
                //or if the ray intersects the sphere behind the camera, at which point it doesn't matter
                
                //This checks to make sure that the color of the pixel is the sphere closest to the eye
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
                lightType currentColor = SandC.LVector.at(currentPoint.shapeIndex); //Colors taken from the shape found at this pixel
                lightIntersection[i*PWIDTH+j] = currentColor.intrinsic * currentColor.weight.ka; //Set the ambient
                rayType VRay = pointsToRay(currentPoint.coordinates, viewing.eye); //Find direction from point to eye and set V equal to it
                vec3 N = surfaceNormal(SandC.SVector.at(currentPoint.shapeIndex), currentPoint.coordinates); //Find surface normal of current shape (N is normalized)
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

                    for(int n = 0; n < min; n++) //Loops through each shape
                    {
                        intersectionPoint LInter = rayTrace(LRay, SandC.SVector.at(n), currentPoint.shapeIndex); //Checks if LRay intersects shape at index n

                        //This is a ray opposite from every manner, starts where LRay stops, stops where LRay starts.
                        //This is used to error check shadows, to make sure that the program only generates shadows
                        //if there is an object in the way going both ways.
                        rayType nLRay = LRay;
                        nLRay.direction = nLRay.direction * -1;
                        nLRay.origin = currentLightSource.vec;
                        intersectionPoint nLInter = rayTrace(nLRay, SandC.SVector.at(n), currentPoint.shapeIndex);

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
                    colorType diffuse = (currentColor.intrinsic * currentColor.weight.kd) * colorDot1;
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
        std::cout << "No output name found! Outputting to raytracer.ppm\n";
        ofile = "raytracer.ppm";
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
                          << static_cast<int>(lightIntersection[i*PWIDTH+j].b*255) << " ";
        }
    }

    output_stream.close();

    delete[] lightIntersection;

    std::cout << "File output to: " << ofile << std::endl;

    return 0;
}