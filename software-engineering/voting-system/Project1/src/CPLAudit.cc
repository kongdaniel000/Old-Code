#include <fstream>
#include <string>

#include "CPLAudit.h"
#include "CPL.h"

/**
 * @brief *auditFile name: CPLAudit.cpp. Description: Contains method definitions
 * for the CPLAudit class, which defines the creation and writing to of the
 * CPL Audit *auditFile by inheriting from the ElectionAudit class.
 * Author: Caleb Otto.
*/


void CPLAudit::writeMetadata(CPL* object){
    *auditFile << "The election type is: CPL\n";
    std::vector<Party> parties = object->getParties();
    *auditFile << "The number of parties are: " << object->getNumParties() << "\n";
    for(int i = 0; i < parties.size(); i++) {
        *auditFile << "Candidates for the " << parties[i].getName() << " party are: " << parties[i].getCandidates() << "\n";
    }
    *auditFile << "The number of seats are: " << object->getSeats() 
           << " and the number of ballots are: " << object->getNumBallots() << ".\n";
}

void CPLAudit::writeInitialVotes(std::vector<Party> parties){
    for(int i=0; i<parties.size(); i++){
        *auditFile << parties[i].getName() << " had " << parties[i].getNumVotes() << " total votes.\n";
    }
}

void CPLAudit::writeFinalResults(std::vector<Party> parties){
    *auditFile << "The final results were as follows: " << "\n";
    int seats;
    for(int i=0; i<parties.size(); i++){
        seats = parties[i].getFirstSeats() + parties[i].getSecondSeats();
        *auditFile << parties[i].getName() << " earned " << seats << " seats." << "\n";
        if(seats > 0){
            *auditFile << "Their winners were: ";
            std::string candidateString = parties[i].getCandidates();
            std::string candidateName;
            std::string delimiter = ",";
            bool moreThanOne = true;
            if(seats == 1) {
                moreThanOne = false;
            }
            for(int j = 0; j<seats; j++){
                if(!moreThanOne) {
                    candidateName = candidateString.substr(0, candidateString.find(delimiter));
                    *auditFile << candidateName << "\n";
                }
                else if(j < seats -1) {
                    candidateName = candidateString.substr(0, candidateString.find(delimiter));
                    *auditFile << candidateName << ", ";
                    candidateString = candidateString.substr(candidateString.find(delimiter)+2, candidateString.length());
                } else {
                    *auditFile << candidateString << "\n";
                }
            }
        }
    }
    auditFile->close();
}

void CPLAudit::writeFirstAllocation(std::vector<Party> parties, int quota){
    for(int i=0; i < parties.size(); i++){
        *auditFile << parties[i].getName() << " received " << parties[i].getNumVotes() << " votes ";
        *auditFile << "and earned " << parties[i].getFirstSeats() << " seats.\n";
    }
}

void CPLAudit::writeSecondAllocation(std::vector<Party> parties){
    for(int i=0; i < parties.size(); i++){
        *auditFile << parties[i].getName() << " had " << parties[i].getLeftoverVotes() << " remainder votes ";
        *auditFile << "and earned " << parties[i].getSecondSeats() << " more seats" << "\n";
    }
}

void CPLAudit::writeLottery(Party source, Party destination){
    *auditFile << "Extra seat lotteried from " << source.getName() << " and given to " << destination.getName() << "\n";
}

void CPLAudit::writeBallot(Party party){
    *auditFile << "Ballot for " << party.getName() << "\n";
}

void CPLAudit::writeTiebreaker(std::vector<ElectionEntity *> original, std::vector<ElectionEntity *> winners) {
    *auditFile << "Tiebreaker chose the following winners: ";
    for(int i=0; i<winners.size(); i++) {
        *auditFile << winners[i]->getName();
        if(i < winners.size() - 1) {
            *auditFile << ", ";
        } else {
            *auditFile << ",\n";
        }
    }
    *auditFile << "Selected from the following parties: ";
    for(int i=0; i< original.size(); i++){
        *auditFile << original[i]->getName();
        if(i < original.size() - 1) {
            *auditFile << ", ";
        } else {
            *auditFile << ".\n";
        }
    }
}

void CPLAudit::writeQuota(int quota) {
    *auditFile << "The quota was: " << quota << ".\n";
}