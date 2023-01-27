#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>

//Yeah I'm not separating the files in 1b either, maybe 1c

int main()
{
    srand(13);
    
    //Output filename
    std::string ofile;
    ofile = "light.txt";
    std::ofstream output_stream(ofile, std::ios::out);

    for(int x = -1200; x < 1201; x++)
    {
        for(int y = -1200; y < 1201; y++)
        {
            for(int z = 800; z < 1501; z++)
            {
                if((x*x+y*y+(z-2000)*(z-2000)) == 1100*1100)
                {
                    if(rand()%100 < 17)
                    {
                        output_stream << "light " << x << " " << y << " " << z << " 1 1 0.59 0\n";
                    }
                }
            }
        }
    }

    output_stream.close();

    std::cout << "File output to: " << ofile << std::endl;

    return 0;
}