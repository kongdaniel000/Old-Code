#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>

//I know I really should use inheritance, but this took me a long time and I'll probably do that in 1b.

struct vec3
{
    double x;
    double y;
    double z;

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

vec3 cross(vec3 fst, vec3 snd)
{
    vec3 crossProduct;
    crossProduct.x = (fst.y*snd.z) - (fst.z*snd.y);
    crossProduct.y = -((fst.x*snd.z) - (fst.z*snd.x));
    crossProduct.z = (fst.x*snd.y) - (fst.y*snd.x);
    return crossProduct;
}

vec3 normalize(vec3 vec)
{
    vec3 normalizedVector;
    double norm = sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
    normalizedVector.x = vec.x / norm;
    normalizedVector.y = vec.y / norm;
    normalizedVector.z = vec.z / norm;
    return normalizedVector;
}

void printVec(vec3 vec)
{
    std::cout << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")\n";
}

struct cameraType
{
    vec3 eye;
    vec3 viewdir;
    vec3 updir;
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
};

void arrayToVec(vec3* vec, double arr[])
{
    vec->x = arr[0];
    vec->y = arr[1];
    vec->z = arr[2];
}

struct colorType
{
    int r = 0;
    int g = 0;
    int b = 0;
};

void printCol(colorType col)
{
    std::cout << "(" << col.r << ", " << col.g << ", " << col.b << ")\n";
}

void arrayToColor(colorType* col, double arr[])
{
    col->r = arr[0]*255;
    col->g = arr[1]*255;
    col->b = arr[2]*255;
}

void cameraSetup(cameraType* cam, double ASPECT_RATIO, double vfov)
{
    cam->w_ = cam->viewdir*-1.0;
    cam->w = normalize(cam->w_);
    cam->u_ = cross(cam->viewdir, cam->updir);
    cam->u = normalize(cam->u_);
    cam->v_ = cross(cam->u, cam->viewdir);
    cam->v = normalize(cam->v_);
    //d is arbitrarily selected as 1
    cam->d = 1.0;
    cam->height = 2 * cam->d * tan((vfov / 180 * M_PI)/2);
    cam->width = cam->height * ASPECT_RATIO;
    vec3 w = cam->viewdir * cam->d;
    vec3 u = cam->u * (cam->width / 2);
    vec3 v = cam->v * (cam->height / 2);
    cam->ul = cam->eye + w - u + v;
    cam->ur = cam->eye + w + u + v;
    cam->ll = cam->eye + w - u - v;
    cam->lr = cam->eye + w + u - v;
}

struct rayType
{
    vec3 origin;
    vec3 direction;
};

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

struct sphereType
{
    vec3 center;
    double radius;
    colorType col;
};

void arrayToSphere(sphereType* sph, double arr[])
{
    sph->center.x = arr[0];
    sph->center.y = arr[1];
    sph->center.z = arr[2];
    sph->radius = arr[3];
}

struct shapesAndColors
{
    int numberOfShapes = 0;
    int numberOfColors = 0;
    std::vector<std::string> SVector;
    std::vector<std::string> CVector;
};

struct intersect
{
    colorType colorOfPixel;
    double distanceFromOrigin = 0;
};

double isIntersectingSphere(rayType ry, sphereType sph)
{
    double A = pow(ry.direction.x, 2)+pow(ry.direction.y, 2)+pow(ry.direction.z, 2);
    double B = 2*((ry.direction.x*(ry.origin.x-sph.center.x))+(ry.direction.y*(ry.origin.y-sph.center.y))+(ry.direction.z*(ry.origin.z-sph.center.z)));
    double C = pow((ry.origin.x-sph.center.x), 2)+pow((ry.origin.y-sph.center.y), 2)+pow((ry.origin.z-sph.center.z), 2)-pow(sph.radius, 2);

    double disc = solveDiscriminant(A, B, C);
    if(disc < 0)
    {
        return -1.0;
    }
    else
    {
        double minus = minusQuadratic(A, B, C);
        double plus = plusQuadratic(A, B, C);
        if(minus < plus && minus > 0)
        {
            return minus;
        }
        else if (plus > 0)
        {
            return plus;
        }
        else
        {
            return -1.0;
        }
    }
}

void vectorToArray(std::string keyword, double arr[], int length, std::vector<std::string> collect)
{
    //Scan through collect for the keyword
    for(int i = 0; i < collect.size(); i++)
    {
        //If keyword is found at index i in collect, then
        if(collect.at(i) == keyword)
        {
            //The strings following the keyword should be the values of that keyword, 
            //so assign the corresponding array with values following the keyword
            for(int j = 0; j < length; j++)
            {
                arr[j] = std::stod(collect.at(i+j+1));
            }
            //Keyword has been found, so no need to search through rest of file
            return;
        }
    }
    //If the for loop is exited, then the keyword has not been found in collect

    //Tell the user which keyword has not been found, so it will be set to default
    std::cout << "Keyword: " << keyword << " not found, setting to default\n";
}

//Transfers input to relevant shape struct, and returns number of shapes and amterial colors detected at input
shapesAndColors toShape(std::string shape, int shapeElements, std::vector<std::string> collect)
{
    shapesAndColors SandC;
    //Scan through collect for the shape and mtlcolor
    for(int i = 0; i < collect.size(); i++)
    {
        //If keyword is found at index i in collect, then
        if(collect.at(i) == shape)
        {
            SandC.numberOfShapes++;
            for(int j = 0; j < shapeElements + 1; j++)
            {
                SandC.SVector.push_back(collect.at(i+j));
            }
        }
        //Find topmost material color (first found color is assumed to be corresponding to the shape, I'm not dealing with formatting)
        if(collect.at(i) == "mtlcolor")
        {
            SandC.numberOfColors++;
            for(int j = 0; j < 4; j++)
            {
                SandC.CVector.push_back(collect.at(i+j));
            }
        }
    }
    if(SandC.numberOfShapes > SandC.numberOfColors)
    {
        std::cout << "Number of colors is less than number of shapes, shape(s) will be dropped\n";
    }
    else if(SandC.numberOfShapes < SandC.numberOfColors)
    {
        std::cout << "Number of shapes is less than number of colors, color(s) will be dropped\n";
    }
    return SandC;
}

int main()
{
    //Vector to collect file input
    std::vector<std::string> collect;

    //Arrays to store values, default initialization
    double eye[3] = {0.0, 0.0, 0.0};
    double viewdir[3] = {0.0, 0.0, 1.0};
    double updir[3] = {0.0, 1.0, 0.0};
    double vfov[1] = {90.0};
    double imsize[2] = {400.0, 400.0};
    double bkgcolor[3] = {0.0, 0.0, 0.0};
    shapesAndColors spheres;

    //Open given input file
    std::string targetFile;
    std::ifstream file;
    std::cout << "Please give the name of the input file (with extension):\n";
    std::cin >> targetFile;
    file.open(targetFile);

    //If input file cannot be read, stop program telling the user that the file failed to open
    if (file.fail())
    {
        std::cout << "Input file failed to open!\n";
    }
    //Read input
    else
    {
        std::string s;
        //Read word-by-word
        while (file >> s)
        {
            //Store words to collect vector
            collect.push_back(s);
        }
        file.close();
    }

    //Scan through collect vector for keywords and assign appropriate values to arrays
    //vectorToArray is a function which assigns the keyword array the corresponding values
    vectorToArray("eye", eye, 3, collect);
    vectorToArray("viewdir", viewdir, 3, collect);
    vectorToArray("updir", updir, 3, collect);
    vectorToArray("vfov", vfov, 1, collect);
    vectorToArray("imsize", imsize, 2, collect);
    vectorToArray("bkgcolor", bkgcolor, 3, collect);

    //Set pixel width, pixel height, and aspect ratio
    const int PWIDTH = imsize[0];
    const int PHEIGHT = imsize[1];
    const double ASPECT_RATIO = static_cast<double>(PWIDTH) / static_cast<double>(PHEIGHT);

    //Set Viewing Coordinate System up
    cameraType viewing;
    arrayToVec(&viewing.eye, eye);
    arrayToVec(&viewing.viewdir, viewdir);
    arrayToVec(&viewing.updir, updir);

    //Check that view direction and up direction are not the same
    if(viewing.viewdir == viewing.updir)
    {
        std::cout << "View direction cannot be equal to up direction!\n";
        exit(1);
    }

    //Setup variables for viewing window
    cameraSetup(&viewing, ASPECT_RATIO, vfov[0]);
    
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

    //Setup shapes and colors
    spheres = toShape("sphere", 4, collect);

    colorType bkg;
    arrayToColor(&bkg, bkgcolor);
    //Min is the smallest number out of colors and shapes, so that the excess is dropped
    int min;
    if(spheres.numberOfColors > spheres.numberOfShapes)
    {
        min = spheres.numberOfShapes;
    }
    else
    {
        min = spheres.numberOfColors;
    }
    //Sphere array
    sphereType* sphereArray = new sphereType[min];
    for(int i = 0; i < min; i++)
    {
        sphereType sph;
        sph.center.x = std::stod(spheres.SVector.at(5*i+1));
        sph.center.y = std::stod(spheres.SVector.at(5*i+2));
        sph.center.z = std::stod(spheres.SVector.at(5*i+3));
        sph.radius = std::stod(spheres.SVector.at(5*i+4));

        sph.col.r = std::stod(spheres.CVector.at(4*i+1))*255;
        sph.col.g = std::stod(spheres.CVector.at(4*i+2))*255;
        sph.col.b = std::stod(spheres.CVector.at(4*i+3))*255;
        
        sphereArray[i] = sph;
    }

    //The final array before printing, sets the color of the pixels in the image after checking for intersection
    intersect* intersection = new intersect[PHEIGHT*PWIDTH];
    for(int i = 0 ; i < PHEIGHT; i++)
    {
        for(int j = 0; j < PWIDTH; j++)
        {
            //Set background color first
            intersection[i*PWIDTH+j].colorOfPixel = bkg;
            intersection[i*PWIDTH+j].distanceFromOrigin = 0;

            //Flag for if this is the first time a color is being put at this pixel
            bool isFirst = true;
            for(int k = 0; k < min; k++)
            {
                double inter = isIntersectingSphere(rays[i*PWIDTH+j], sphereArray[k]);
                //inter is only returned as -1.0 if the discriminant is negative, which implies the ray misses the sphere
                
                //This checks to make sure that the color of the pixel is the sphere closest to the eye
                if((isFirst || inter < intersection[i*PWIDTH+j].distanceFromOrigin) && inter > 0)
                {
                    intersection[i*PWIDTH+j].colorOfPixel = sphereArray[k].col;
                    intersection[i*PWIDTH+j].distanceFromOrigin = inter;
                    isFirst = false;
                }
            }
        }
    }

    delete[] sphereArray;
    delete[] rays;

    
    //Output filename
    std::string ofile
    if(argv[2] != NULL)
    {
        ofile = argv[2];
    }
    else
    {
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
            output_stream << intersection[i*PWIDTH+j].colorOfPixel.r << " " << intersection[i*PWIDTH+j].colorOfPixel.g << " " << intersection[i*PWIDTH+j].colorOfPixel.b << " ";
        }
    }

    output_stream.close();

    delete[] intersection;

    return 0;
}