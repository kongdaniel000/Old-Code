#include "ElectionObject.h"

#include <ctime>

std::vector<ElectionEntity*> ElectionObject::breakTie(std::vector<ElectionEntity*> entities, int winners, bool isRandom) {
    std::vector<ElectionEntity*> win;
    if(isRandom) {
        std::srand(time(nullptr));
        while(winners > 0) {
            int random = std::rand() % entities.size();
            win.push_back(entities[random]);
            entities.erase(entities.begin()+random);
            winners--;
        }
    } else {
        for(int i = 0; i < winners; i++) {
            win.push_back(entities[i]);
        }
    }
    return win;
 }