#include "Party.h"

/**
 * @brief File name: Party.cc. Description: Contains method definitions
 * for the Party class. Author: Ruolei Zeng.
 */

Party::Party() {
    numVotes = 0;
    firstSeats = 0;
    secondSeats = 0;
    std::string candidates = "";
    numCandidates = 0;
    leftoverVotes = 0;
}

void Party::setCandidates(std::string candidateString) {
    candidates = candidateString;
    int i = 0;
    int num = 1;
    while(i < candidates.length()) {
        if(candidates[i] == ',') {
            num++;
        }
        i++;
    }
    numCandidates = num;
}

Party::~Party() {
    //delete[] candidates;
}