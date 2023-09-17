#include <gtest/gtest.h>
#include <iostream>
#include "../src/BallotNode.h"
#include "../src/BallotNode.cc"

class BallotNodeTest: public ::testing::Test { 
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



TEST_F(BallotNodeTest, getNextTest) {
    BallotNode* node1 = new BallotNode();
    BallotNode* node2 = new BallotNode();
    node1->setIndex(3);
    node2->setIndex(5);
    node1->setNext(node2);
    EXPECT_EQ(node1->getNext()->getIndex(), 5);
    node2->setNext(node1);
    EXPECT_EQ(node2->getNext()->getIndex(), 3);
    delete node1;
    delete node2;
}

TEST_F(BallotNodeTest, getIndexTest) {
    BallotNode* node1 = new BallotNode();
    node1->setIndex(1);
    EXPECT_EQ(node1->getIndex(), 1);
    node1->setIndex(9999);
    EXPECT_EQ(node1->getIndex(), 9999);
    delete node1;
}

TEST_F(BallotNodeTest, setIndexTest) {
    BallotNode node;
    node.setIndex(7);
    EXPECT_EQ(node.getIndex(), 7);
    node.setIndex(-9000);
    EXPECT_EQ(node.getIndex(), -9000);
}

TEST_F(BallotNodeTest, setNextTest) {
    BallotNode* node1 = new BallotNode();
    BallotNode* node2 = new BallotNode();
    node1->setIndex(-100);
    node2->setIndex(500);
    node1->setNext(node2);
    EXPECT_EQ(node1->getNext()->getIndex(), 500);
    node2->setNext(node1);
    EXPECT_EQ(node2->getNext()->getIndex(), -100);
    delete node1;
    delete node2;
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}