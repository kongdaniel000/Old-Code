#include <iostream>
#include <string>
#include <vector>

#include "ElectionObject.h"
#include "FileParser.h"

int main(int argc, char* argv[])
{
    std::string filename;
    if(argc < 2) {
        std::cout << "Please enter a file name to import: ";
        std::cin >> filename;
    } else if(argc == 2) {
        filename = argv[1];
    } else {
        std::cout << "Too many arguments! Program ending.";
        return 1;
    }
    FileParser fp;
    if(fp.openFile(filename)) {
        return 2;
    }
    ElectionObject* election = fp.getMetadata();
    election->calculateWinners();
    delete election;

    return 0;
}
