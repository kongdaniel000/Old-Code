#ifndef SRC_ELECTION_OBJECT_H_
#define SRC_ELECTION_OBJECT_H_

#include <iostream>
#include <vector>

#include "ElectionEntity.h"

class ElectionObject {
public:
    /**
    * @brief Destructor for ElectionObject, necessary for child objects to be deleted
    **/
    virtual ~ElectionObject() {}
    virtual void calculateWinners() = 0;
    std::vector<ElectionEntity*> breakTie(std::vector<ElectionEntity*> entities, int winners, bool isRandom = true);

    void setNumBallots(int num) {numBallots = num;}
    int getNumBallots() {return numBallots;}
protected:
    int numBallots;
};

#endif