#include <gtest/gtest.h>
#include "../src/ElectionEntity.h"

class ElectionEntityTest: public ::testing::Test { 
public:

    ElectionEntityTest(){}
    ~ElectionEntityTest(){}

    void SetUp() {
        // code here will execute just before the test ensues	
        electionEntity = new ElectionEntity();
    }

    void TearDown() {
        // code here will be called just after the test completes
        // ok to throw exceptions from here if need be
        delete electionEntity; 
    }

protected:
    // put in any custom data members that you need
    ElectionEntity *electionEntity;
};

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST_F(ElectionEntityTest, setNameTest) {
    electionEntity->setName("test1234");
    std::string name = electionEntity->getName();

    EXPECT_EQ(name, "test1234");
}