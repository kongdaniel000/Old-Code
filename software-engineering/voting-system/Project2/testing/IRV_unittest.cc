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
#include "vector"

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

TEST_F(IRVTest, maxCandWinCheckTest) {
    Candidate c1(3);
    Candidate c2(3);
    Candidate c3(3);
    IRV test;
    IRVAudit* audit = new IRVAudit();
    std::ofstream* stream = new std::ofstream("test", std::ios::out);
    audit->setAuditFile(stream);
    test.setAudit(audit);
    test.setNumCandidates(3);
    std::vector<Candidate> candidates;
    std::vector<bool> inRun = {true, true, true};
    c1.incWorkingVotes(500);
    c2.incWorkingVotes(1000);
    c3.incWorkingVotes(250);
    candidates.push_back(c1);
    candidates.push_back(c2);
    candidates.push_back(c3);
    test.setCandidates(candidates);
    test.setTotal(2000);
    Candidate* winner = nullptr;
    winner = test.maxCandWinCheck(inRun);
    EXPECT_EQ(winner, nullptr);

    test.setTotal(1999);
    winner = test.maxCandWinCheck(inRun);
    EXPECT_EQ(winner->getWorkingVotes(), 1000);

    inRun[1] = false;
    winner = test.maxCandWinCheck(inRun);
    EXPECT_EQ(winner, nullptr);

    Candidate c4(3);
    Candidate c5(3);
    Candidate c6(3);
    IRV test2;
    IRVAudit* audit2 = new IRVAudit();
    std::ofstream* stream2 = new std::ofstream("test", std::ios::out);
    audit2->setAuditFile(stream2);
    test2.setAudit(audit2);
    test2.setNumCandidates(3);
    std::vector<Candidate> candidates2;
    std::vector<bool> inRun2 = {true, true, true};
    c4.incWorkingVotes(500);
    c5.incWorkingVotes(500);
    c6.incWorkingVotes(500);
    candidates2.push_back(c4);
    candidates2.push_back(c5);
    candidates2.push_back(c6);
    test2.setCandidates(candidates2);
    test2.setTotal(1500);
    winner = test2.maxCandWinCheck(inRun2);
    EXPECT_EQ(winner, nullptr);
}

TEST_F(IRVTest, printResultsTest) {
    Candidate c1(3);
    Candidate c2(3);
    Candidate c3(3);
    IRV test;
    test.setNumCandidates(3);
    IRVAudit* audit = new IRVAudit();
    std::ofstream* stream = new std::ofstream("test", std::ios::out);
    audit->setAuditFile(stream);
    test.setAudit(audit);
    c1.incInitialFirstVotes(100);
    c2.incInitialFirstVotes(50);
    c3.incInitialFirstVotes(25);
    c1.setName("Jacob");
    c2.setName("Eugene");
    c3.setName("Gage");
    c3.setGainedVotes(0);
    for (int i = 0; i < 20; i++) {
        c1.incGainedVotes(0);
    }
    for (int i = 0; i < 5; i++) {
        c2.incGainedVotes(0);
    }
    std::vector<Candidate> candidates;
    candidates.push_back(c1);
    candidates.push_back(c2);
    candidates.push_back(c3);
    test.setCandidates(candidates);
    std::vector<int> ex;
    ex.push_back(30);
    test.setExhausted(ex);
    test.setTotal(50);
    std::string expect = "Candidates & Parties          First Place Votes   Transfer 1   New Totals   \n*Jacob                        100                 +20          120          \nEugene                        50                  +5           55           \nGage                          25                  -25          ----------   \n\nEXHAUSTED PILE                0                   +30          30           \nTOTALS                        50                  \n";
    testing::internal::CaptureStdout();
    test.printResults(c1, 1);
    std::string actual = testing::internal::GetCapturedStdout();
    cout << expect;
    cout << actual;
    EXPECT_EQ(expect.compare(actual), 0);

}

TEST_F(IRVTest, setGetExhaustedTest) {
    IRV test;
    IRVAudit* audit = new IRVAudit();
    std::ofstream* stream = new std::ofstream("test", std::ios::out);
    audit->setAuditFile(stream);
    test.setAudit(audit);
    std::vector<int> exhaust = {12, 3, 5};
    test.setExhausted(exhaust);
    std::vector<int> ret;
    ret = test.getExhausted();
    EXPECT_EQ(ret[0], 12);
    EXPECT_EQ(ret[1], 3);
    EXPECT_EQ(ret[2], 5);
}

TEST_F(IRVTest, setGetTotalTest) {
    IRV test;
    IRVAudit* audit = new IRVAudit();
    std::ofstream* stream = new std::ofstream("test", std::ios::out);
    audit->setAuditFile(stream);
    test.setAudit(audit);
    test.setTotal(1000);
    EXPECT_EQ(test.getTotal(), 1000);
}

// TEST_F(IRVTest, calculateWinnersTest) {
//     Candidate c1(3);
//     Candidate c2(3);
//     Candidate c3(3);
//     IRV test;
//     c1.setName("Jacob");
//     c2.setName("Eugene");
//     c3.setName("Gage");
//     c1.setIndex(0);
//     c2.setIndex(1);
//     c3.setIndex(2);
//     std::vector<Candidate> candidates;
//     candidates.push_back(c1);
//     candidates.push_back(c2);
//     candidates.push_back(c3);
//     test.setCandidates(candidates);
//     test.setNumCandidates(3);


//     std::vector<int> votes1 = {0, 1, 2};
//     std::vector<int> votes2 = {0, 2, 1};
//     std::vector<int> votes3 = {0, 1, -1};
//     std::vector<int> votes4 = {1, -1, -1};
//     BallotLinkedList ballotList1;
//     BallotLinkedList ballotList2;
//     BallotLinkedList ballotList3;
//     BallotLinkedList ballotList4;
//     ballotList1.populateList(votes1);
//     ballotList2.populateList(votes2);
//     ballotList3.populateList(votes3);
//     ballotList4.populateList(votes4);
//     std::vector<BallotLinkedList>* lists = new std::vector<BallotLinkedList>;
//     lists->push_back(ballotList1);
//     lists->push_back(ballotList2);
//     lists->push_back(ballotList3);
//     lists->push_back(ballotList4);
//     IRVAudit* audit = new IRVAudit();
//     std::ofstream* stream = new std::ofstream("test", std::ios::out);
//     audit->setAuditFile(stream);
//     test.setAudit(audit);
//     test.setBallotList(lists);
//     ASSERT_NO_THROW(test.calculateWinners());
//     EXPECT_EQ(test.getCandidates().at(0).getWorkingVotes(), 3);
//     EXPECT_EQ(test.getCandidates().at(0).getInitialFirstVotes(), 3);
//     delete lists;
//     delete audit;
//     delete stream;

//     Candidate c4(3);
//     Candidate c5(3);
//     Candidate c6(3);
//     IRV test2;
//     c4.setName("Jacob");
//     c5.setName("Eugene");
//     c6.setName("Gage");
//     c4.setIndex(0);
//     c5.setIndex(1);
//     c6.setIndex(2);
//     std::vector<Candidate> candidates2;
//     candidates2.push_back(c4);
//     candidates2.push_back(c5);
//     candidates2.push_back(c6);
//     test2.setCandidates(candidates2);
//     test2.setNumCandidates(3);


//     std::vector<int> votes5 = {0, 1, 2};
//     std::vector<int> votes6 = {0, 2, 1};
//     std::vector<int> votes7 = {0, 1, -1};
//     std::vector<int> votes8 = {1, -1, -1};
//     std::vector<int> votes9 = {1, 2, 0};
//     std::vector<int> votes10 = {2, 0, -1};
//     BallotLinkedList ballotList5;
//     BallotLinkedList ballotList6;
//     BallotLinkedList ballotList7;
//     BallotLinkedList ballotList8;
//     BallotLinkedList ballotList9;
//     BallotLinkedList ballotList10;

//     ballotList5.populateList(votes5);
//     ballotList6.populateList(votes6);
//     ballotList7.populateList(votes7);
//     ballotList8.populateList(votes8);
//     ballotList9.populateList(votes9);
//     ballotList10.populateList(votes10);
//     std::vector<BallotLinkedList>* lists2 = new std::vector<BallotLinkedList>;
//     lists2->push_back(ballotList5);
//     lists2->push_back(ballotList6);
//     lists2->push_back(ballotList7);
//     lists2->push_back(ballotList8);
//     lists2->push_back(ballotList9);
//     lists2->push_back(ballotList10);
//     IRVAudit* audit2 = new IRVAudit();
//     std::ofstream* stream2 = new std::ofstream("test", std::ios::out);
//     audit2->setAuditFile(stream2);
//     test2.setAudit(audit2);
//     test2.setBallotList(lists2);
//     ASSERT_NO_THROW(test2.calculateWinners());
//     EXPECT_EQ(test2.getCandidates().at(0).getWorkingVotes(), 4);
//     EXPECT_EQ(test2.getCandidates().at(0).getInitialFirstVotes(), 3);
//     EXPECT_EQ(test2.getCandidates().at(0).getGainedVotes().at(0), 1);

//     EXPECT_EQ(test2.getCandidates().at(1).getWorkingVotes(), 2);
//     EXPECT_EQ(test2.getCandidates().at(1).getInitialFirstVotes(), 2);
//     EXPECT_EQ(test2.getCandidates().at(1).getGainedVotes().at(0), 0);

//     EXPECT_EQ(test2.getCandidates().at(2).getWorkingVotes(), 1);
//     EXPECT_EQ(test2.getCandidates().at(2).getInitialFirstVotes(), 1);
//     EXPECT_EQ(test2.getCandidates().at(2).getGainedVotes().at(0), 0);

//     delete lists2;
//     delete audit2;
//     delete stream2;

// }

// TEST_F(IRVTest, redistributeTest) {
//     Candidate c1(4);
//     Candidate c2(4);
//     Candidate c3(4);
//     Candidate c4(4);
//     IRV test;
//     test.setNumCandidates(4);
//     std::vector<Candidate> candidates;
//     c1.incWorkingVotes(10);
//     c2.incWorkingVotes(2);
//     c3.incWorkingVotes(12);
//     c4.incWorkingVotes(13);
//     c1.setIndex(0);
//     c2.setIndex(1);
//     c3.setIndex(2);
//     c4.setIndex(3);

//     candidates.push_back(c1);
//     candidates.push_back(c2);
//     candidates.push_back(c3);
//     candidates.push_back(c4);

//     test.setCandidates(candidates);

//     BallotLinkedList ballotList1;
//     BallotLinkedList ballotList2;
//     BallotLinkedList ballotList3;
//     BallotLinkedList ballotList4;
    
//     std::vector<int> votes1 = {1, 0, 3, -1};
//     std::vector<int> votes2 = {1, 2, 3, 0};
//     std::vector<int> votes3 = {2, 0, 3, -1};
//     std::vector<int> votes4 = {-1, -1, -1, -1};
//     ballotList1.populateList(votes1);
//     ballotList2.populateList(votes2);
//     ballotList3.populateList(votes3);
//     ballotList4.populateList(votes4);

//     std::vector<BallotLinkedList>* lists = new std::vector<BallotLinkedList>;
//     lists->push_back(ballotList1);
//     lists->push_back(ballotList2);
//     lists->push_back(ballotList3);
//     lists->push_back(ballotList4);

//     IRVAudit* audit = new IRVAudit();
//     std::ofstream* stream = new std::ofstream("test", std::ios::out);
//     audit->setAuditFile(stream);
//     test.setAudit(audit);
//     test.setBallotList(lists);
//     std::vector<bool> inRun = {true, true, false, true};
//     test.redistribute(c2, inRun, 1);
//     EXPECT_EQ(test.getBallotList()->at(0).getHead(), 0);
//     EXPECT_EQ(test.getBallotList()->at(1).getHead(), 3);
//     EXPECT_EQ(test.getBallotList()->at(2).getHead(), 2);
//     EXPECT_EQ(test.getBallotList()->at(3).getHead(), -1);

//     EXPECT_EQ(test.getCandidates().at(0).getWorkingVotes(), 11);
//     EXPECT_EQ(test.getCandidates().at(1).getWorkingVotes(), 2);
//     EXPECT_EQ(test.getCandidates().at(2).getWorkingVotes(), 12);
//     EXPECT_EQ(test.getCandidates().at(3).getWorkingVotes(), 14);

//     EXPECT_EQ(test.getCandidates().at(0).getGainedVotes().at(1), 1);
//     EXPECT_EQ(test.getCandidates().at(1).getGainedVotes().at(1), 0);
//     EXPECT_EQ(test.getCandidates().at(2).getGainedVotes().at(1), 0);
//     EXPECT_EQ(test.getCandidates().at(3).getGainedVotes().at(1), 1);
//     delete lists;
//     delete audit;
// }

// TEST_F(IRVTest, getLoserTest) {
//     Candidate c1(3);
//     Candidate c2(3);
//     Candidate c3(3);
//     IRV test;
//     test.setNumCandidates(3);
//     std::vector<Candidate> candidates;
//     std::vector<bool> inRun = {true, true, true};
//     c1.incWorkingVotes(1001);
//     c2.incWorkingVotes(1000);
//     c3.incWorkingVotes(999);
//     candidates.push_back(c1);
//     candidates.push_back(c2);
//     candidates.push_back(c3);
//     test.setCandidates(candidates);
//     Candidate loser = test.getLoser(inRun);
//     EXPECT_EQ(loser.getWorkingVotes(), 999);

//     Candidate c4(3);
//     Candidate c5(3);
//     Candidate c6(3);
//     IRV test2;
//     test2.setNumCandidates(3);
//     std::vector<Candidate> candidates2;
//     std::vector<bool> inRun2 = {true, true, false};
//     c4.incWorkingVotes(1001);
//     c5.incWorkingVotes(1000);
//     c6.incWorkingVotes(999);
//     candidates2.push_back(c4);
//     candidates2.push_back(c5);
//     candidates2.push_back(c6);
//     test2.setCandidates(candidates2);
//     Candidate loser2 = test2.getLoser(inRun2);
//     EXPECT_EQ(loser2.getWorkingVotes(), 1000);

//     Candidate c7(3);
//     Candidate c8(3);
//     Candidate c9(3);
//     IRV test3;
//     test3.setNumCandidates(3);
//     std::vector<Candidate> candidates3;
//     std::vector<bool> inRun3 = {true, true, false};
//     c7.incWorkingVotes(5000);
//     c8.incWorkingVotes(5000);
//     c9.incWorkingVotes(999);
//     candidates3.push_back(c7);
//     candidates3.push_back(c8);
//     candidates3.push_back(c9);
//     IRVAudit* audit = new IRVAudit();
//     std::ofstream* stream = new std::ofstream("test", std::ios::out);
//     audit->setAuditFile(stream);
//     test3.setAudit(audit);
//     test3.setCandidates(candidates3);
//     Candidate loser3 = test3.getLoser(inRun3);
//     EXPECT_EQ(loser3.getWorkingVotes(), 5000);
//     delete audit;
//     delete stream;

// }

// TEST_F(IRVTest, getWinnerTwoLeftTest) {
//     Candidate c1(3);
//     Candidate c2(3);
//     Candidate c3(3);
//     IRV test;
//     c1.incWorkingVotes(999);
//     c3.incWorkingVotes(999999);
//     c2.incWorkingVotes(1000);
//     std::vector<Candidate> candidates;
//     candidates.push_back(c1);
//     candidates.push_back(c2);
//     candidates.push_back(c3);
//     std::vector<bool> inRun = {true, true, false};
//     test.setCandidates(candidates);
//     test.setNumCandidates(3);
//     Candidate* winner = nullptr;
//     winner = test.getWinnerTwoLeft(inRun);
//     ASSERT_NE(winner, nullptr);
//     EXPECT_EQ(winner->getWorkingVotes(), 1000);

//     Candidate c4(3);
//     Candidate c5(3);
//     Candidate c6(3);
//     IRVAudit* audit = new IRVAudit();
//     std::ofstream* stream = new std::ofstream("test", std::ios::out);
//     audit->setAuditFile(stream);
//     c4.setIndex(69);
//     c6.setIndex(70);
//     c4.setName("bob");
//     c5.setName("bruh");
//     c6.setName("Jacob");
//     IRV test2;
//     test2.setAudit(audit);
//     c4.incWorkingVotes(1000);
//     c5.incWorkingVotes(999999);
//     c6.incWorkingVotes(1000);
//     std::vector<Candidate> candidates2;
//     candidates2.push_back(c4);
//     candidates2.push_back(c5);
//     candidates2.push_back(c6);
//     std::vector<bool> inRun2 = {true, false, true};
//     test2.setCandidates(candidates2);
//     test2.setNumCandidates(3);
//     winner = nullptr;
//     //winner = test2.getWinnerTwoLeft(inRun2);
//     ASSERT_NE(winner, nullptr);
//     EXPECT_EQ(winner->getIndex(), 69);
//     delete audit;
//     delete stream;
// }

// TEST_F(IRVTest, fillFirstPlaceVotesTest) {
//     Candidate c1(3);
//     Candidate c2(3);
//     Candidate c3(3);
//     std::vector<Candidate> candidates;
//     candidates.push_back(c1);
//     candidates.push_back(c2);
//     candidates.push_back(c3);
//     IRV test;
//     test.setCandidates(candidates);
//     BallotLinkedList ballotList1;
//     std::vector<int> votes1 = {2, 2, 1};
//     ballotList1.populateList(votes1);
//     BallotLinkedList ballotList2;
//     std::vector<int> votes2 = {1, 1, 3};
//     ballotList2.populateList(votes2);
//     BallotLinkedList ballotList3;
//     std::vector<int> votes3 = {1, 1, 3};
//     ballotList3.populateList(votes3);
//     BallotLinkedList ballotList4;
//     std::vector<int> votes4 = {-1, 1, 3};
//     ballotList4.populateList(votes4);
//     std::vector<BallotLinkedList>* lists = new std::vector<BallotLinkedList>;
//     lists->push_back(ballotList1);
//     lists->push_back(ballotList2);
//     lists->push_back(ballotList3);
//     lists->push_back(ballotList4);
//     test.setBallotList(lists);
//     test.fillFirstPlaceVotes();
//     EXPECT_EQ(test.getCandidates()[0].getInitialFirstVotes(), 0);
//     EXPECT_EQ(test.getCandidates()[0].getWorkingVotes(), 0);
//     EXPECT_EQ(test.getCandidates()[1].getInitialFirstVotes(), 2);
//     EXPECT_EQ(test.getCandidates()[1].getWorkingVotes(), 2);
//     EXPECT_EQ(test.getCandidates()[2].getInitialFirstVotes(), 1);
//     EXPECT_EQ(test.getCandidates()[2].getWorkingVotes(), 1);
//     delete lists;
// }

// TEST_F(IRVTest, printResultsTest) {
//     Candidate c1(3);
//     Candidate c2(3);
//     Candidate c3(3);
//     IRV test;
//     test.setNumCandidates(3);
//     c1.incInitialFirstVotes(100);
//     c2.incInitialFirstVotes(50);
//     c3.incInitialFirstVotes(25);
//     c1.setName("Jacob");
//     c2.setName("Eugene");
//     c3.setName("Gage");
//     for (int i = 0; i < 20; i++) {
//         c1.incGainedVotes(0);
//     }
//     for (int i = 0; i < 5; i++) {
//         c2.incGainedVotes(0);
//     }
//     std::vector<Candidate> candidates;
//     candidates.push_back(c1);
//     candidates.push_back(c2);
//     candidates.push_back(c3);
//     test.setCandidates(candidates);
//     std::string expect = "Candidates & Parties          First Place Votes   Transfer 1   New Totals   \n*Jacob                        100                 +20          120          \nEugene                        50                  +5           55           \nGage                          25                  ----------   ----------   \n";
//     testing::internal::CaptureStdout();
//     test.printResults(c1, 1);
//     std::string actual = testing::internal::GetCapturedStdout();
//     EXPECT_EQ(expect.compare(actual), 0);

// }

// TEST_F(IRVTest, getCandidatesTest) {
//     IRV test;
//     std::vector<Candidate> candidates;
//     Candidate cand1(3);
//     Candidate cand2(3);
//     cand1.setIndex(1);
//     cand2.setIndex(2);
//     candidates.push_back(cand1);
//     candidates.push_back(cand2);
//     test.setCandidates(candidates);
//     EXPECT_EQ(test.getCandidates().at(0).getIndex(), 1);
// }

// TEST_F(IRVTest, setCandidatesTest) {
//     IRV test;
//     std::vector<Candidate> candidates;
//     Candidate cand1(3);
//     Candidate cand2(2);
//     cand1.setIndex(1);
//     cand2.setIndex(2);
//     candidates.push_back(cand1);
//     candidates.push_back(cand2);
//     test.setCandidates(candidates);
//     EXPECT_EQ(test.getCandidates().at(0).getIndex(), 1);
// }

// TEST_F(IRVTest, setBallotListTest) {
//     IRV test;
//     BallotLinkedList list;
//     std::vector<BallotLinkedList>* lists = new std::vector<BallotLinkedList>;
//     list.appendNode(1);
//     lists->push_back(list);
//     test.setBallotList(lists);
//     EXPECT_EQ(test.getBallotList()->at(0).getLength(), 1);
//     delete lists;
// }

// TEST_F(IRVTest, getBallotListTest) {
//     IRV test;
//     BallotLinkedList list;
//     std::vector<BallotLinkedList>* lists = new std::vector<BallotLinkedList>;
//     list.appendNode(1);
//     lists->push_back(list);
//     test.setBallotList(lists);
//     EXPECT_EQ(test.getBallotList()->at(0).getLength(), 1);
//     delete lists;
// }

// TEST_F(IRVTest, setNumCandidatesTest) {
//     IRV test;
//     test.setNumCandidates(10);
//     EXPECT_EQ(test.getNumCandidates(), 10);
//     test.setNumCandidates(-10);
//     EXPECT_EQ(test.getNumCandidates(), -10);
// }

// TEST_F(IRVTest, getNumCandidatesTest) {
//     IRV test;
//     test.setNumCandidates(10);
//     EXPECT_EQ(test.getNumCandidates(), 10);
//     test.setNumCandidates(-10);
//     EXPECT_EQ(test.getNumCandidates(), -10);
// }

// TEST_F(IRVTest, setRemainCandidatesTest) {
//     IRV test;
//     test.setRemainCandidates(10);
//     EXPECT_EQ(test.getRemainCandidates(), 10);
//     test.setRemainCandidates(-10);
//     EXPECT_EQ(test.getRemainCandidates(), -10);
// }

// TEST_F(IRVTest, getRemainCandidatesTest) {
//     IRV test;
//     test.setRemainCandidates(10);
//     EXPECT_EQ(test.getRemainCandidates(), 10);
//     test.setRemainCandidates(-10);
//     EXPECT_EQ(test.getRemainCandidates(), -10);
// }

// TEST_F(IRVTest, setAuditTest) {
//     IRV test;
//     IRVAudit* audit = new IRVAudit();
//     test.setAudit(audit);
//     EXPECT_NE(test.getAudit(), nullptr);
// }

// TEST_F(IRVTest, getAuditTest) {
//     IRV test;
//     IRVAudit* audit = new IRVAudit();
//     test.setAudit(audit);
//     EXPECT_NE(test.getAudit(), nullptr);
//     delete audit;
// }
