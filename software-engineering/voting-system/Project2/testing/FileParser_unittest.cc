#include <gtest/gtest.h>
#include <iostream>
#include "../src/FileParser.cc"
#include "../src/ElectionObject.cc"
#include "../src/IRV.cc"
#include "../src/BallotLinkedList.cc"
#include "../src/BallotNode.cc"
#include "../src/CPL.cc"
#include "../src/ElectionEntity.cc"
#include "../src/Party.cc"
#include "../src/Candidate.cc"
#include "../src/ElectionAudit.h"
#include "../src/CPLAudit.cc"
#include "../src/IRVAudit.cc"

class FileParserTest: public ::testing::Test { 
public:
    FileParserTest() {  
        // initialization code here
    }	    

    void SetUp() {
        // code here will execute just before the test ensues	
    }

    void TearDown() {
        // code here will be called just after the test completes
        // ok to throw exceptions from here if need be
    }

    ~FileParserTest() {
        // cleanup any pending stuff, but no exceptions allowed
    }
protected:
    // put in any custom data members that you need
    FileParser setupFP;
    std::string CPLTest1 = "CPLTest1.csv";
    std::string CPLTest2 = "CPLTest2.csv";
    std::string CPLTest1b = "CPLTest1Ballots.csv";
    std::string CPLTest2b = "CPLTest2Ballots.csv";
    std::string CPLTest1h = "CPLTest1Handle.csv";
    std::string CPLTest2h = "CPLTest2Handle.csv";
    std::string IRVTest = "IRVTest.csv";
    std::string IRVTestb = "IRVTestBallots.csv";
    std::string IRVTesth = "IRVTestHandle.csv";
};

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST_F(FileParserTest, testOpenFile) {
    EXPECT_EQ(setupFP.openFile(CPLTest1), 0);
    EXPECT_EQ(setupFP.openFile(IRVTest), 0);
    EXPECT_EQ(setupFP.openFile("FailInput"), 1);
}

TEST_F(FileParserTest, testIRVBallotToArray) {
    // ballot is always assumed to be correct
    std::string ballot1 = "1,2,3,4,5";
    std::string ballot2 = ",4,3,2,1";
    std::string ballot3 = "1,3,,4,2";
    std::string ballot4 = "1,,,,";
    std::string ballot5 = ",,,1,2";
    std::vector<int> ballot1Arr = setupFP.ballotToArray(ballot1, 5);
    std::vector<int> ballot2Arr = setupFP.ballotToArray(ballot2, 5);
    std::vector<int> ballot3Arr = setupFP.ballotToArray(ballot3, 5);
    std::vector<int> ballot4Arr = setupFP.ballotToArray(ballot4, 5);
    std::vector<int> ballot5Arr = setupFP.ballotToArray(ballot5, 5);
    // candidate 1 is 1st place, 2 is 2nd, etc.
    std::vector<int> ballot1Expect = {0, 1, 2, 3, 4};
    // candidate 5 is 1st place, 4 is 2nd, etc., 0 was not assigned
    std::vector<int> ballot2Expect = {4, 3, 2, 1, -1};
    // candidate 1 is 1st place, 5 is 2nd, 2 is 3rd, 4 is 4th, 3 was not assigned
    std::vector<int> ballot3Expect = {0, 4, 1, 3, -1};
    // candidate 1 is 1st place, all others not assigned
    std::vector<int> ballot4Expect = {0, -1, -1, -1, -1};
    // candidate 4 is 1st place, 5 is 2nd place, all others not assigned
    std::vector<int> ballot5Expect = {3, 4, -1, -1, -1};
    for(int i = 0; i < 5; i++){
        EXPECT_EQ(ballot1Arr[i], ballot1Expect[i]);
    }
    for(int i = 0; i < 5; i++){
        EXPECT_EQ(ballot2Arr[i], ballot2Expect[i]);
    }
    for(int i = 0; i < 5; i++){
        EXPECT_EQ(ballot3Arr[i], ballot3Expect[i]);
    }
    for(int i = 0; i < 5; i++){
        EXPECT_EQ(ballot4Arr[i], ballot4Expect[i]);
    }
    for(int i = 0; i < 5; i++){
        EXPECT_EQ(ballot5Arr[i], ballot5Expect[i]);
    }
}

TEST_F(FileParserTest, testCPLReadBallots1) {
    EXPECT_EQ(setupFP.openFile(CPLTest1b), 0);
    CPL* testCPL = new CPL();
    CPLAudit* testAudit = new CPLAudit();
    std::ofstream* testAuditFile = new std::ofstream();
    testAudit->setAuditFile(testAuditFile); 
    testCPL->setAudit(testAudit);
    testCPL->setNumParties(6);
    std::vector<Party> parties(6);
    testCPL->setParties(parties);
    testCPL->setNumBallots(9);
    setupFP.readBallots(testCPL);
    EXPECT_EQ(testCPL->getParties()[0].getNumVotes(), 3);
    EXPECT_EQ(testCPL->getParties()[1].getNumVotes(), 2);
    EXPECT_EQ(testCPL->getParties()[2].getNumVotes(), 0);
    EXPECT_EQ(testCPL->getParties()[3].getNumVotes(), 2);
    EXPECT_EQ(testCPL->getParties()[4].getNumVotes(), 1);
    EXPECT_EQ(testCPL->getParties()[5].getNumVotes(), 1);
    delete testCPL;
}

TEST_F(FileParserTest, testHandleCPL1) {
    EXPECT_EQ(setupFP.openFile(CPLTest1h), 0);
    std::string stringStore;
    CPL* returnCPL = setupFP.handleCPL("26", "06", "2023");
    EXPECT_EQ(returnCPL->getParties()[0].getNumVotes(), 3);
    EXPECT_EQ(returnCPL->getParties()[1].getNumVotes(), 2);
    EXPECT_EQ(returnCPL->getParties()[2].getNumVotes(), 0);
    EXPECT_EQ(returnCPL->getParties()[3].getNumVotes(), 2);
    EXPECT_EQ(returnCPL->getParties()[4].getNumVotes(), 1);
    EXPECT_EQ(returnCPL->getParties()[5].getNumVotes(), 1);
    delete returnCPL;
}

TEST_F(FileParserTest, testCPLReadBallots2) {
    EXPECT_EQ(setupFP.openFile(CPLTest2b), 0);
    CPL* testCPL = new CPL();
    CPLAudit* testAudit = new CPLAudit();
    std::ofstream* testAuditFile = new std::ofstream();
    testAudit->setAuditFile(testAuditFile);
    testCPL->setAudit(testAudit);
    testCPL->setNumParties(5);
    std::vector<Party> parties(5);
    testCPL->setParties(parties);
    testCPL->setNumBallots(18);
    setupFP.readBallots(testCPL);
    EXPECT_EQ(testCPL->getParties()[0].getNumVotes(), 3);
    for(int i = 1; i < 4; i++) {
        EXPECT_EQ(testCPL->getParties()[i].getNumVotes(), 4);
    }
    EXPECT_EQ(testCPL->getParties()[4].getNumVotes(), 3);
    delete testCPL;
}

TEST_F(FileParserTest, testHandleCPL2) {
    EXPECT_EQ(setupFP.openFile(CPLTest2h), 0);
    CPL* returnCPL = setupFP.handleCPL("26", "03", "2023");
    EXPECT_EQ(returnCPL->getParties()[0].getNumVotes(), 3);
    for(int i = 1; i < 4; i++) {
        EXPECT_EQ(returnCPL->getParties()[i].getNumVotes(), 4);
    }
    EXPECT_EQ(returnCPL->getParties()[4].getNumVotes(), 3);
    delete returnCPL;
}

TEST_F(FileParserTest, testIRVReadBallots) {
    EXPECT_EQ(setupFP.openFile(IRVTestb), 0);
    IRV* testIRV = new IRV();
    IRVAudit* testAudit = new IRVAudit();
    std::ofstream* testAuditFile = new std::ofstream();
    testAudit->setAuditFile(testAuditFile); 
    testIRV->setAudit(testAudit);
    testIRV->setNumCandidates(4);
    std::vector<Candidate> candidates;
    for(int i = 0; i < 4; i++) {
        Candidate cand(4);
        candidates.push_back(cand);
    }
    testIRV->setCandidates(candidates);
    testIRV->setNumBallots(6);
    setupFP.readBallots(testIRV);
    std::vector<BallotLinkedList>* testBallotList = testIRV->getBallotList();
    EXPECT_EQ((*testBallotList)[0].getHead(), 0);
    EXPECT_EQ((*testBallotList)[1].getHead(), 0);
    EXPECT_EQ((*testBallotList)[2].getHead(), 0);
    EXPECT_EQ((*testBallotList)[3].getHead(), 2);
    EXPECT_EQ((*testBallotList)[4].getHead(), 2);
    EXPECT_EQ((*testBallotList)[5].getHead(), 3);
    delete testIRV;
}

TEST_F(FileParserTest, testHandleIRV) {
    EXPECT_EQ(setupFP.openFile(IRVTesth), 0);
    IRV* returnIRV = setupFP.handleIRV("26", "03", "2023");
    std::vector<BallotLinkedList>* testBallotList = returnIRV->getBallotList();
    EXPECT_EQ((*testBallotList)[0].getHead(), 0);
    EXPECT_EQ((*testBallotList)[1].getHead(), 0);
    EXPECT_EQ((*testBallotList)[2].getHead(), 0);
    EXPECT_EQ((*testBallotList)[3].getHead(), 2);
    EXPECT_EQ((*testBallotList)[4].getHead(), 2);
    EXPECT_EQ((*testBallotList)[5].getHead(), 3);
    delete returnIRV;
}