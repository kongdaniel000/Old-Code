#include "IRVAudit.h"

/**
 * @brief File name: IRVAudit.cpp. Description: Contains method definitions
 * for the IRVAudit class, which defines the creation and writing to of the
 * IRV Audit file by inheriting from the ElectionAudit class.
 * Author: Caleb Otto.
*/

void IRVAudit::writeMetadata(IRV* object){
    *auditFile << "The election type is: IRV\n";
    std::vector<Candidate> candidates = object->getCandidates();
    *auditFile << "The number of candidates are: " << object->getNumCandidates() << "\n";
    for(int i = 0; i < candidates.size(); i++) {
        *auditFile << "Candidate " << (i+1) << " is " << candidates[i].getName() << "\n";
    }
    *auditFile << "The number of ballots are: " << object->getNumBallots() << ".\n";
}

void IRVAudit::writeFinalResults(Candidate winner, float percentage){
    *auditFile << "The final results have " << winner.getName() << " as the winning candidate with a vote majority of " << percentage << "\n";
    (*auditFile).close();
}

void IRVAudit::writeBallot(std::vector<int> votes, std::vector<Candidate> candidates){
    *auditFile << "Ballot data: \n";
    for(int i =0; i< votes.size(); i++){
        if(votes[i] != -1) {
            *auditFile << "Voted for " << candidates.at(votes.at(i)).getName() << " as their " << i+1 << " choice." << "\n";
        }
    }
}

void IRVAudit::writeVoteTransfer(std::vector<Candidate> candidates, std::vector<bool> running, Candidate eliminated, int redist){
    *auditFile << eliminated.getName() << " was eliminated and their votes distributed\n";
    for(int i = 0; i < candidates.size(); i++){
        if (running.at(candidates.at(i).getIndex())) {
            *auditFile << candidates.at(i).getName() << " had " << candidates.at(i).getGainedVotes().at(redist) << " votes distributed to them\n";
        }
    }
}

void IRVAudit::writeTieLead(std::vector<ElectionEntity*> original, ElectionEntity* winner){
    for (int i = 0; i < original.size(); i++){
        if (winner->getName() == original.at(i)->getName()){
            *auditFile << winner->getName() << " won in a tie breaker for winning the election against ";
            for(int j = 0; j<original.size(); j++){
                if(j != i){
                    *auditFile << (original.at(j)->getName()) << ", ";
                }
            }
            *auditFile << "\n";
        }
    }
}

void IRVAudit::writeTieLose(std::vector<ElectionEntity*> original, ElectionEntity* winner){
    for (int i = 0; i < original.size(); i++){
        if (winner->getName() == original.at(i)->getName()){
            *auditFile << winner->getName() << " lost in a tie breaker for lowest votes against ";
            for(int j = 0; j<original.size(); j++){
                if(j != i){
                    *auditFile << (original.at(j)->getName()) << ", ";
                }
            }
            *auditFile << "\n";
        }
    }
}


