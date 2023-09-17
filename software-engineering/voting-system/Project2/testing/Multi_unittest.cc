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

class MultiTest: public ::testing::Test { 
public:
    void SetUp() {
        // code here will execute just before the test ensues	
    }

    void TearDown() {
        // code here will be called just after the test completes
        // ok to throw exceptions from here if need be
    }
protected:
    // put in any custom data members that you need   
    FileParser setupFP;
    std::vector<std::string> CPLMulti{"CPLTest1_1.csv", "CPLTest1_2.csv", "CPLTest1_3.csv", "CPLTest1_4.csv", "CPLTest1_5.csv"};
    std::vector<std::string> IRVMulti{"IRVTest1_1.csv", "IRVTest1_2.csv", "IRVTest1_3.csv", "IRVTest1_4.csv", "IRVTest1_5.csv"};
    std::string expect1 =
R"(Results for the Democratic party are:
The candidates for the Democratic party are: Foster, Volz, Pike
Votes: 3
First Allocation of Seats: 1.
Remaining Votes: 0.
Second Allocation of Seats: 0.
Final Seat Total: 1.
% of votes to % of seats: 33% / 33%

Results for the Republican party are:
The candidates for the Republican party are: Green, Xu, Wang
Votes: 2
First Allocation of Seats: 0.
Remaining Votes: 2.
Second Allocation of Seats: 1.
Final Seat Total: 1.
% of votes to % of seats: 22% / 33%

Results for the New Wave party are:
The candidates for the New Wave party are: Jacks, Rosen
Votes: 0
First Allocation of Seats: 0.
Remaining Votes: 0.
Second Allocation of Seats: 0.
Final Seat Total: 0.
% of votes to % of seats: 0% / 0%

Results for the Reform party are:
The candidates for the Reform party are: McClure, Berg
Votes: 2
First Allocation of Seats: 0.
Remaining Votes: 2.
Second Allocation of Seats: 1.
Final Seat Total: 1.
% of votes to % of seats: 22% / 33%

Results for the Green party are:
The candidates for the Green party are: Zheng, Melvin
Votes: 1
First Allocation of Seats: 0.
Remaining Votes: 1.
Second Allocation of Seats: 0.
Final Seat Total: 0.
% of votes to % of seats: 11% / 0%

Results for the Independent party are:
The candidates for the Independent party are: Peters
Votes: 1
First Allocation of Seats: 0.
Remaining Votes: 1.
Second Allocation of Seats: 0.
Final Seat Total: 0.
% of votes to % of seats: 11% / 0%

)";
    std::string expect2 =
R"(Loser is Kleinberg (R)
Loser is Royce (L)
Winner is: Rosen (D)
Candidates & Parties          First Place Votes   Transfer 1   New totals   Transfer 2   New totals   
*Rosen (D)                    3                   +0           3            +0           3            
Kleinberg (R)                 0                   ----------   ----------   ----------   ----------   
Chou (I)                      2                   +0           2            +0           2            
Royce (L)                     1                   +0           1            ----------   ----------   
)";
};

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

TEST_F(MultiTest, testOpenFiles) {
    EXPECT_EQ(setupFP.openMultiple(CPLMulti), 0);
    EXPECT_EQ(setupFP.openMultiple(IRVMulti), 0);
    std::vector<std::string> failMulti1{"FailInput", "FailInput"};
    std::vector<std::string> failMulti2{"CPLTest1_1.csv", "CPLTest1_6.csv"};
    std::vector<std::string> failMulti3{"CPLTest1_1.csv", "CPLTest1_2.csdv", "CPLTest1_23.csv", "CPLTest1_4.csv", "CPLTest1_5.csv"};
    EXPECT_EQ(setupFP.openMultiple(failMulti1), 1);
    EXPECT_EQ(setupFP.openMultiple(failMulti2), 1);
    EXPECT_EQ(setupFP.openMultiple(failMulti3), 1);
}

TEST_F(MultiTest, testCPL) {
    setupFP.openMultiple(CPLMulti);
    std::ifstream date("dateTestInput.txt");
    std::streambuf *cinbuf = std::cin.rdbuf(); //save old buf
    std::cin.rdbuf(date.rdbuf()); //redirect std::cin to dateTestInput.txt
    ElectionObject* testCPL = setupFP.getMultipleMetadata();
    EXPECT_EQ(testCPL->getNumBallots(), 9);
    std::cin.rdbuf(cinbuf); //reset to standard input again
    testing::internal::CaptureStdout();
    testCPL->calculateWinners();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, expect1);
    delete testCPL;
}

TEST_F(MultiTest, testIRV) {
    setupFP.openMultiple(IRVMulti);
    std::ifstream date("dateTestInput.txt");
    std::streambuf *cinbuf = std::cin.rdbuf(); //save old buf
    std::cin.rdbuf(date.rdbuf()); //redirect std::cin to dateTestInput.txt
    ElectionObject* testIRV = setupFP.getMultipleMetadata();
    EXPECT_EQ(testIRV->getNumBallots(), 6);
    std::cin.rdbuf(cinbuf); //reset to standard input again
    testing::internal::CaptureStdout();
    testIRV->calculateWinners();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, expect2);
    delete testIRV;
}