#include "CPL.h"

#include <cstdlib>
#include <ctime>

void CPL::seatLottery(std::vector<bool>* allowed, Party source, int extraSeats, bool isRandom) {
    if(isRandom) {
        std::srand(std::time(nullptr));
        while(extraSeats > 0) {
            int num = std::rand() % allowed->size();
            if((*allowed)[num]) {
                parties[num].setFirstSeats(parties[num].getFirstSeats() + 1);
                extraSeats--;
                audit->writeLottery(source, parties[num]);
            }
        }
    } else {
        while(extraSeats > 0) {
            int num = 0;
            if((*allowed)[num]) {
                parties[num].setFirstSeats(parties[num].getFirstSeats() + 1);
                extraSeats--;
                audit->writeLottery(source, parties[num]);
            }
            num++;
        }
    }
}

void CPL::firstAllocation(std::vector<bool>* allowed, int quota) {
    for(int i = 0; i < numParties; i++) {
        int seats = parties[i].getNumVotes() / quota;
        parties[i].setLeftoverVotes(parties[i].getNumVotes() % quota);
        int numCandidates = parties[i].getNumCandidates();
        if(seats > numCandidates) {
            parties[i].setFirstSeats(numCandidates);
            (*allowed)[i] = false;
            seatLottery(allowed, parties[i], seats - numCandidates);
        } else {
            parties[i].setFirstSeats(seats);
        }
        seatsLeft -= seats;
    }
    audit->writeFirstAllocation(parties, quota);
}

void CPL::secondAllocation(std::vector<bool>* allowed, bool isRandom) {
    std::vector<int> remainingVotes(numParties, 0);
    for(int i = 0; i < numParties; i++) {
        remainingVotes[i] = parties[i].getLeftoverVotes();
    }
    std::vector<ElectionEntity*> needsTiebreaking;
    while(seatsLeft > 0) {
        int maxRemain = 0;
        int index = 0;
        for(int i = 0; i < numParties; i++) {
            if((*allowed)[i]) {
                if(remainingVotes[i] > maxRemain) {
                    maxRemain = remainingVotes[i];
                    index = i;
                } else if(remainingVotes[i] == maxRemain && maxRemain != 0) {
                    needsTiebreaking.push_back(&parties[i]);
                }
            }
        }
        needsTiebreaking.push_back(&parties[index]);
        if(needsTiebreaking.size() > 1) {
            std::vector<ElectionEntity *> winners = breakTie(needsTiebreaking, seatsLeft, isRandom);
            audit->writeTiebreaker(needsTiebreaking, winners);
            for(int i = 0; i < winners.size(); i++) {
                seatsLeft--;
                static_cast<Party *>(winners[i])->setSecondSeats(1);
            }
        } else {
            seatsLeft--;
            parties[index].setSecondSeats(1);
            (*allowed)[index] = false;
            needsTiebreaking.clear();
        }
    }
    audit->writeSecondAllocation(parties);
}

void CPL::printResults() {
    for(int i = 0; i < numParties; i++) {
        std::cout << "Results for the ";
        std::cout << parties[i].getName();
        std::cout << " party are:\n";
        std::cout << "The candidates for the ";
        std::cout << parties[i].getName();
        std::cout << " party are: ";
        std::cout << parties[i].getCandidates() << "\n";
        std::cout << "Votes: " << parties[i].getNumVotes() << "\n";
        std::cout << "First Allocation of Seats: " << parties[i].getFirstSeats() << ".\n";
        std::cout << "Remaining Votes: " << parties[i].getLeftoverVotes() << ".\n";
        std::cout << "Second Allocation of Seats: " << parties[i].getSecondSeats() << ".\n";
        int total = parties[i].getFirstSeats() + parties[i].getSecondSeats();
        std::cout << "Final Seat Total: " << total << ".\n";
        std::cout << "% of votes to % of seats: " 
                  << 100*parties[i].getNumVotes()/numBallots << "% / "
                  << 100*total/totalSeats << "%\n\n";
    }
}

void CPL::calculateWinners() {
    std::vector<bool>* allowed = new std::vector<bool>(numParties, true);
    audit->writeInitialVotes(parties);
    int totalVotes = 0;
    for(int i = 0; i < numParties; i++) {
        totalVotes += parties[i].getNumVotes();
    }
    int quota = totalVotes / totalSeats;
    audit->writeQuota(quota);
    // First allocation
    firstAllocation(allowed, quota);
    // Second allocation
    if(seatsLeft > 0) {
        secondAllocation(allowed);
    }
    audit->writeFinalResults(parties);
    printResults();
    delete allowed;
}
