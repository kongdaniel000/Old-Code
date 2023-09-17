#include <gtest/gtest.h>
#include <iostream>
#include "../src/BallotLinkedList.h"
#include "../src/BallotLinkedList.cc"
#include "../src/BallotNode.cc"

class BallotLinkedListTest: public ::testing::Test { 
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

TEST_F(BallotLinkedListTest, removeHeadTest) {
    BallotLinkedList list;
    list.appendNode(1);
    list.appendNode(2);
    list.appendNode(3);
    list.removeHead();
    EXPECT_EQ(list.getLength(), 2);
    EXPECT_EQ(list.getHead(), 2);
    EXPECT_EQ(list.getTail()->getIndex(), 3);
    list.removeHead();
    EXPECT_EQ(list.getLength(), 1);
    EXPECT_EQ(list.getHead(), 3);
    EXPECT_EQ(list.getTail()->getIndex(), 3);
    list.removeHead();
    EXPECT_EQ(list.getLength(), 0);
    EXPECT_EQ(list.getHead(), -1);
    EXPECT_EQ(list.getTail(), nullptr);
    list.removeHead();
    EXPECT_EQ(list.getLength(), 0);
}

TEST_F(BallotLinkedListTest, appendNodeTest) {
    BallotLinkedList list;
    list.appendNode(1);
    EXPECT_EQ(list.getLength(), 1);
    EXPECT_EQ(list.getHead(), 1);
    EXPECT_EQ(list.getTail()->getIndex(), 1);
    list.appendNode(2);
    EXPECT_EQ(list.getLength(), 2);
    EXPECT_EQ(list.getHead(), 1);
    EXPECT_EQ(list.getTail()->getIndex(), 2);
}

TEST_F(BallotLinkedListTest, getHeadTest) {
    BallotLinkedList list; 
    EXPECT_EQ(list.getHead(), -1);
    list.appendNode(1);
    EXPECT_EQ(list.getHead(), 1);
    list.appendNode(2);
    EXPECT_EQ(list.getHead(), 1);
    list.removeHead();
    EXPECT_EQ(list.getHead(), 2);
    list.removeHead();
    EXPECT_EQ(list.getHead(), -1);
}

TEST_F(BallotLinkedListTest, populateListTest) {
    std::vector<int> vector1{1,2,3};
    std::vector<int> vector2{1,2,-1};
    std::vector<int> vector3{-1,-1};
    BallotLinkedList list;
    list.populateList(vector1);
    EXPECT_EQ(list.getLength(), 3);
    EXPECT_EQ(list.getHead(), 1);
    EXPECT_EQ(list.getTail()->getIndex(), 3);
    list.cleanup();
    list.populateList(vector2);
    EXPECT_EQ(list.getLength(), 2);
    EXPECT_EQ(list.getHead(), 1);
    EXPECT_EQ(list.getTail()->getIndex(), 2);
    list.cleanup();
    list.populateList(vector3);
    EXPECT_EQ(list.getLength(), 0);
    EXPECT_EQ(list.getHead(), -1);
    EXPECT_EQ(list.getTail(), nullptr);
}

TEST_F(BallotLinkedListTest, cleanupTest) {
    BallotLinkedList list;
    list.appendNode(1);
    list.appendNode(2);
    list.cleanup();
    EXPECT_EQ(list.getLength(), 0);
    EXPECT_EQ(list.getHead(), -1);
    EXPECT_EQ(list.getTail(), nullptr);
}

TEST_F(BallotLinkedListTest, getLengthTest) {
    BallotLinkedList list;
    EXPECT_EQ(list.getLength(), 0);
    list.appendNode(1);
    EXPECT_EQ(list.getLength(), 1);
    list.appendNode(2);
    EXPECT_EQ(list.getLength(), 2);
    list.removeHead();
    EXPECT_EQ(list.getLength(), 1);
    list.removeHead();
}

TEST_F(BallotLinkedListTest, getTailTest) {
    BallotLinkedList list;
    EXPECT_EQ(list.getTail(), nullptr);
    list.appendNode(1);
    EXPECT_EQ(list.getTail()->getIndex(), 1);
    list.appendNode(2);
    EXPECT_EQ(list.getTail()->getIndex(), 2);
    list.removeHead();
    EXPECT_EQ(list.getTail()->getIndex(), 2);
    list.removeHead();
    EXPECT_EQ(list.getTail(), nullptr);
}