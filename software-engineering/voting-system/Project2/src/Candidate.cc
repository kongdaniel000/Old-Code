#include "Candidate.h"
#include <vector>

/**
 * @brief File name: Candidate.cc. Description: Contains attributes and method definitions
 * for the Candidate class. Author: Ruolei Zeng.
*/


Candidate::Candidate(int numCandidates) {
    workingVotes = 0;
    gainedVotes = std::vector<int>(numCandidates);
    for (int i = 0; i < numCandidates; i++) {
        gainedVotes[i] = 0;
    }
    index = -1;
    initialFirstVotes = 0;
    name = "";
}

void Candidate::incWorkingVotes(int amt) {
    workingVotes += amt;
}

int Candidate::getWorkingVotes() {
    return workingVotes;
}

int Candidate::getIndex() {
    return index;
}

void Candidate::setIndex(int newIdx) {
    index = newIdx;
}

void Candidate::incGainedVotes(int roundIndex) {
    gainedVotes[roundIndex]++;
}

int Candidate::getInitialFirstVotes() {
    return initialFirstVotes;
}

void Candidate::incInitialFirstVotes(int votes) {
    initialFirstVotes += votes;
}

std::vector<int> Candidate::getGainedVotes() {
    return gainedVotes;
}

void Candidate::setGainedVotes(int index) {
    if (index < 0 || index >= gainedVotes.size()) {
        return;
    }
    gainedVotes.at(index) = -1;
}

Candidate::~Candidate() {
    //delete[] gainedVotes;
}

void Candidate::setWorkingVotes(int newWorkingVotes) {
    workingVotes = newWorkingVotes;
}
