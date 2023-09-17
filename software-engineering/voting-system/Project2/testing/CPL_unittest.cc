#include <gtest/gtest.h>
#include <iostream>
#include <fstream>
#include "../src/CPL.cc"
#include "../src/Party.cc"
#include "../src/CPLAudit.cc"
#include "../src/ElectionEntity.cc"
#include "../src/ElectionObject.cc"
class CPLTest: public ::testing::Test { 
public:
    void SetUp() {
        // code here will execute just before the test ensues	
        setupCPL1.setNumParties(5);
        setupCPL1.setSeats(10);
        setupCPL1.setNumBallots(100000);
        Party Democratic;
        Democratic.setName("Democratic");
        Democratic.setCandidates("Pike, Rosen-Amy, Gentzler, Foster, Volz");
        Democratic.setNumVotes(38000);
        Party Republican;
        Republican.setName("Republican");
        Republican.setCandidates("Deutsch, Grolnie, Berg, Epstein, McClurg");
        Republican.setNumVotes(23000);
        Party Reform;
        Reform.setName("Reform");
        Reform.setCandidates("Wong, Gorlin, Crenshaw, Czitrom, Fingrutd");
        Reform.setNumVotes(21000);
        Party Green;
        Green.setName("Green");
        Green.setCandidates("Wartenberg, Hernandez, Pangopoules, Morey, Pringle");
        Green.setNumVotes(12000);
        Party Independent1;
        Independent1.setName("Independent");
        Independent1.setCandidates("Moll");
        Independent1.setNumVotes(6000);
        std::vector<Party> parties = {Democratic, Republican, Reform, Green, Independent1};
        setupCPL1.setParties(parties);
        CPLAudit* setupAudit = new CPLAudit();
        std::ofstream* setupAuditFile = new std::ofstream(); // since this is never opened, this shouldn't write anything
        setupAudit->setAuditFile(setupAuditFile);
        setupCPL1.setAudit(setupAudit);
    }

    void TearDown() {
        // code here will be called just after the test completes
        // ok to throw exceptions from here if need be
    }
protected:
    // put in any custom data members that you need
    CPL setupCPL1;
    int quota1 = 10000;
    std::string expect1 =    
R"(Results for the Democratic party are:
The candidates for the Democratic party are: Pike, Rosen-Amy, Gentzler, Foster, Volz
Votes: 38000
First Allocation of Seats: 3.
Remaining Votes: 8000.
Second Allocation of Seats: 1.
Final Seat Total: 4.
% of votes to % of seats: 38% / 40%

Results for the Republican party are:
The candidates for the Republican party are: Deutsch, Grolnie, Berg, Epstein, McClurg
Votes: 23000
First Allocation of Seats: 2.
Remaining Votes: 3000.
Second Allocation of Seats: 0.
Final Seat Total: 2.
% of votes to % of seats: 23% / 20%

Results for the Reform party are:
The candidates for the Reform party are: Wong, Gorlin, Crenshaw, Czitrom, Fingrutd
Votes: 21000
First Allocation of Seats: 2.
Remaining Votes: 1000.
Second Allocation of Seats: 0.
Final Seat Total: 2.
% of votes to % of seats: 21% / 20%

Results for the Green party are:
The candidates for the Green party are: Wartenberg, Hernandez, Pangopoules, Morey, Pringle
Votes: 12000
First Allocation of Seats: 1.
Remaining Votes: 2000.
Second Allocation of Seats: 0.
Final Seat Total: 1.
% of votes to % of seats: 12% / 10%

Results for the Independent party are:
The candidates for the Independent party are: Moll
Votes: 6000
First Allocation of Seats: 0.
Remaining Votes: 6000.
Second Allocation of Seats: 1.
Final Seat Total: 1.
% of votes to % of seats: 6% / 10%

)";
};

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

TEST_F(CPLTest, testSettersGetters) {
    CPL testCPL;
    testCPL.setNumParties(5);
    testCPL.setSeats(10);
    Party testParty;
    std::vector<Party>* testParties = new std::vector<Party>(5, testParty);
    testCPL.setParties(*testParties);
    CPLAudit* testAudit = new CPLAudit();
    std::ofstream* testAuditFile = new std::ofstream(); // since this is never opened, this shouldn't write anything
    testAudit->setAuditFile(testAuditFile);
    testCPL.setAudit(testAudit);

    EXPECT_EQ(testCPL.getNumParties(), 5);
    EXPECT_EQ(testCPL.getSeats(), 10);
    EXPECT_EQ(testCPL.getAudit(), testAudit);
    EXPECT_EQ(testCPL.getParties().size(), testParties->size());
    delete testParties;
}

TEST_F(CPLTest, testFirstAllocation) {
    std::vector<bool>* testAllowed = new std::vector<bool>(setupCPL1.getNumParties(), true);
    setupCPL1.firstAllocation(testAllowed, quota1);
    for(int i = 0; i < setupCPL1.getNumParties(); i++) {
        EXPECT_EQ((*testAllowed)[i], true);
    }
    EXPECT_EQ(setupCPL1.getParties()[0].getFirstSeats(), 3);
    EXPECT_EQ(setupCPL1.getParties()[1].getFirstSeats(), 2);
    EXPECT_EQ(setupCPL1.getParties()[2].getFirstSeats(), 2);
    EXPECT_EQ(setupCPL1.getParties()[3].getFirstSeats(), 1);
    EXPECT_EQ(setupCPL1.getParties()[4].getFirstSeats(), 0);

    EXPECT_EQ(setupCPL1.getParties()[0].getLeftoverVotes(), 8000);
    EXPECT_EQ(setupCPL1.getParties()[1].getLeftoverVotes(), 3000);
    EXPECT_EQ(setupCPL1.getParties()[2].getLeftoverVotes(), 1000);
    EXPECT_EQ(setupCPL1.getParties()[3].getLeftoverVotes(), 2000);
    EXPECT_EQ(setupCPL1.getParties()[4].getLeftoverVotes(), 6000);
    delete testAllowed;
}

TEST_F(CPLTest, testSecondAllocation) {
    std::vector<bool>* testAllowed = new std::vector<bool>(setupCPL1.getNumParties(), true);
    setupCPL1.firstAllocation(testAllowed, quota1);
    setupCPL1.secondAllocation(testAllowed);
    EXPECT_EQ(setupCPL1.getParties()[0].getSecondSeats(), 1);
    EXPECT_EQ(setupCPL1.getParties()[1].getSecondSeats(), 0);
    EXPECT_EQ(setupCPL1.getParties()[2].getSecondSeats(), 0);
    EXPECT_EQ(setupCPL1.getParties()[3].getSecondSeats(), 0);
    EXPECT_EQ(setupCPL1.getParties()[4].getSecondSeats(), 1);
    delete testAllowed;
}

TEST_F(CPLTest, testPrintResults) {
    std::vector<bool>* testAllowed = new std::vector<bool>(setupCPL1.getNumParties(), true);
    setupCPL1.firstAllocation(testAllowed, quota1);
    setupCPL1.secondAllocation(testAllowed);
    testing::internal::CaptureStdout();
    setupCPL1.printResults();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, expect1);
    delete testAllowed;
}

TEST_F(CPLTest, testCalculateWinners) {
    testing::internal::CaptureStdout();
    setupCPL1.calculateWinners();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, expect1);
}