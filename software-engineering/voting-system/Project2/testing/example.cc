#include <gtest/gtest.h>
#include <iostream>

class ExampleFixture: public ::testing::Test { 
public:
    ExampleFixture() {  
        // initialization code here
    }	    

    void SetUp() {
        // code here will execute just before the test ensues	
    }

    void TearDown() {
        // code here will be called just after the test completes
        // ok to throw exceptions from here if need be
    }

    ~ExampleFixture() {
        // cleanup any pending stuff, but no exceptions allowed
    }
protected:
    // put in any custom data members that you need
};

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

TEST_F(ExampleFixture, test_name) {
    //do something here
}

