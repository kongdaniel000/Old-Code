#include "fileIO.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

//Reads collect vector into necessary places
void readInput(cameraType *cam, colorType *bkgcolor, std::vector<std::string> collect)
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
        if(!found[i])
        {
            std::cout << "Keyword " << keyword[i] << " not found, setting to default\n";
        }
    }
}

//Transforms input into shapes struct and returns it
shapes toShape(std::vector<std::string> collect)
{
    shapes shapeStore;
    lightType lit;

    int textureIndex = 0;
    colorType* texture;
    int THEIGHT;
    int TWIDTH;

    //Scan through collect for the shape and mtlcolor
    for(int i = 0; i < collect.size(); i++)
    {
        //If keyword is found at index i in collect, then set variables according to syntax
        if(collect.at(i) == "sphere")
        {
            sphereType* sph = new sphereType;

            sph->center.x = std::stod(collect.at(i+1));
            sph->center.y = std::stod(collect.at(i+2));
            sph->center.z = std::stod(collect.at(i+3));
            sph->radius = std::stod(collect.at(i+4));
            sph->L = lit;
            sph->texture = shapeStore.textureVec.at(textureIndex);
            sph->TWIDTH = TWIDTH;
            sph->THEIGHT = THEIGHT;

            shapeStore.SVector.push_back(sph);
        }

        //If keyword is found at index i in collect, then set variables according to syntax
        // if(collect.at(i) == "cylinder")
        // {
        //     shapeStore.numberOfShapes++;
        //     cylinderType* cyl = new cylinderType;

        //     cyl->center.x = std::stod(collect.at(i+1));
        //     cyl->center.y = std::stod(collect.at(i+2));
        //     cyl->center.z = std::stod(collect.at(i+3));
        //     cyl->dir.x = std::stod(collect.at(i+4));
        //     cyl->dir.y = std::stod(collect.at(i+5));
        //     cyl->dir.z = std::stod(collect.at(i+6));
        //     cyl->radius = std::stod(collect.at(i+7));
        //     cyl->length = std::stod(collect.at(i+8));

        //     shapeStore.SVector.push_back(cyl);
        // }
        //Find topmost material color (first found color is assumed to be corresponding to the shape, I'm not dealing with formatting)

        if(collect.at(i) == "v")
        {
            vec3 vec;

            vec.x = std::stod(collect.at(i+1));
            vec.y = std::stod(collect.at(i+2));
            vec.z = std::stod(collect.at(i+3));

            shapeStore.triangleVectors.push_back(vec);
        }

        if(collect.at(i) == "vn")
        {
            vec3 vec;

            vec.x = std::stod(collect.at(i+1));
            vec.y = std::stod(collect.at(i+2));
            vec.z = std::stod(collect.at(i+3));

            vec = normalize(vec);

            shapeStore.triangleNormalVectors.push_back(vec);
        }

        if(collect.at(i) == "vt")
        {
            textureCoordinate tex;

            tex.u = std::stod(collect.at(i+1));
            tex.v = std::stod(collect.at(i+2));

            shapeStore.triangleTextureCoordinates.push_back(tex);
        }

        if(collect.at(i) == "f")
        {
            triangleType* tri = new triangleType;

            for(int j = 0; j < 3; j++)
            {
                std::string currentString = collect.at(i+j+1);
                std::string vString = "";
                std::string vtString = "";
                std::string vnString = "";
                int endOfVert = currentString.length();
                int startofText = currentString.length();
                int endOfText = currentString.length();
                int startofNorm = currentString.length();

                for(int k = 1; k < currentString.length(); k++) //Starts at 1 because '/' should not be at the beginning of the string
                {
                    if(currentString[k] == '/')
                    {
                        endOfVert = k;
                        if(currentString[k+1] == '/')
                        {
                            endOfText = k + 1;
                            startofNorm = k + 2;
                            break; 
                        }
                        else
                        {
                            startofText = k + 1;
                            for(int l = k+1; l < currentString.length(); l++)
                            {
                                if(currentString[l] == '/')
                                {
                                    endOfText = l;
                                    startofNorm = l + 1;
                                }
                            }
                            break;
                        }
                    }
                }

                for(int n = 0; n < endOfVert; n++)
                {
                    vString += currentString[n];
                }
                for(int n = startofText; n < endOfText; n++)
                {
                    vtString += currentString[n];
                }
                for(int n = startofNorm; n < currentString.length(); n++)
                {
                    vnString += currentString[n];
                }

                tri->v[j] = shapeStore.triangleVectors.at(std::stoi(vString) - 1);
                if(!vtString.empty())
                {
                    tri->vt[j] = shapeStore.triangleTextureCoordinates.at(std::stoi(vtString) - 1);
                }
                if(!vnString.empty())
                {
                    tri->isSmooth = true;
                    tri->vn[j] = shapeStore.triangleNormalVectors.at(std::stoi(vnString) - 1);
                }
            }
            tri->L = lit;
            tri->setTriangle();
            tri->texture = shapeStore.textureVec.at(textureIndex);
            tri->THEIGHT = THEIGHT;
            tri->TWIDTH = TWIDTH;

            shapeStore.SVector.push_back(tri);
        }

        if(collect.at(i) == "mtlcolor")
        {
            lit.intrinsic.r = std::stod(collect.at(i+1));
            lit.intrinsic.g = std::stod(collect.at(i+2));
            lit.intrinsic.b = std::stod(collect.at(i+3));

            lit.specularH.r = std::stod(collect.at(i+4));
            lit.specularH.g = std::stod(collect.at(i+5));
            lit.specularH.b = std::stod(collect.at(i+6));

            lit.weight.ka = std::stod(collect.at(i+7));
            lit.weight.kd = std::stod(collect.at(i+8));
            lit.weight.ks = std::stod(collect.at(i+9));

            lit.exponent = std::stod(collect.at(i+10));

            lit.alpha = std::stod(collect.at(i+11));
            lit.eta = std::stod(collect.at(i+12));
        }

        if(collect.at(i) == "texture")
        {  
            textureIndex++;

            std::string targetFile;
            targetFile = "./texture files/" + collect.at(i+1);
            std::ifstream file;
            file.open(targetFile);

            if (file.fail()) //If input file cannot be read tell the user that the file failed to open
            {
                std::cout << "Input file failed to open, objects will not use this texture: " << targetFile << "\n";
            }
            else //Add texture values to textureArray and push textureArray to textureVec
            {
                std::string s;
                bool hasRead = false;

                double MAXVAL; // A double because I don't want to deal with type operations

                while (file >> s) //Read word-by-word
                {
                    if(s == "P3") {} //Ignore P3 for now
                    else if(s == "#") //Now reading comment
                    {
                        getline(file, s); //Ignore entire comment line
                    }
                    else if(!hasRead) //Now reading the rest of the header
                    {
                        TWIDTH = std::stoi(s);
                        file >> s;
                        THEIGHT = std::stoi(s);
                        file >> s;
                        MAXVAL = std::stod(s);

                        texture = new colorType[TWIDTH*THEIGHT];
                        hasRead = true;
                    }
                    else //Now reading color values
                    {
                        for(int j = 0; j < THEIGHT; j++)
                        {
                            for(int k = 0; k < TWIDTH; k++)
                            {
                                texture[j*TWIDTH+k].r = std::stod(s) / MAXVAL;
                                file >> s;
                                texture[j*TWIDTH+k].g = std::stod(s) / MAXVAL;
                                file >> s;
                                texture[j*TWIDTH+k].b = std::stod(s) / MAXVAL;
                                file >> s;
                            }
                        }
                    }
                }
            }

            file.close();
            shapeStore.textureVec.push_back(texture);
        }
    }
    return shapeStore;
}

//Transforms input into lightSources struct and returns it
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