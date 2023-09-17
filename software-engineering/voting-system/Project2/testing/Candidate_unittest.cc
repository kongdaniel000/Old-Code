#include <gtest/gtest.h>
#include <iostream>
#include <vector>
#include "../src/Candidate.cc"
#include "../src/Candidate.h"

class CandidateTest: public ::testing::Test { 
public:

    void SetUp() {
        // code here will execute just before the test ensues
        numCandidates = 10;
        candidate = new Candidate(numCandidates);
    }

    void TearDown() {
        // code here will be called just after the test completes
        // ok to throw exceptions from here if need be
        delete candidate;
    }

protected:
    // put in any custom data members that you need
    int numCandidates;
    Candidate *candidate;
};

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

TEST_F(CandidateTest, testSetGainedVotes) {
    Candidate c1(3);
    EXPECT_NO_THROW(c1.setGainedVotes(-1));
    EXPECT_NO_THROW(c1.setGainedVotes(3));
    c1.setGainedVotes(1);
    EXPECT_EQ(c1.getGainedVotes().at(1), -1);
}

TEST_F(CandidateTest, testIncWorkingVotes) {
    candidate->incWorkingVotes(1);
    int a = candidate->getWorkingVotes();
    EXPECT_EQ(1, a);
    candidate->incWorkingVotes(10000);
    a = candidate->getWorkingVotes();
    EXPECT_EQ(10001, a);
}

TEST_F(CandidateTest, testSetIndex) {
    EXPECT_EQ(candidate->getIndex(), -1);
    candidate->setIndex(10);
    EXPECT_EQ(candidate->getIndex(), 10);
}

TEST_F(CandidateTest, testSetWorkingVotes) {
    candidate->setWorkingVotes(10);
    EXPECT_EQ(candidate->getWorkingVotes(), 10);
    candidate->setWorkingVotes(-10);
    EXPECT_EQ(candidate->getWorkingVotes(), -10);
}

TEST_F(CandidateTest, testIncInitialFirstVotes) {
    EXPECT_EQ(candidate->getInitialFirstVotes(), 0);
    candidate->incInitialFirstVotes(10);
    EXPECT_EQ(candidate->getInitialFirstVotes(), 10);
    candidate->incInitialFirstVotes(-2);
    EXPECT_EQ(candidate->getInitialFirstVotes(), 8);
}

TEST_F(CandidateTest, testIncGainedVotes) {
    std::vector<int> s = candidate->getGainedVotes();
    for (int i = 0; i < numCandidates; i++) {
        int a = s[i];
        EXPECT_EQ(a, 0);
    }

    for (int i = 0; i < numCandidates; i++) {
        candidate->incGainedVotes(i);
    }
    std::vector<int> ss = candidate->getGainedVotes();

    for (int i = 0; i < numCandidates; i++) {
        EXPECT_EQ(ss[i], 1);
    }

}
