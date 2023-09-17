#include <gtest/gtest.h>
#include <iostream>
#include "../src/IRV.cc"
#include "../src/IRV.h"
#include "../src/Candidate.cc"
#include "../src/BallotLinkedList.cc"
#include "../src/ElectionEntity.cc"
#include "../src/IRVAudit.cc"
#include "../src/BallotNode.cc"
#include "../src/ElectionObject.cc"

class IRVTest: public ::testing::Test { 
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
};

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

TEST_F(IRVTest, calculateWinnersTest) {
    //do something here
}

TEST_F(IRVTest, redistributeTest) {
    //do something here
}

TEST_F(IRVTest, getLoserTest) {
    //do something here
}

TEST_F(IRVTest, maxCandWinCheckTest) {

}

TEST_F(IRVTest, getWinnerTwoLeftTest) {
    //do something here
}

TEST_F(IRVTest, fillFirstPlaceVotesTest) {
    //do something here
}

TEST_F(IRVTest, printResultsTest) {
    //do something here
}

TEST_F(IRVTest, getCandidatesTest) {
    IRV test;
    std::vector<Candidate> candidates;
    Candidate cand1("R", 3);
    Candidate cand2("D", 3);
    cand1.setIndex(1);
    cand2.setIndex(2);
    candidates.push_back(cand1);
    candidates.push_back(cand2);
    test.setCandidates(candidates);
    EXPECT_EQ(test.getCandidates().at(0).getIndex(), 1);
}

TEST_F(IRVTest, setCandidatesTest) {
    IRV test;
    std::vector<Candidate> candidates;
    Candidate cand1("R", 3);
    Candidate cand2("D", 2);
    cand1.setIndex(1);
    cand2.setIndex(2);
    candidates.push_back(cand1);
    candidates.push_back(cand2);
    test.setCandidates(candidates);
    EXPECT_EQ(test.getCandidates().at(0).getIndex(), 1);
}

TEST_F(IRVTest, setBallotListTest) {
     IRV test;
    BallotLinkedList list;
    std::vector<BallotLinkedList> lists;
    list.appendNode(1);
    lists.push_back(list);
    test.setBallotList(lists);
    EXPECT_EQ(test.getBallotList().at(0).getLength(), 1);
}

TEST_F(IRVTest, getBallotListTest) {
    IRV test;
    BallotLinkedList list;
    std::vector<BallotLinkedList> lists;
    list.appendNode(1);
    lists.push_back(list);
    test.setBallotList(lists);
    EXPECT_EQ(test.getBallotList().at(0).getLength(), 1);
}

TEST_F(IRVTest, setNumCandidatesTest) {
    IRV test;
    test.setNumCandidates(10);
    EXPECT_EQ(test.getNumCandidates(), 10);
    test.setNumCandidates(-10);
    EXPECT_EQ(test.getNumCandidates(), -10);
}

TEST_F(IRVTest, getNumCandidatesTest) {
    IRV test;
    test.setNumCandidates(10);
    EXPECT_EQ(test.getNumCandidates(), 10);
    test.setNumCandidates(-10);
    EXPECT_EQ(test.getNumCandidates(), -10);
}

TEST_F(IRVTest, setRemainCandidatesTest) {
    IRV test;
    test.setRemainCandidates(10);
    EXPECT_EQ(test.getRemainCandidates(), 10);
    test.setRemainCandidates(-10);
    EXPECT_EQ(test.getRemainCandidates(), -10);
}

TEST_F(IRVTest, getRemainCandidatesTest) {
    IRV test;
    test.setRemainCandidates(10);
    EXPECT_EQ(test.getRemainCandidates(), 10);
    test.setRemainCandidates(-10);
    EXPECT_EQ(test.getRemainCandidates(), -10);
}

TEST_F(IRVTest, setAuditTest) {
    IRV test;
    IRVAudit* audit = new IRVAudit("10", "20", "2020");
    test.setAudit(audit);
    EXPECT_NE(test.getAudit(), nullptr);
}

TEST_F(IRVTest, getAuditTest) {
    IRV test;
    IRVAudit* audit = new IRVAudit("10", "20", "2020");
    test.setAudit(audit);
    EXPECT_NE(test.getAudit(), nullptr);
}