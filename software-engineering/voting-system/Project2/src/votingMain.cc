#include <iostream>
#include <string>
#include <vector>
#include <sstream>

#include "ElectionObject.h"
#include "FileParser.h"

int main(int argc, char* argv[])
{
    std::vector<std::string> filenames(1);
    std::string input;
    bool multiple = false;
    if(argc < 2) { // no filenames
        while(true) {
            std::cout << "Would you like to import multiple files? Y/N \n";
            std::cin >> input;
            if(input == "Y" || input == "y") {
                multiple = true;
                std::cout << "Please enter all file names to import, separated by spaces: \n";
                std::cin.ignore();
                std::string files = "";
                while(files == "") {
                    std::getline(std::cin, files);
                }
                if(files[files.length()-1] == ' ') {
                    files = files.substr(0, files.length()-1);
                }
                std::string delimiter = " ";
                std::string filename = files.substr(0, files.find(delimiter));
                files = files.substr(files.find(delimiter)+1, files.length());
                filenames[0] = filename;
                while(files.find(delimiter) != -1) {
                    filename = files.substr(0, files.find(delimiter));
                    filenames.push_back(filename);
                    files = files.substr(files.find(delimiter)+1, files.length());
                    if(files.find(delimiter) == -1) {
                        filenames.push_back(files);
                    }
                }
                for(int j = 0; j < filenames.size(); j++) {
                    std::cout << filenames[j] << "\n";
                }
                break;
            } else if(input == "N" || input == "n") {
                std::cout << "Please enter a file name to import: \n";
                std::cin >> filenames[0];
                break;
            } else { // If incorrect input
                std::cout << "Incorrect input detected, please try again with (Y)es or (N)o.\n";
            }
        }
    } else if(argc == 2) { // one filename, assumed only singular mode wanted
        filenames[0] = argv[1];
    } else { // multiple filenames, assumed all files are given when program is called
        multiple = true;
        filenames[0] = argv[1];
        for(int i = 1; i < argc - 1; i++) {
            filenames.push_back(argv[i+1]);
        }
    }
    FileParser fp;
    if(multiple) { // if multiple files
        if(fp.openMultiple(filenames)) {  // if a file fails to open
            return 1;
        }
        ElectionObject* election = fp.getMultipleMetadata();
        election->calculateWinners();
        delete election;
    } else { // if singular file
        if(fp.openFile(filenames[0], 0)) { // if file fails to open
            return 1;
        }
        ElectionObject* election = fp.getMetadata();
        election->calculateWinners();
        delete election;
    }

    return 0;
}
