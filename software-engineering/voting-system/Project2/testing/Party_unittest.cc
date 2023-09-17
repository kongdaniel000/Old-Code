#include <gtest/gtest.h>
#include "../src/Party.h"

class PartyTest: public ::testing::Test {
public:
    void SetUp() {
        // code here will execute just before the test ensues
        party = new Party();
    }

    void TearDown() {
        // code here will be called just after the test completes
        // ok to throw exceptions from here if need be
        delete party;
    }

protected:
    // put in any custom data members that you need
    Party *party;
};

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST_F(PartyTest, incrementVotesTest) {
    EXPECT_EQ(party->getNumVotes(), 0);
    party->incrementVotes();
    EXPECT_EQ(party->getNumVotes(), 1);
    party->incrementVotes();
    EXPECT_EQ(party->getNumVotes(), 2);
}

TEST_F(PartyTest, setFirstSeatsTest) {
    EXPECT_EQ(party->getFirstSeats(), 0);
    party->setFirstSeats(100);
    EXPECT_EQ(party->getFirstSeats(), 100);
    party->setFirstSeats(-100);
    EXPECT_EQ(party->getFirstSeats(), -100);
}

TEST_F(PartyTest, setSecondSeatsTest) {
    EXPECT_EQ(party->getSecondSeats(), 0);
    party->setSecondSeats(100);
    EXPECT_EQ(party->getSecondSeats(), 100);
    party->setSecondSeats(-100);
    EXPECT_EQ(party->getSecondSeats(), -100);
}


TEST_F(PartyTest, setCandidates) {
    EXPECT_EQ(party->getCandidates(),"");
    party->setCandidates("test1");
    EXPECT_EQ(party->getCandidates(), "test1");
    party->setCandidates("test1, test2");

    EXPECT_EQ(party->getCandidates(), "test1, test2");
    EXPECT_EQ(party->getNumCandidates(),2);
}


TEST_F(PartyTest, setNumVotes) {
    EXPECT_EQ(party->getNumVotes(), 0);
    party->setNumVotes(2);
    EXPECT_EQ(party->getNumVotes(), 2);

    party->setNumVotes(-2);
    EXPECT_EQ(party->getNumVotes(), -2);
}

TEST_F(PartyTest, setLeftoverVotes) {
    EXPECT_EQ(party->getLeftoverVotes(), 0);
    party->setLeftoverVotes(2);
    EXPECT_EQ(party->getLeftoverVotes(), 2);

    party->setLeftoverVotes(-2);
    EXPECT_EQ(party->getLeftoverVotes(), -2);
}
