#include "FileParser.h"
#include "BallotLinkedList.h"
#include <bits/stdc++.h>
using namespace std;

FileParser::FileParser() {
    std::ifstream* initialStream = new std::ifstream;
    files.push_back(initialStream);
}

FileParser::~FileParser() {
    for(int i = 0; i < files.size(); i++) {
        files[i]->close();
        delete files[i];
    }
}

int FileParser::openFile(std::string filename, int index) {
    if(files[index]->good()){
        files[index]->close();
    }
    bool csvFail = filename.length() < 4 || filename.substr(filename.length() - 4) != ".csv";
    files[index]->open(filename);
    bool openFail = files[index]->fail(); //If input file cannot be read, reprompt the user for a better filename
    while(csvFail || openFail) {
        if(csvFail) {
            std::cout << "File: " + filename + " is not in .csv format. Please type the filename for this file again.\n";
        } else if(openFail) {
            std::cout << "Input file " + filename + " failed to open, please type the filename for this file again. \n";
        }
        std::cin >> filename;
        csvFail = filename.length() < 4 || filename.substr(filename.length() - 4) != ".csv";
        if(!csvFail) {
            files[index]->open(filename);
            openFail = files[index]->fail();
        }
    }
    return 0;
}

int FileParser::openMultiple(std::vector<std::string> filenames) {
    for(int i = 0; i < filenames.size(); i++) {
        // std::cout << filenames[i] << "\n";
        if(i > 0) { // since an initial ifstream is already there
            std::ifstream* newStream = new std::ifstream;
            files.push_back(newStream);
        }
        if(openFile(filenames[i], i)) {
            return 1; // this shouldn't happen anymore, might remove later 
        }
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
    for(int i = 0; i < files.size(); i++) {
        while(std::getline(*(files[i]), stringStore)) {
            int j = 0;
            int currentParty = 0;
            while(j < parties.size()) {
                if(stringStore[j] == '1') {
                    parties[currentParty].incrementVotes();
                    election->getAudit()->writeBallot((parties)[currentParty]);
                    break;
                } else {
                    currentParty++;
                }
                j++;
            }
        }
    }
    election->setParties(parties);

}

void FileParser::readBallots(IRV* election) {
    std::vector<BallotLinkedList>* listOfLinkedLists = new std::vector<BallotLinkedList>;
    election->setBallotList(listOfLinkedLists);
    std::string stringStore;
    for(int i = 0; i < files.size(); i++) {
        while(std::getline(*(files[i]), stringStore)) {
            BallotLinkedList* newBLL = new BallotLinkedList();
            std::vector<int> ballotArray = ballotToArray(stringStore, election->getNumCandidates());
            election->getAudit()->writeBallot(ballotArray, election->getCandidates());
            newBLL->populateList(ballotArray);
            (election->getBallotList())->push_back(*newBLL);
        }
    }
}

//TODO HANDLE READING PO BALLOTS
void FileParser::readBallots(PO* election) {
    std::vector<POCandidate> candidates = election->getCandidates();
    std::string stringStore;
    while(std::getline(*(files[0]), stringStore)) {
        int i = 0;
        int currentParty = 0;
        while(i < candidates.size()) {
            if(stringStore[i] == '1') {
                candidates[currentParty].incInitialFirstVotes();
//                election->getAudit()->writeBallot((parties)[currentParty]);
                break;
            } else {
                currentParty++;
            }
            i++;
        }
    }
    election->setCandidates(candidates);
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
                if(1 <= std::stoi(day)) {
                    inRange = true;
                    if(!inRange) {
                        std::cout << "Day must be a positive number!\n";
                    }
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
    std::getline(*(files[0]), stringStore);
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
    std::getline(*(files[0]), candidateString);
    for(int i = 0; i < numCandidates; i++) {
        if(i < numCandidates - 1) {
            candidateName = candidateString.substr(0, candidateString.find(delimiter));
            candidates[i].setName(candidateName);
            candidateString = candidateString.substr(candidateString.find(delimiter)+2, candidateString.length());
        } else {
            candidates[i].setName(candidateString);
        }
    }
    std::vector<int> exhaust(numCandidates, 0);
    IRElection->setExhausted(exhaust);
    IRElection->setCandidates(candidates);
    std::getline(*(files[0]), stringStore);
    int numBallots = std::stoi(stringStore);
    IRElection->setNumBallots(numBallots);
    audit->writeMetadata(IRElection);
    readBallots(IRElection);
    return IRElection;
}

IRV* FileParser::handleMultipleIRV(std::string day, std::string month, std::string year) {
    IRV* IRElection = new IRV();
    IRVAudit* audit = new IRVAudit();
    std::string fname = "./misc/audits/IRVAudit_" + day + "_" + month + "_" + year + ".txt";
    std::ofstream* stream = new std::ofstream(fname, std::ios::out);
    audit->setAuditFile(stream);
    IRElection->setAudit(audit);
    std::string stringStore;
    std::getline(*(files[0]), stringStore);
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
    std::getline(*(files[0]), candidateString);
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
    std::getline(*(files[0]), stringStore);
    int numBallots = std::stoi(stringStore);
    for(int i = 1; i < files.size(); i++) {
        for(int j = 0; j < 3; j++) { // ignore the first 3 lines
            std::getline(*(files[i]), stringStore);
        }
        std::getline(*(files[i]), stringStore);
        numBallots += std::stoi(stringStore);
    }
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
    std::getline(*(files[0]), stringStore);
    int numParties = std::stoi(stringStore);
    CPLElection->setNumParties(numParties);
    std::vector<Party> parties(numParties);
    std::string partyString;
    std::string partyName;
    std::string delimiter = ",";
    std::getline(*(files[0]), partyString);
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
        std::getline(*(files[0]), candidates);
        parties[i].setCandidates(candidates);
    }
    CPLElection->setParties(parties);
    std::getline(*(files[0]), stringStore);
    int numSeats = std::stoi(stringStore);
    CPLElection->setSeats(numSeats);
    std::getline(*(files[0]), stringStore);
    int numBallots = std::stoi(stringStore);
    CPLElection->setNumBallots(numBallots);
    audit->writeMetadata(CPLElection);
    readBallots(CPLElection);
    return CPLElection;
}

CPL* FileParser::handleMultipleCPL(std::string day, std::string month, std::string year) {
    CPL* CPLElection = new CPL();
    CPLAudit* audit = new CPLAudit();
    std::string fname = "./misc/audits/CPLAudit_" + day + "_" + month + "_" + year + ".txt";
    std::ofstream* stream = new std::ofstream(fname, std::ios::out);
    audit->setAuditFile(stream);
    CPLElection->setAudit(audit);
    std::string stringStore;
    std::getline(*(files[0]), stringStore);
    int numParties = std::stoi(stringStore);
    CPLElection->setNumParties(numParties);
    std::vector<Party> parties(numParties);
    std::string partyString;
    std::string partyName;
    std::string delimiter = ",";
    std::getline(*(files[0]), partyString);
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
        std::getline(*(files[0]), candidates);
        parties[i].setCandidates(candidates);
    }
    CPLElection->setParties(parties);
    std::getline(*(files[0]), stringStore);
    int numSeats = std::stoi(stringStore);
    CPLElection->setSeats(numSeats);
    std::getline(*(files[0]), stringStore);
    int numBallots = std::stoi(stringStore);
    for(int i = 1; i < files.size(); i++) {
        for(int j = 0; j < 4 + CPLElection->getNumParties(); j++) { // ignore the first 4 lines + num parties
            std::getline(*(files[i]), stringStore);
        }
        std::getline(*(files[i]), stringStore);
        numBallots += std::stoi(stringStore);
    }
    CPLElection->setNumBallots(numBallots);
    audit->writeMetadata(CPLElection);
    readBallots(CPLElection);
    return CPLElection;
}

PO* FileParser::handlePO(std::string day, std::string month, std::string year) {
    PO* POElection = new PO();
    std::string stringStore;
    std::getline(*(files[0]), stringStore);
    int numCandidates = std::stoi(stringStore);
    POElection->setNumCandidates(numCandidates);
    std::vector<POCandidate> candidates;
    for(int i = 0; i < numCandidates; i++) {
        POCandidate* newCan = new POCandidate(numCandidates);
        newCan->setIndex(i);
        candidates.push_back(*newCan);
    }
    std::string candidateString;
    std::string candidateName;
    std::string candidateParty;
    std::string delimiter = ",";
    std::getline(*(files[0]), candidateString);
    candidateString.erase(remove(candidateString.begin(), candidateString.end(), ']'), candidateString.end());
    candidateString.erase(remove(candidateString.begin(), candidateString.end(), '['), candidateString.end());
    candidateString.erase(std::remove_if(candidateString.begin(), candidateString.end(), std::bind(std::isspace < char > , std::placeholders::_1,std::locale::classic())),candidateString.end());

    stringstream ss(candidateString);
    vector<string> v;
    while (ss.good()) {
        string substr;
        getline(ss, substr, ',');
        v.push_back(substr);
    }
    int j =0;
    for(int i = 0; i < v.size(); i++) {
        if(i%2 != 0){
            candidates[j].setParty(v[i][0]);
            j++;
        }
        else{
            candidates[j].setName(v[i]);
        }

    }
    POElection->setCandidates(candidates);
    std::getline(*(files[0]), stringStore);
    int numBallots = std::stoi(stringStore);
//    POElection->setNumBallots(numBallots);
//    audit->writeMetadata(IRElection);
    readBallots(POElection);
    return POElection;
}

ElectionObject* FileParser::getMetadata() {
    std::string year = getYear();
    std::string month = getMonth();
    std::string day = getDay();
    std::vector<int> ranges{31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int monthAsInt = std::stoi(month);
    int dayAsInt = std::stoi(day);
    while(dayAsInt >= ranges[monthAsInt-1]) {
        std::cout << "Day is out of range for month! (>" << ranges[monthAsInt-1] << ")\n";
        std::cout << "Please try again.\n";
        year = getYear();
        month = getMonth();
        day = getDay();
        monthAsInt = std::stoi(month);
        dayAsInt = std::stoi(day);
    }
    std::string stringStore;
    std::getline(*(files[0]), stringStore);
    if(stringStore == "IR") {
        IRV* irv = handleIRV(day, month, year);
        return irv;
    } else if(stringStore == "CPL") {
        CPL* cpl = handleCPL(day, month, year);
        return cpl;
    } else if(stringStore == "PO") {
        PO* po = handlePO(day, month, year);
        return po;
    } 
    throw std::invalid_argument("Invalid election type!\n");
}

ElectionObject* FileParser::getMultipleMetadata() {
    std::string year = getYear();
    std::string month = getMonth();
    std::string day = getDay();
    std::vector<int> ranges{31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int monthAsInt = std::stoi(month);
    int dayAsInt = std::stoi(day);
    while(dayAsInt >= ranges[monthAsInt-1]) {
        std::cout << "Day is out of range for month! (>" << ranges[monthAsInt-1] << ")\n";
        std::cout << "Please try again.\n";
        year = getYear();
        month = getMonth();
        day = getDay();
        monthAsInt = std::stoi(month);
        dayAsInt = std::stoi(day);
    }
    std::string stringStore;
    std::getline(*(files[0]), stringStore);
    if(stringStore == "IR") {
        IRV* irv = handleMultipleIRV(day, month, year);
        return irv;
    } else if(stringStore == "CPL") {
        CPL* cpl = handleMultipleCPL(day, month, year);
        return cpl;
    }
    throw std::invalid_argument("Invalid election type!\n");
}
