#include <gtest/gtest.h>
#include <iostream>
#include "fstream"
#include "../src/ElectionAudit.h"

class ElectionAuditTest: public ::testing::Test {
public:

    void setUp() {

    }

    void TearDown() {

    }

protected:
};

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

//Really not sure on this test, I was trying to find info on how to test
//an fstream but there's not much
TEST_F(ElectionAuditTest, setAuditFileTest) {
    std::streambuf *psbuf, *backup;
    std::ofstream* filestr = new std::ofstream;
    filestr->open ("test.txt");
    backup = filestr->rdbuf(); //backup filestr streambuf
    psbuf = std::cout.rdbuf(); //get std::out's streambuf
    filestr->basic_ios<char>::rdbuf(psbuf); //assign cout to streambuf
    ElectionAudit *auditFile = new ElectionAudit;
    auditFile->setAuditFile(filestr);
    testing::internal::CaptureStdout();
    *(auditFile->getAuditFile()) << "yessir";
    std::string output = testing::internal::GetCapturedStdout();
    std::string tester = "yessir";
    filestr->basic_ios<char>::rdbuf(backup);        // restore cout's original streambuf
    filestr->close();
    EXPECT_EQ(tester, output);
}

