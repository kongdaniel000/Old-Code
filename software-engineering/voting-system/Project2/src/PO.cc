#include "PO.h"
#include <iostream>
#include <iomanip>
#include <string.h>
#include <sstream>
#include <vector>
#include <bits/stdc++.h>
using namespace std;

/**
* @brief File name: PO.cc. Description: Contains method definitons for the PO class,
* which contains PO processes. Author: Caleb Otto.
*
*/

void PO::calculateWinners() {
    POCandidate* winner = nullptr;
    std::vector<ElectionEntity*> tiebreakers;
    int maxVote = 0;
    int currWinIndex = 0;
    int stillIn = numCandidates;                              // number of candidates not eliminated
    int numLoop = 0;     // array telling which candidates are not eliminated
    for(int i=0; i< candidates.size(); i++){
        if(candidates.at(1).getInitialFirstVotes() > maxVote ){
            winner = &candidates.at(i);
            maxVote = candidates.at(i).getInitialFirstVotes();
            currWinIndex = i;
            tiebreakers.clear();
        }
        else if(candidates.at(i).getInitialFirstVotes() == maxVote){
            tiebreakers.push_back(&candidates.at(currWinIndex));
            tiebreakers.push_back(&candidates.at(i));
        }
    }
    if(tiebreakers.size() == 0) {
        cout << "Winner is: " << winner->getName() << " from party: " << winner->getParty() << endl;
    }
    else{
        cout << "TIEBREAKER CALLED, contestants are: " << endl;
        for(int i =0; i<tiebreakers.size(); i++) {
            cout << tiebreakers[i]->getName() << endl;
        }
        std::vector<ElectionEntity*> winnerboi = breakTie(tiebreakers, 1, true);
        winner = static_cast<POCandidate *>(winnerboi[0]);
        cout << "Winner of tiebreaker is: " << winner->getName() << " from party: " << winner->getParty() << endl;
    }
    cout << endl;
    cout << "All candidates, their parties, and their votes are below: " << endl;
    for(int i = 0; i < candidates.size(); i++){
        cout << candidates[i].getName() << " Party: " << candidates[i].getParty() << " Votes: " << candidates[i].getInitialFirstVotes() << endl;
    }
    return;
}

vector<POCandidate> PO::getCandidates() {
    return candidates;
}

void PO::setCandidates(vector<POCandidate> candidateArray) {
    candidates = candidateArray;
    return;
}

void PO::setNumCandidates(int sum) {
    numCandidates = sum;
    return;
}

int PO::getNumCandidates() {
    return numCandidates;
}
