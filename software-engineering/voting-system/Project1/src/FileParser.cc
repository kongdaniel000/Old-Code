#include "FileParser.h"
#include "BallotLinkedList.h"

int FileParser::openFile(std::string filename) {
    if(file.good()){
        file.close();
    }
    file.open(filename);
    if (file.fail()) { //If input file cannot be read, reprompt the user for a better filename
        std::cout << "Input file failed to open, program ending. \n";
        return 1;
    }
    return 0;
}

std::vector<int> FileParser::ballotToArray(std::string ballot, int numCandidates) {
    std::vector<int> initialArray(numCandidates, -1);
    int candidatesAdded = 0;
    std::string delimiter = ",";
    while(candidatesAdded < numCandidates) {
        std::string ballotValue = ballot.substr(0, ballot.find(delimiter));
        if(ballotValue == "") {
            ballotValue = "-1";
        }
        initialArray[candidatesAdded] = std::stoi(ballotValue);
        ballot = ballot.substr(ballot.find(delimiter)+1, ballot.length());
        candidatesAdded++;
    }
    std::vector<int> translatedArray(numCandidates, -1);
    for(int j = 0; j < numCandidates; j++){
        if(initialArray[j] != -1) {
            translatedArray[initialArray[j] - 1] = j;
        }
    }
    return translatedArray;
}

void FileParser::readBallots(CPL* election) {
    std::vector<Party> parties = election->getParties();
    std::string stringStore;
    while(std::getline(file, stringStore)) {
        int i = 0;
        int currentParty = 0;
        while(i < parties.size()) {
            if(stringStore[i] == '1') {
                parties[currentParty].incrementVotes();
                election->getAudit()->writeBallot((parties)[currentParty]);
                break;
            } else {
                currentParty++;
            }
            i++;
        }
    }
    election->setParties(parties);
}

void FileParser::readBallots(IRV* election) {
    std::vector<BallotLinkedList>* listOfLinkedLists = new std::vector<BallotLinkedList>;
    election->setBallotList(listOfLinkedLists);
    std::string stringStore;
    while(std::getline(file, stringStore)) {
        BallotLinkedList* newBLL = new BallotLinkedList();
        std::vector<int> ballotArray = ballotToArray(stringStore, election->getNumCandidates());
        election->getAudit()->writeBallot(ballotArray, election->getCandidates());
        newBLL->populateList(ballotArray);
        (election->getBallotList())->push_back(*newBLL);
    }
}

std::string FileParser::getDay() {
    std::string day;
    bool isNum = true;
    bool inRange = false;
    do {
        isNum = true;
        inRange = false;
        std::cout << "Please give the current day in dd format. Example: \"12\".\n";
        std::cin >> day;
        for(int i = 0; i < day.length(); i++) {
            if(!std::isdigit(day[i])) {
                isNum = false;
            }
        }
        if(isNum) {
            if(day.length() < 2) {
                std::cout << "Not enough digits! (Expected 2)\n";
            } else if(day.length() > 2) {
                std::cout << "Too many digits! (Expected 2)\n";
            } else {
                if(1 <= std::stoi(day) && std::stoi(day) <= 31) {
                    inRange = true;
                }
                if(!inRange) {
                    std::cout << "Day is out of range! (1-31)\n";
                }
            }
        } else {
            std::cout << "Input is not a number!\n";
        }
    } while(!isNum || !inRange);
    return day;
}

std::string FileParser::getMonth() {
    std::string month;
    bool isNum = true;
    bool inRange = false;
    do {
        isNum = true;
        inRange = false;
        std::cout << "Please give the current month in mm format. Example: \"07\".\n";
        std::cin >> month;
        for(int i = 0; i < month.length(); i++) {
            if(!std::isdigit(month[i])) {
                isNum = false;
            }
        }
        if(isNum) {
            if(month.length() < 2) {
                std::cout << "Not enough digits! (Expected 2)\n";
            } else if(month.length() > 2) {
                std::cout << "Too many digits! (Expected 2)\n";
            } else {
                if(1 <= std::stoi(month) && std::stoi(month) <= 12) {
                    inRange = true;
                }
                if(!inRange) {
                    std::cout << "Month is out of range! (1-12)\n";
                }
            }
        } else {
            std::cout << "Input is not a number!\n";
        }
    } while(!isNum || !inRange);
    return month;
}

std::string FileParser::getYear() {
    std::string year;
    bool isNum = true;
    do {
        isNum = true;
        std::cout << "Please give the current year in yyyy format. Example: \"2001\".\n";
        std::cin >> year;
        for(int i = 0; i < year.length(); i++) {
            if(!std::isdigit(year[i])) {
                isNum = false;
            }
        }
        if(isNum) {
            if(year.length() < 4) {
                std::cout << "Not enough digits! (Expected 4)\n";
                isNum = false;
            } else if (year.length() > 4) {
                std::cout << "Too many digits! (Expected 4)\n";
                isNum = false;
            }
        }
        else {
            std::cout << "Input is not a number!\n";
        }
    } while(!isNum);
    std::cout << "\n";
    return year;
}

IRV* FileParser::handleIRV(std::string day, std::string month, std::string year) {
    IRV* IRElection = new IRV();
    IRVAudit* audit = new IRVAudit();
    std::string fname = "./misc/audits/IRVAudit_" + day + "_" + month + "_" + year + ".txt";
    std::ofstream* stream = new std::ofstream(fname, std::ios::out);
    audit->setAuditFile(stream);
    IRElection->setAudit(audit);
    std::string stringStore;
    std::getline(file, stringStore);
    int numCandidates = std::stoi(stringStore);
    IRElection->setNumCandidates(numCandidates);
    std::vector<Candidate> candidates;
    for(int i = 0; i < numCandidates; i++) {
        Candidate* newCan = new Candidate(numCandidates);
        newCan->setIndex(i);
        candidates.push_back(*newCan);
    }
    std::string candidateString;
    std::string candidateName;
    std::string delimiter = ",";
    std::getline(file, candidateString);
    for(int i = 0; i < numCandidates; i++) {
        if(i < numCandidates - 1) {
            candidateName = candidateString.substr(0, candidateString.find(delimiter));
            candidates[i].setName(candidateName);
            candidateString = candidateString.substr(candidateString.find(delimiter)+2, candidateString.length());
        } else {
            candidates[i].setName(candidateString);
        }
    }
    IRElection->setCandidates(candidates);
    std::getline(file, stringStore);
    int numBallots = std::stoi(stringStore);
    IRElection->setNumBallots(numBallots);
    audit->writeMetadata(IRElection);
    readBallots(IRElection);
    return IRElection;
}

CPL* FileParser::handleCPL(std::string day, std::string month, std::string year) {
    CPL* CPLElection = new CPL();
    CPLAudit* audit = new CPLAudit();
    std::string fname = "./misc/audits/CPLAudit_" + day + "_" + month + "_" + year + ".txt";
    std::ofstream* stream = new std::ofstream(fname, std::ios::out);
    audit->setAuditFile(stream);
    CPLElection->setAudit(audit);
    std::string stringStore;
    std::getline(file, stringStore);
    int numParties = std::stoi(stringStore);
    CPLElection->setNumParties(numParties);
    std::vector<Party> parties(numParties);
    std::string partyString;
    std::string partyName;
    std::string delimiter = ",";
    std::getline(file, partyString);
    for(int i = 0; i < numParties; i++) {
        if(i < numParties - 1) {
            partyName = partyString.substr(0, partyString.find(delimiter));
            parties[i].setName(partyName);
            partyString = partyString.substr(partyString.find(delimiter)+2, partyString.length());
        } else {
            parties[i].setName(partyString);
        }
    }
    for(int i = 0; i < numParties; i++) {
        std::string candidates;
        std::getline(file, candidates);
        parties[i].setCandidates(candidates);
    }
    CPLElection->setParties(parties);
    std::getline(file, stringStore);
    int numSeats = std::stoi(stringStore);
    CPLElection->setSeats(numSeats);
    std::getline(file, stringStore);
    int numBallots = std::stoi(stringStore);
    CPLElection->setNumBallots(numBallots);
    audit->writeMetadata(CPLElection);
    readBallots(CPLElection);
    return CPLElection;
}

ElectionObject* FileParser::getMetadata() {
    std::string day = getDay();
    std::string month = getMonth();
    std::string year = getYear();
    std::string stringStore;
    std::getline(file, stringStore);
    if(stringStore == "IR") {
        IRV* irv = handleIRV(day, month, year);
        return irv;
    } else if(stringStore == "CPL") {
        CPL* cpl = handleCPL(day, month, year);
        return cpl;
    }
    throw std::invalid_argument("Invalid election type!\n");
}