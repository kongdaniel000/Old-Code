#include "IRV.h"
#include "IRVAudit.h"
#include <iostream>
#include <iomanip>
#include <string.h>
#include <sstream>
#include <vector>
#include <bits/stdc++.h>
using namespace std;

/**
* @brief File name: IRV.cc. Description: Contains method definitons for the IRV class,
* which contains IRV processes. Author: Jacob.
*
*/

void IRV::calculateWinners() {
    Candidate* winner = nullptr;
    int stillIn = numCandidates;                              // number of candidates not eliminated
    int numLoop = 0;
    vector<bool> stillRunning(numCandidates, true);           // array telling which candidates are not eliminated
    fillFirstPlaceVotes();                                    // find all candidates first place votes
    while (winner == nullptr) {
        if (stillIn == 2) {                                   // if only 2 candidates remain, find winner
            winner = getWinnerTwoLeft(stillRunning);
            break;
        }
        else {                                                                  // otherwise, find max vote getter, return them if majority
            winner = maxCandWinCheck(stillRunning);
            if (winner != nullptr) {                                            // if the candidate has a majority they win
                break;
            }
            else {                                                             // otherwise we must redistribute
                Candidate loser = getLoser(stillRunning);
                //cout << "Loser is " << loser.getName() << std::endl;
                stillRunning[loser.getIndex()] = false;                       // loser is eliminated
                stillIn--;
                redistribute(loser, stillRunning, numLoop);                   // redistribute losers votes       
                for (int i = numLoop; i < candidates.at(loser.getIndex()).getGainedVotes().size(); i++) {          
                    candidates.at(loser.getIndex()).setGainedVotes(i);
                }  
                numLoop++;
            }
        }
    }
    cout << "Winner is: " << winner->getName() << endl;
    printResults(*winner, numLoop);                                                              // print results to the screen
    int total = 0;
    for (int i = 0; i < numCandidates; i++) {                                                   // find total number of votes
        if (stillRunning[i]) {
            total += candidates[i].getWorkingVotes();
        }
    }
    (static_cast<IRVAudit*>(audit))->writeFinalResults(*winner, winner->getWorkingVotes() / (float) total);         // write final results to audit file
    
    for (int i = 0; i < ballotList->size(); i++) {                                               // clear all BallotNodes
        ballotList->at(i).cleanup(); 
    }                                  
    return;
}

void IRV::redistribute(Candidate loser, vector<bool> inRunning, int nLoop) {
    int original = loser.getWorkingVotes();
    int redisted = 0;
    for (int i = 0; i < ballotList->size(); i++) {                              // redistribute loser votes to other candidates
        if (ballotList->at(i).getHead() == loser.getIndex()) {     
            bool done = false;
            while (!done) {
                ballotList->at(i).removeHead();                                 // remove head of the ballot list until it is a candidate still in the running
                if (ballotList->at(i).getHead() == -1) {done = true;}
                else if (inRunning[ballotList->at(i).getHead()]) {
                    done = true;
                    //cout << "Increasing votes for: " << candidates[ballotList->at(i).getHead()].getName() << endl;
                    candidates[ballotList->at(i).getHead()].incWorkingVotes();            // give candidate who received the vote a working vote
                    candidates[ballotList->at(i).getHead()].incGainedVotes(nLoop);        // increase the gained votes for this candidate for this redistribution
                    redisted++;
                }
            }
        }
    }
    (static_cast<IRVAudit*>(audit))->writeVoteTransfer(candidates, inRunning, loser, nLoop);              // document the vote transfer in the audit file 
    exhausted[nLoop] = original - redisted;                                                               // get number of exhausted votes
    return;
}

Candidate IRV::getLoser(vector<bool> inRunning) {
    vector<ElectionEntity *> losers;
    int min = INT_MAX;
    Candidate* minCand = nullptr;
    for (int i = 0; i < numCandidates; i++) {
        if (inRunning[i] && candidates[i].getWorkingVotes() < min) {      // loser is candidate with fewest votes
            losers.clear();
            min = candidates[i].getWorkingVotes();
            minCand = &candidates[i];
            losers.push_back(&candidates[i]);
        }
        else if (inRunning[i] && candidates[i].getWorkingVotes() == min) {        // if another candidate has the fewest, they are also a "loser"
            losers.push_back(&candidates[i]);
        }
    }
    if (losers.size() > 1) {                                      // if more than one loser, break the tie to find true loser
        vector<ElectionEntity *> tieWinner; 
        tieWinner = breakTie(losers, 1);                          // "winner" of the tie is loser to be redistributed
        audit->writeTieLose(losers, tieWinner[0]);         
        losers.clear(); 
        losers.push_back(tieWinner[0]);                           // loser vector now only has true loser
    }
    return *(static_cast<Candidate*>(losers[0]));
}

Candidate* IRV::maxCandWinCheck(vector<bool> inRunning) {
    Candidate* maxCand = nullptr;
    int max = 0;
    for (int i = 0; i < numCandidates; i++) {
        if (inRunning[i] && candidates[i].getWorkingVotes() > max) {
            max = candidates[i].getWorkingVotes();
            maxCand = &candidates[i];
        }
    }
    if ((maxCand->getWorkingVotes() / (float) totalVotes) > 0.5) {    // if the candidate has a majority they win
        return maxCand;
    }
    return nullptr;                                                    // the candidate with most votes did not win
}

Candidate* IRV::getWinnerTwoLeft(vector<bool> inRunning) {
    int votes1 = 0;   
    int votes2 = 0;
    int idx1 = 0;
    int idx2 = 0;
    Candidate* winner = nullptr;
    for (int i = 0; i < numCandidates; i++) {         // determine the two candidates still running, and their working votes
        if (inRunning[i] && votes1 != 0) {
            votes2 = candidates[i].getWorkingVotes();
            idx2 = i;
        }
        else if (inRunning[i]) {
            votes1 = candidates[i].getWorkingVotes();
            idx1 = i;
        }
    }
    if (votes1 == votes2) {                                 // two remaining have same working votes, break the tie
        vector<ElectionEntity*> tieArr;
        tieArr.push_back(&candidates[idx1]);
        tieArr.push_back(&candidates[idx2]);
        vector<ElectionEntity*> tieWin = breakTie(tieArr, 1);
        audit->writeTieLead(tieArr, tieWin[0]);                       // record the tie in audit file
        winner = static_cast<Candidate*>(tieWin[0]);                // will need to free this array ig
        return winner;                                                      // winner has been found
    }
    else {
        if (votes1 > votes2) {                                             // otherwise winner goes by majority
            winner = &candidates[idx1];
            return winner;
        }
        else {
            winner = &candidates[idx2];
            return winner;
        }
    }
}

void IRV::fillFirstPlaceVotes() {
    int total = 0;
    for (int i = 0; i < ballotList->size(); i++) {
        if (ballotList->at(i).getHead() != -1) {                                // iterate through each ballot
            candidates[ballotList->at(i).getHead()].incWorkingVotes();          // give candidate a working vote
            candidates[ballotList->at(i).getHead()].incInitialFirstVotes();     // give candidate an initial first place vote
            total++;
        }
    }
    totalVotes = total;
}

void IRV::printResults(Candidate winner, int redists) {   // check working votes situation
    for (int i = 0; i < numCandidates; i++) {
        candidates[i].setWorkingVotes(candidates[i].getInitialFirstVotes());                               // reset all candidates working votes to first place votes
    }
    cout << left << setw(30) << "Candidates & Parties" << left << setw(20) << "First Place Votes";         // set up first two column headers
    for (int i = 0; i < redists; i++) {                                                                    // add column header for each vote redistribution
        cout << "Transfer " << left << setw(4) << i+1 << left << setw(13) << "New totals";            
    }
    cout << endl;                                                                                          // move on to next row
    ostringstream s;
    for (int i = 0; i < numCandidates; i++) {
        s.str("");
        if (candidates[i].getName().compare(winner.getName()) == 0) {                                      // add asterisk in front of name if winner
            s << "*";
        }  
        s << candidates[i].getName();     
        //cout << s.str(); 
        cout << left << setw(30) << s.str();
        cout << left << setw(20) << candidates[i].getInitialFirstVotes();                                  // print candidates original first place votes
        for (int j = 0; j < redists; j++) {                                                                // loop to print out each vote redistribution for a candidate
            s.str("");
            if ((candidates[i].getGainedVotes()[j] == -1 && j != 0 && candidates[i].getGainedVotes()[j - 1] != -1) || (j == 0 && candidates[i].getGainedVotes()[j] == -1)) {    // if the candidate is eliminated this round, print how many votes they lost
                s << "-" << candidates[i].getWorkingVotes();
                cout << left << setw(13) << s.str();
                cout << left << setw(13) << "----------";
            }
            else if (candidates[i].getGainedVotes()[j] == -1) {                                              // if the candidate was/is eliminated, they did not gain any votes
                cout << left << setw(13) << "----------";
                cout << left << setw(13) << "----------";
            }
            else {
                s << "+" << candidates[i].getGainedVotes()[j];                                               // print number of votes that candidate gained
                cout << left << setw(13) << s.str();
                candidates[i].incWorkingVotes(candidates[i].getGainedVotes()[j]);                            // increasae the candidates working votes by the number they gained
                cout << left << setw(13) << candidates[i].getWorkingVotes();                                 // print new total of working votes
            }
        }
        cout << endl;                                                                                        // move on to next line (next candidate)
    }
    cout << endl;
    int totExhaust = 0;                                                   
    cout << left << setw(30) << "EXHAUSTED PILE";
    cout << left << setw(20) << "0";                                                        // all ballots have at least 1 candidate ranked
    for (int i = 0; i < redists; i++) {                                                     // print out how many are exhausted per redistribution
        s.str("");
        s << "+" << exhausted[i];
        totExhaust += exhausted[i];
        cout << left << setw(13) << s.str();
        cout << left << setw(13) << totExhaust;
    }
    cout << endl;
    cout << left << setw(30) << "TOTALS" << left << setw(20) << totalVotes << endl;         // print out total number of votes
    return;
}

vector<Candidate> IRV::getCandidates() {
    return candidates;
}

void IRV::setCandidates(vector<Candidate> candidateArray) {
    candidates = candidateArray;
    return;
}

void IRV::setBallotList(vector<BallotLinkedList>* linkedList) {
    ballotList = linkedList;
    return;
}

vector<BallotLinkedList>* IRV::getBallotList() {
    return ballotList;
}

void IRV::setNumCandidates(int sum) {
    numCandidates = sum;
    return;
}

void IRV::setRemainCandidates(int newRemainCandidates) {
    remainCandidates = newRemainCandidates;
    return;
}

int IRV::getNumCandidates() {
    return numCandidates;
}

int IRV::getRemainCandidates() {
    return remainCandidates;
}

void IRV::setExhausted(vector<int> exhaust) {
    exhausted = exhaust;
}

void IRV::setTotal(int total) {
    totalVotes = total;
}

std::vector<int> IRV::getExhausted() {
    return exhausted;
}

int IRV::getTotal() {
    return totalVotes;
}