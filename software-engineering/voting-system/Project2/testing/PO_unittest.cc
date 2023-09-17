#include <gtest/gtest.h>
#include <iostream>
#include <fstream>
#include "../src/PO.cc"
#include "../src/PO_Candidate.cc"
#include "../src/ElectionEntity.cc"
#include "../src/ElectionObject.cc"
class POTest: public ::testing::Test {
public:
    void SetUp() {

    }

    void TearDown() {
        // code here will be called just after the test completes
        // ok to throw exceptions from here if need be
    }
protected:

};

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST_F(POTest, testSettersGetters) {
    PO* POElection = new PO();
    POElection->setNumCandidates(3);
    EXPECT_EQ(POElection->getNumCandidates(), 3);
    POElection->setNumBallots(7);
    EXPECT_EQ(POElection->getNumBallots(), 7);
    std::vector<POCandidate> candidates;
    for(int i = 0; i < 3; i++) {
        POCandidate* newCan = new POCandidate(3);
        newCan->setIndex(i);
        candidates.push_back(*newCan);
    }
    candidates[0].setParty('A');
    candidates[1].setParty('B');
    candidates[2].setParty('C');
    candidates[0].setName("A");
    candidates[1].setName("B");
    candidates[2].setName("C");
    candidates[0].incInitialFirstVotes();
    candidates[0].incInitialFirstVotes();
    candidates[0].incInitialFirstVotes();
    candidates[0].incInitialFirstVotes();
    candidates[1].setName("B");
    candidates[1].incInitialFirstVotes();
    candidates[1].incInitialFirstVotes();
    candidates[2].setName("C");
    candidates[2].incInitialFirstVotes();
    POElection->setCandidates(candidates);
    std::vector<POCandidate> newcandidates = POElection->getCandidates();
    EXPECT_EQ(newcandidates[0].getName(), "A");
}

TEST_F(POTest, testCalculateWinners) {
    PO* POElection = new PO();
    POElection->setNumCandidates(3);
    EXPECT_EQ(POElection->getNumCandidates(), 3);
    POElection->setNumBallots(7);
    EXPECT_EQ(POElection->getNumBallots(), 7);
    std::vector<POCandidate> candidates;
    for(int i = 0; i < 3; i++) {
        POCandidate* newCan = new POCandidate(3);
        newCan->setIndex(i);
        candidates.push_back(*newCan);
    }
    candidates[0].setParty('A');
    candidates[1].setParty('B');
    candidates[2].setParty('C');
    candidates[0].setName("A");
    candidates[1].setName("B");
    candidates[2].setName("C");
    candidates[0].incInitialFirstVotes();
    candidates[0].incInitialFirstVotes();
    candidates[0].incInitialFirstVotes();
    candidates[0].incInitialFirstVotes();
    candidates[1].setName("B");
    candidates[1].incInitialFirstVotes();
    candidates[1].incInitialFirstVotes();
    candidates[2].setName("C");
    candidates[2].incInitialFirstVotes();
    POElection->setCandidates(candidates);
    std::vector<POCandidate> newcandidates = POElection->getCandidates();
    EXPECT_EQ(newcandidates[0].getName(), "A");
    testing::internal::CaptureStdout();
    POElection->calculateWinners();
    std::string output = testing::internal::GetCapturedStdout();
    std::string tester = "Winner is: A from party: A\n\nAll candidates, their parties, and their votes are below: \nA Party: A Votes: 4\nB Party: B Votes: 2\nC Party: C Votes: 1\n";
    EXPECT_EQ(output, tester);
}
