#include <gtest/gtest.h>
#include <iostream>
#include "../src/IRVAudit.cc"
#include "../src/ElectionAudit.h"
#include "../src/Candidate.cc"
#include "../src/IRV.cc"
#include "../src/BallotLinkedList.cc"
#include "../src/BallotNode.cc"
#include "../src/ElectionObject.cc"
#include "../src/ElectionObject.h"
#include "../src/ElectionEntity.cc"
#include <iomanip>
#include <string.h>
#include <sstream>
#include <vector>
#include <bits/stdc++.h>
#include "../src/IRV.h"
#include <fstream>

class IRVAuditTest: public ::testing::Test {
public:

    void setUp() {

    }

    void TearDown() {

    }

protected:
};

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST_F(IRVAuditTest, writeMetadataTest) {
    std::streambuf *psbuf, *backup;
    std::ofstream* filestr = new std::ofstream;
    filestr->open ("test.txt");
    backup = filestr->rdbuf(); //backup filestr streambuf
    psbuf = std::cout.rdbuf(); //get std::out's streambuf
    filestr->basic_ios<char>::rdbuf(psbuf); //assign cout to streambuf
    IRV* irvy = new IRV();
    IRVAudit *auditFile = new IRVAudit;
    auditFile->setAuditFile(filestr);
    irvy->setAudit(auditFile);
    Candidate c1(3);
    Candidate c2(3);
    Candidate c3(3);
    irvy->setNumCandidates(3);
    irvy->setNumBallots(3);
    testing::internal::CaptureStdout();
    auditFile->writeMetadata(irvy);
    std::string output = testing::internal::GetCapturedStdout();
    std::string tester = "The election type is: IRV\nThe number of candidates are: 3\nThe number of ballots are: 3.\n";
    filestr->basic_ios<char>::rdbuf(backup);        // restore cout's original streambuf
    filestr->close();
    EXPECT_EQ(tester, output);
}

TEST_F(IRVAuditTest, writeFinalResultsTest) {
    std::streambuf *psbuf, *backup;
    std::ofstream* filestr = new std::ofstream;
    filestr->open ("test.txt");
    backup = filestr->rdbuf(); //backup filestr streambuf
    psbuf = std::cout.rdbuf(); //get std::out's streambuf
    filestr->basic_ios<char>::rdbuf(psbuf); //assign cout to streambuf
    IRV* irvy = new IRV();
    IRVAudit *auditFile = new IRVAudit;
    auditFile->setAuditFile(filestr);
    irvy->setAudit(auditFile);
    Candidate winner(1);
    winner.setName("Bob");
    testing::internal::CaptureStdout();
    auditFile->writeFinalResults(winner, 0.5);
    std::string output = testing::internal::GetCapturedStdout();
    std::string tester = "The final results have Bob as the winning candidate with a vote majority of 0.5\n";
    filestr->basic_ios<char>::rdbuf(backup);        // restore cout's original streambuf
    filestr->close();
    EXPECT_EQ(tester, output);
}

TEST_F(IRVAuditTest, writeBallotTest) {
    std::streambuf *psbuf, *backup;
    std::ofstream* filestr = new std::ofstream;
    filestr->open ("test.txt");
    backup = filestr->rdbuf(); //backup filestr streambuf
    psbuf = std::cout.rdbuf(); //get std::out's streambuf
    filestr->basic_ios<char>::rdbuf(psbuf); //assign cout to streambuf
    IRV* irvy = new IRV();
    IRVAudit *auditFile = new IRVAudit;
    auditFile->setAuditFile(filestr);
    irvy->setAudit(auditFile);
    int numCandidates = 3;
    std::vector<Candidate> candidates;
    for(int i = 0; i < numCandidates; i++) {
        Candidate* newCan = new Candidate(numCandidates);
        newCan->setIndex(i);
        candidates.push_back(*newCan);
    }
    candidates[0].setName("Bob");
    candidates[1].setName("Steve");
    candidates[2].setName("Tres");
    std::vector<int> votes;
    for(int i = 0; i < 3; i++) {
        votes.push_back(i);
    }
    testing::internal::CaptureStdout();
    auditFile->writeBallot(votes, candidates);
    std::string output = testing::internal::GetCapturedStdout();
    std::string tester = "Ballot data: \nVoted for Bob as their 1 choice.\nVoted for Steve as their 2 choice.\nVoted for Tres as their 3 choice.\n";
    filestr->basic_ios<char>::rdbuf(backup);        // restore cout's original streambuf
    filestr->close();
    EXPECT_EQ(tester, output);
}

TEST_F(IRVAuditTest, writeVoteTransferTest) {
    std::streambuf *psbuf, *backup;
    std::ofstream* filestr = new std::ofstream;
    filestr->open ("test.txt");
    backup = filestr->rdbuf(); //backup filestr streambuf
    psbuf = std::cout.rdbuf(); //get std::out's streambuf
    filestr->basic_ios<char>::rdbuf(psbuf); //assign cout to streambuf
    IRV* irvy = new IRV();
    IRVAudit *auditFile = new IRVAudit;
    auditFile->setAuditFile(filestr);
    irvy->setAudit(auditFile);
    int numCandidates = 3;
    std::vector<Candidate> candidates;
    for(int i = 0; i < numCandidates; i++) {
        Candidate* newCan = new Candidate(numCandidates);
        newCan->setIndex(i);
        candidates.push_back(*newCan);
    }
    candidates[0].setName("Bob");
    candidates[1].setName("Steve");
    candidates[2].setName("Tres");
    std::vector<bool> running;
    running.push_back(true);
    running.push_back(false);
    running.push_back(true);
    Candidate eliminated(1);
    eliminated.setName("Bob");
    int redist = 1;
    candidates[1].incGainedVotes(1);
    candidates[2].incGainedVotes(1);
    testing::internal::CaptureStdout();
    auditFile->writeVoteTransfer(candidates, running, eliminated, redist);
    std::string output = testing::internal::GetCapturedStdout();
    std::string tester = "Bob was eliminated and their votes distributed\nBob had 0 votes distributed to them\nTres had 1 votes distributed to them\n";
    filestr->basic_ios<char>::rdbuf(backup);        // restore cout's original streambuf
    filestr->close();
    EXPECT_EQ(tester, output);
}

TEST_F(IRVAuditTest, writeTieLeadTest) {
    std::streambuf *psbuf, *backup;
    std::ofstream* filestr = new std::ofstream;
    filestr->open ("test.txt");
    backup = filestr->rdbuf(); //backup filestr streambuf
    psbuf = std::cout.rdbuf(); //get std::out's streambuf
    filestr->basic_ios<char>::rdbuf(psbuf); //assign cout to streambuf
    IRV* irvy = new IRV();
    IRVAudit *auditFile = new IRVAudit;
    auditFile->setAuditFile(filestr);
    irvy->setAudit(auditFile);
//    int numCandidates = 3;
//    std::vector<Candidate> candidates;
//    for(int i = 0; i < numCandidates; i++) {
//        Candidate* newCan = new Candidate(numCandidates);
//        newCan->setIndex(i);
//        candidates.push_back(*newCan);
//    }
//    candidates[0].setName("Bob");
//    candidates[1].setName("Steve");
//    candidates[2].setName("Tres");
//    Candidate winner(1);
//    winner.setName("Bob");
    std::vector<Candidate> candidates;
    for(int i = 0; i < 3; i++) {
        Candidate newCan = Candidate(3);
        candidates.push_back(newCan);
    }
    candidates.at(0).setName("Bob");
    candidates.at(1).setName("Steve");
    candidates.at(2).setName("Tres");
//    ElectionEntity* winner = nullptr;
//    winner->setName("Bob");
    testing::internal::CaptureStdout();
    std::vector<ElectionEntity*> tieArr;
    tieArr.push_back(&candidates[0]);
    tieArr.push_back(&candidates[1]);
    std::vector<ElectionEntity*> tieWin;
    tieWin.push_back(tieArr[0]) ;
    auditFile->writeTieLead(tieArr, tieWin[0]);
    std::string output = testing::internal::GetCapturedStdout();
    std::string tester = "Bob won in a tie breaker for winning the election against Steve, \n";
    filestr->basic_ios<char>::rdbuf(backup);        // restore cout's original streambuf
    filestr->close();
    EXPECT_EQ(tester, output);
}

TEST_F(IRVAuditTest, writeTieLoseTest) {
    std::streambuf *psbuf, *backup;
    std::ofstream* filestr = new std::ofstream;
    filestr->open ("test.txt");
    backup = filestr->rdbuf(); //backup filestr streambuf
    psbuf = std::cout.rdbuf(); //get std::out's streambuf
    filestr->basic_ios<char>::rdbuf(psbuf); //assign cout to streambuf
    IRV* irvy = new IRV();
    IRVAudit *auditFile = new IRVAudit;
    auditFile->setAuditFile(filestr);
    irvy->setAudit(auditFile);
//    int numCandidates = 3;
//    std::vector<Candidate> candidates;
//    for(int i = 0; i < numCandidates; i++) {
//        Candidate* newCan = new Candidate(numCandidates);
//        newCan->setIndex(i);
//        candidates.push_back(*newCan);
//    }
//    candidates[0].setName("Bob");
//    candidates[1].setName("Steve");
//    candidates[2].setName("Tres");
//    Candidate winner(1);
//    winner.setName("Bob");
    std::vector<Candidate> candidates;
    for(int i = 0; i < 3; i++) {
        Candidate newCan = Candidate(3);
        candidates.push_back(newCan);
    }
    candidates.at(0).setName("Bob");
    candidates.at(1).setName("Steve");
    candidates.at(2).setName("Tres");
//    ElectionEntity* winner = nullptr;
//    winner->setName("Bob");
    testing::internal::CaptureStdout();
    std::vector<ElectionEntity*> tieArr;
    tieArr.push_back(&candidates[0]);
    tieArr.push_back(&candidates[1]);
    std::vector<ElectionEntity*> tieWin;
    tieWin.push_back(tieArr[0]) ;
    auditFile->writeTieLose(tieArr, tieWin[0]);
    std::string output = testing::internal::GetCapturedStdout();
    std::string tester = "Bob lost in a tie breaker for lowest votes against Steve, \n";
    filestr->basic_ios<char>::rdbuf(backup);        // restore cout's original streambuf
    filestr->close();
    EXPECT_EQ(tester, output);
}
