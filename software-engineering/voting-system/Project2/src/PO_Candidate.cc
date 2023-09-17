#include "PO_Candidate.h"
#include <vector>

/**
 * @brief File name: PO_Candidate.cc. Description: Contains attributes and method definitions
 * for the PO_Candidate class. Author: Caleb Otto.
*/


POCandidate::POCandidate(int numCandidates) {
    workingVotes = 0;
    party = 'X';
    gainedVotes = std::vector<int>(numCandidates);
    for (int i = 0; i < numCandidates; i++) {
        gainedVotes[i] = 0;
    }
    index = -1;
    initialFirstVotes = 0;
    name = "";
}


int POCandidate::getWorkingVotes() {
    return workingVotes;
}

int POCandidate::getIndex() {
    return index;
}

void POCandidate::setIndex(int newIdx) {
    index = newIdx;
}

int POCandidate::getInitialFirstVotes() {
    return initialFirstVotes;
}

void POCandidate::incInitialFirstVotes(int votes) {
    initialFirstVotes += votes;
}

POCandidate::~POCandidate() {
    //delete[] gainedVotes;
}

void POCandidate::setParty(char newparty) {
    party = newparty;
}

char POCandidate::getParty() {
    return party;
}

void POCandidate::setWorkingVotes(int newWorkingVotes) {
    workingVotes = newWorkingVotes;
}