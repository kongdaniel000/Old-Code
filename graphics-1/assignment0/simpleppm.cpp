#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <ctype.h>

int main()
{
    //Default image size if not given
    int WIDTH = 1024;
    int HEIGHT = 1024;

    //Settings array for keyword and dimensions
    std::string settings[3] = {"place", "hold", "er"};

    //Input filename
    std::ifstream file;
    file.open("input.txt");

    //Read input
    if (file.fail())
    {
        std::cout << "Input file failed to open";
    }
    else
    {
        std::string s;
        int i = 0;
        //Read word-by-word
        while (file >> s)
        {
            //Only care about first three words: imsize, width, and height
            if (i < 3)
            {
                //Store word to settings array
                settings[i] = s;
                i++;
            }
        }
        file.close();
    }

    //Keyword check to set width and height, otherwise ignore settings array
    if(settings[0] == "imsize")
    {
        std::string w = settings[1];
        std::string h = settings[2];

        //Check width and heights for expected inputs (positive and numerical)
        for(int i = 0; i < w.length(); i++)
        {
            if(!isdigit(w[i]))
            {
                throw std::invalid_argument("Invalid width");
            } 
        }

        for(int i = 0; i < h.length(); i++)
        {
            if(!isdigit(h[i]))
            {
                throw std::invalid_argument("Invalid height");
            } 
        }

        //Store new width and height
        WIDTH = std::stoi(w);
        HEIGHT = std::stoi(h);
    }

    //Output filename
    std::string ofile = "assignment0.ppm";
    std::ofstream output_stream(ofile, std::ios::out);

    //Seed random values for pixel generation
    srand(13);

    //Header
    output_stream << "P3\n"
                  << WIDTH << "\n"
                  << HEIGHT << "\n"
                  << 255 << "\n";

    //Generate random pixels for image, columns then rows
    for(uint32_t x = 0; x < WIDTH; x++)
    {
        for(uint32_t y = 0; y < HEIGHT; y++)
        {
            output_stream << rand() % 256 << " " << rand() % 256 << " " << rand() % 256 << " ";
        }
    }

    output_stream.close();

    return 0;
}