#include "../src/CPLAudit.h"
#include "../src/CPLAudit.cc"
#include "../src/ElectionAudit.h"
#include "../src/Party.h"
#include "../src/Party.cc"
#include <gtest/gtest.h>
#include <iostream>
#include "../src/BallotLinkedList.cc"
#include "../src/BallotNode.cc"
#include "../src/ElectionObject.cc"
#include "../src/ElectionObject.h"
#include "../src/ElectionEntity.cc"
#include "../src/CPL.h"
#include "../src/CPL.cc"
#include <iomanip>
#include <string.h>
#include <sstream>
#include <vector>
#include <bits/stdc++.h>
#include <fstream>
#include "../src/Candidate.cc"
class CPLAuditTest: public ::testing::Test {
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

TEST_F(CPLAuditTest, writeMetadataTest) {
    std::streambuf *psbuf, *backup;
    std::ofstream* filestr = new std::ofstream;
    filestr->open ("test.txt");
    backup = filestr->rdbuf(); //backup filestr streambuf
    psbuf = std::cout.rdbuf(); //get std::out's streambuf
    filestr->basic_ios<char>::rdbuf(psbuf); //assign cout to streambuf
    CPL* cply = new CPL();
    CPLAudit *auditFile = new CPLAudit;
    auditFile->setAuditFile(filestr);
    cply->setAudit(auditFile);
    cply->setNumParties(2);
    Party Democratic;
    Democratic.setName("Democratic");
    Democratic.setCandidates("Bob, Steve");
    Party Republican;
    Republican.setName("Republican");
    Republican.setCandidates("Gabi, Amy");
    std::vector<Party> parties;
    parties.push_back(Democratic);
    parties.push_back(Republican);
    cply->setParties(parties);
    cply->setSeats(2);
    cply->setNumBallots(2);
    testing::internal::CaptureStdout();
    auditFile->writeMetadata(cply);
    std::string output = testing::internal::GetCapturedStdout();
    std::string tester = "The election type is: CPL\nThe number of parties are: 2\nCandidates for the Democratic party are: Bob, Steve\nCandidates for the Republican party are: Gabi, Amy\nThe number of seats are: 2 and the number of ballots are: 2.\n";
    filestr->basic_ios<char>::rdbuf(backup);        // restore cout's original streambuf
    filestr->close();
    EXPECT_EQ(tester, output);
}

TEST_F(CPLAuditTest, writeInitialVotesTest) {
    std::streambuf *psbuf, *backup;
    std::ofstream* filestr = new std::ofstream;
    filestr->open ("test.txt");
    backup = filestr->rdbuf(); //backup filestr streambuf
    psbuf = std::cout.rdbuf(); //get std::out's streambuf
    filestr->basic_ios<char>::rdbuf(psbuf); //assign cout to streambuf
    CPL* cply = new CPL();
    CPLAudit *auditFile = new CPLAudit;
    auditFile->setAuditFile(filestr);
    cply->setAudit(auditFile);
    cply->setNumParties(2);
    Party Democratic;
    Democratic.setName("Democratic");
    Democratic.setCandidates("Bob, Steve");
    Democratic.setNumVotes(2);
    Party Republican;
    Republican.setName("Republican");
    Republican.setCandidates("Gabi, Amy");
    Republican.setNumVotes(2);
    std::vector<Party> parties;
    parties.push_back(Democratic);
    parties.push_back(Republican);
    cply->setParties(parties);
    cply->setSeats(2);
    cply->setNumBallots(2);
    testing::internal::CaptureStdout();
    auditFile->writeInitialVotes(parties);
    std::string output = testing::internal::GetCapturedStdout();
    std::string tester = "Democratic had 2 total votes.\nRepublican had 2 total votes.\n";
    filestr->basic_ios<char>::rdbuf(backup);        // restore cout's original streambuf
    filestr->close();
    EXPECT_EQ(tester, output);
}

TEST_F(CPLAuditTest, writeFinalResultsTest) {
    std::streambuf *psbuf, *backup;
    std::ofstream* filestr = new std::ofstream;
    filestr->open ("test.txt");
    backup = filestr->rdbuf(); //backup filestr streambuf
    psbuf = std::cout.rdbuf(); //get std::out's streambuf
    filestr->basic_ios<char>::rdbuf(psbuf); //assign cout to streambuf
    CPL* cply = new CPL();
    CPLAudit *auditFile = new CPLAudit;
    auditFile->setAuditFile(filestr);
    cply->setAudit(auditFile);
    cply->setNumParties(2);
    Party Democratic;
    Democratic.setName("Democratic");
    Democratic.setCandidates("Bob, Steve");
    Democratic.setNumVotes(2);
    Party Republican;
    Republican.setName("Republican");
    Republican.setCandidates("Gabi, Amy");
    Republican.setNumVotes(2);
    std::vector<Party> parties;
    parties.push_back(Democratic);
    parties.push_back(Republican);
    Democratic.setFirstSeats(1);
    Democratic.setSecondSeats(1);
    Republican.setFirstSeats(0);
    Republican.setSecondSeats(0);
    cply->setParties(parties);
    cply->setSeats(2);
    cply->setNumBallots(2);
    testing::internal::CaptureStdout();
    auditFile->writeFinalResults(parties);
    std::string output = testing::internal::GetCapturedStdout();
    std::string tester = "The final results were as follows: \nDemocratic earned 0 seats.\nRepublican earned 0 seats.\n";
    filestr->basic_ios<char>::rdbuf(backup);        // restore cout's original streambuf
    filestr->close();
    EXPECT_EQ(tester, output);
}

TEST_F(CPLAuditTest, writeFirstAllocationTest) {
    std::streambuf *psbuf, *backup;
    std::ofstream* filestr = new std::ofstream;
    filestr->open ("test.txt");
    backup = filestr->rdbuf(); //backup filestr streambuf
    psbuf = std::cout.rdbuf(); //get std::out's streambuf
    filestr->basic_ios<char>::rdbuf(psbuf); //assign cout to streambuf
    CPL* cply = new CPL();
    CPLAudit *auditFile = new CPLAudit;
    auditFile->setAuditFile(filestr);
    cply->setAudit(auditFile);
    cply->setNumParties(2);
    Party Democratic;
    Democratic.setName("Democratic");
    Democratic.setCandidates("Bob, Steve");
    Democratic.setNumVotes(2);
    Party Republican;
    Republican.setName("Republican");
    Republican.setCandidates("Gabi, Amy");
    Republican.setNumVotes(2);
    std::vector<Party> parties;
    parties.push_back(Democratic);
    parties.push_back(Republican);
    Democratic.setFirstSeats(1);
    Democratic.setSecondSeats(1);
    Republican.setFirstSeats(0);
    Republican.setSecondSeats(0);
    cply->setParties(parties);
    cply->setSeats(2);
    cply->setNumBallots(2);
    testing::internal::CaptureStdout();
    auditFile->writeFirstAllocation(parties, 1);
    std::string output = testing::internal::GetCapturedStdout();
    std::string tester = "Democratic received 2 votes and earned 0 seats.\nRepublican received 2 votes and earned 0 seats.\n";
    filestr->basic_ios<char>::rdbuf(backup);        // restore cout's original streambuf
    filestr->close();
    EXPECT_EQ(tester, output);
}

TEST_F(CPLAuditTest, writeSecondAllocationTest) {
    std::streambuf *psbuf, *backup;
    std::ofstream* filestr = new std::ofstream;
    filestr->open ("test.txt");
    backup = filestr->rdbuf(); //backup filestr streambuf
    psbuf = std::cout.rdbuf(); //get std::out's streambuf
    filestr->basic_ios<char>::rdbuf(psbuf); //assign cout to streambuf
    CPL* cply = new CPL();
    CPLAudit *auditFile = new CPLAudit;
    auditFile->setAuditFile(filestr);
    cply->setAudit(auditFile);
    cply->setNumParties(2);
    Party Democratic;
    Democratic.setName("Democratic");
    Democratic.setCandidates("Bob, Steve");
    Democratic.setNumVotes(2);
    Party Republican;
    Republican.setName("Republican");
    Republican.setCandidates("Gabi, Amy");
    Republican.setNumVotes(2);
    std::vector<Party> parties;
    parties.push_back(Democratic);
    parties.push_back(Republican);
    Democratic.setFirstSeats(1);
    Democratic.setSecondSeats(1);
    Republican.setFirstSeats(0);
    Republican.setSecondSeats(0);
    cply->setParties(parties);
    cply->setSeats(2);
    cply->setNumBallots(2);
    testing::internal::CaptureStdout();
    auditFile->writeSecondAllocation(parties);
    std::string output = testing::internal::GetCapturedStdout();
    std::string tester = "Democratic had 0 remainder votes and earned 0 more seats\nRepublican had 0 remainder votes and earned 0 more seats\n";
    filestr->basic_ios<char>::rdbuf(backup);        // restore cout's original streambuf
    filestr->close();
    EXPECT_EQ(tester, output);
}

TEST_F(CPLAuditTest, writeLotteryTest) {
    std::streambuf *psbuf, *backup;
    std::ofstream* filestr = new std::ofstream;
    filestr->open ("test.txt");
    backup = filestr->rdbuf(); //backup filestr streambuf
    psbuf = std::cout.rdbuf(); //get std::out's streambuf
    filestr->basic_ios<char>::rdbuf(psbuf); //assign cout to streambuf
    CPL* cply = new CPL();
    CPLAudit *auditFile = new CPLAudit;
    auditFile->setAuditFile(filestr);
    cply->setAudit(auditFile);
    cply->setNumParties(2);
    Party Democratic;
    Democratic.setName("Democratic");
    Democratic.setCandidates("Bob, Steve");
    Democratic.setNumVotes(2);
    Party Republican;
    Republican.setName("Republican");
    Republican.setCandidates("Gabi, Amy");
    Republican.setNumVotes(2);
    std::vector<Party> parties;
    parties.push_back(Democratic);
    parties.push_back(Republican);
    Democratic.setFirstSeats(1);
    Democratic.setSecondSeats(1);
    Republican.setFirstSeats(0);
    Republican.setSecondSeats(0);
    cply->setParties(parties);
    cply->setSeats(2);
    cply->setNumBallots(2);
    testing::internal::CaptureStdout();
    auditFile->writeLottery(Democratic, Republican);
    std::string output = testing::internal::GetCapturedStdout();
    std::string tester = "Extra seat lotteried from Democratic and given to Republican\n";
    filestr->basic_ios<char>::rdbuf(backup);        // restore cout's original streambuf
    filestr->close();
    EXPECT_EQ(tester, output);
}

TEST_F(CPLAuditTest, writeBallotTest) {
    std::streambuf *psbuf, *backup;
    std::ofstream* filestr = new std::ofstream;
    filestr->open ("test.txt");
    backup = filestr->rdbuf(); //backup filestr streambuf
    psbuf = std::cout.rdbuf(); //get std::out's streambuf
    filestr->basic_ios<char>::rdbuf(psbuf); //assign cout to streambuf
    CPL* cply = new CPL();
    CPLAudit *auditFile = new CPLAudit;
    auditFile->setAuditFile(filestr);
    cply->setAudit(auditFile);
    cply->setNumParties(2);
    Party Democratic;
    Democratic.setName("Democratic");
    Democratic.setCandidates("Bob, Steve");
    Democratic.setNumVotes(2);
    Party Republican;
    Republican.setName("Republican");
    Republican.setCandidates("Gabi, Amy");
    Republican.setNumVotes(2);
    std::vector<Party> parties;
    parties.push_back(Democratic);
    parties.push_back(Republican);
    Democratic.setFirstSeats(1);
    Democratic.setSecondSeats(1);
    Republican.setFirstSeats(0);
    Republican.setSecondSeats(0);
    cply->setParties(parties);
    cply->setSeats(2);
    cply->setNumBallots(2);
    testing::internal::CaptureStdout();
    auditFile->writeBallot(Democratic);
    std::string output = testing::internal::GetCapturedStdout();
    std::string tester = "Ballot for Democratic\n";
    filestr->basic_ios<char>::rdbuf(backup);        // restore cout's original streambuf
    filestr->close();
    EXPECT_EQ(tester, output);
}

TEST_F(CPLAuditTest, writeTiebreakerTest) {
    std::streambuf *psbuf, *backup;
    std::ofstream* filestr = new std::ofstream;
    filestr->open ("test.txt");
    backup = filestr->rdbuf(); //backup filestr streambuf
    psbuf = std::cout.rdbuf(); //get std::out's streambuf
    filestr->basic_ios<char>::rdbuf(psbuf); //assign cout to streambuf
    CPL* cply = new CPL();
    CPLAudit *auditFile = new CPLAudit;
    auditFile->setAuditFile(filestr);
    cply->setAudit(auditFile);
    cply->setNumParties(2);
    Party Democratic;
    Democratic.setName("Democratic");
    Democratic.setCandidates("Bob, Steve");
    Democratic.setNumVotes(2);
    Party Republican;
    Republican.setName("Republican");
    Republican.setCandidates("Gabi, Amy");
    Republican.setNumVotes(2);
    std::vector<Party> parties;
    parties.push_back(Democratic);
    parties.push_back(Republican);
    Democratic.setFirstSeats(1);
    Democratic.setSecondSeats(1);
    Republican.setFirstSeats(0);
    Republican.setSecondSeats(0);
    cply->setParties(parties);
    cply->setSeats(2);
    cply->setNumBallots(2);
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
    auditFile->writeTiebreaker(tieArr, tieWin);
    std::string output = testing::internal::GetCapturedStdout();
    std::string tester = "Tiebreaker chose the following winners: Bob,\nSelected from the following parties: Bob, Steve.\n";
    filestr->basic_ios<char>::rdbuf(backup);        // restore cout's original streambuf
    filestr->close();
    EXPECT_EQ(tester, output);
}

TEST_F(CPLAuditTest, writeQuotaTest) {
    std::streambuf *psbuf, *backup;
    std::ofstream* filestr = new std::ofstream;
    filestr->open ("test.txt");
    backup = filestr->rdbuf(); //backup filestr streambuf
    psbuf = std::cout.rdbuf(); //get std::out's streambuf
    filestr->basic_ios<char>::rdbuf(psbuf); //assign cout to streambuf
    CPL* cply = new CPL();
    CPLAudit *auditFile = new CPLAudit;
    auditFile->setAuditFile(filestr);
    cply->setAudit(auditFile);
    cply->setNumParties(2);
    Party Democratic;
    Democratic.setName("Democratic");
    Democratic.setCandidates("Bob, Steve");
    Democratic.setNumVotes(2);
    Party Republican;
    Republican.setName("Republican");
    Republican.setCandidates("Gabi, Amy");
    Republican.setNumVotes(2);
    std::vector<Party> parties;
    parties.push_back(Democratic);
    parties.push_back(Republican);
    Democratic.setFirstSeats(1);
    Democratic.setSecondSeats(1);
    Republican.setFirstSeats(0);
    Republican.setSecondSeats(0);
    cply->setParties(parties);
    cply->setSeats(2);
    cply->setNumBallots(2);
    testing::internal::CaptureStdout();
    auditFile->writeQuota(1);
    std::string output = testing::internal::GetCapturedStdout();
    std::string tester = "The quota was: 1.\n";
    filestr->basic_ios<char>::rdbuf(backup);        // restore cout's original streambuf
    filestr->close();
    EXPECT_EQ(tester, output);
}
