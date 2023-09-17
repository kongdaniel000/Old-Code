#include <gtest/gtest.h>
#include <iostream>
#include <vector>
#include "../src/PO_Candidate.cc"
#include "../src/PO_Candidate.h"
#include <gtest/gtest.h>
#include <iostream>
#include "../src/ElectionAudit.h"
#include "../src/ElectionObject.cc"
#include "../src/ElectionObject.h"
#include "../src/ElectionEntity.cc"
#include <iomanip>
#include <string.h>
#include <sstream>
#include <bits/stdc++.h>
#include <fstream>

class POCandidateTest: public ::testing::Test {
public:

    void SetUp() {

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

TEST_F(POCandidateTest, testSettersGetters) {
    POCandidate c1(3);
    POCandidate c2(3);
    POCandidate c3(3);
    EXPECT_EQ(c1.getInitialFirstVotes(), 0);
    c1.setIndex(1);
    EXPECT_EQ(c1.getIndex(), 1);
    c2.setName("George");
    EXPECT_EQ(c2.getName(), "George");
    c3.setParty('A');
    EXPECT_EQ(c3.getParty(), 'A');
    c1.incInitialFirstVotes();
    EXPECT_EQ(c1.getInitialFirstVotes(), 1);
}

TEST_F(POCandidateTest, testIncInitialVotes) {
    POCandidate c1(3);
    c1.incInitialFirstVotes();
    EXPECT_EQ(c1.getInitialFirstVotes(), 1);
}