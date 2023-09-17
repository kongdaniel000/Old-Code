#ifndef SRC_FILE_PARSER_H_
#define SRC_FILE_PARSER_H_

#include <iostream>
#include <vector>
#include <fstream>

#include "ElectionObject.h"
#include "IRV.h"
#include "CPL.h"
#include "IRVAudit.h"
#include "CPLAudit.h"

class FileParser {
public:
    /**
     * @brief Destructor for FileParser, but close the file before destroying
     *
     */
    ~FileParser() {file.close();}

    /**
     * @brief Opens a file with the given name in FileParser.
     *
     * @param filename File to open
     * @return 0 if success, 1 if fail.
     */
    int openFile(std::string filename);

    /**
     * @brief Converts ballot string to an array to be used.
     *
     * @param ballot Ballot string to convert.
     * @param numCandidates Number of candidates in the IRV election.
     * @return a vector representation of the ballot string.
     */
    std::vector<int> ballotToArray(std::string ballot, int numCandidates);

    /**
     * @brief Read the ballots into a CPL object.
     *
     * @param election Pointer to a CPL object to read ballots into.
     */
    void readBallots(CPL* election);

    /**
     * @brief Read the ballots into a IRV object.
     *
     * @param election Pointer to a IRV object to read ballots into.
     */
    void readBallots(IRV* election);

    /**
     * @brief Get the day from the user
     *
     * @return a string representation of the day
     */
    std::string getDay();

    /**
     * @brief Get the month from the user
     *
     * @return a string representation of the month
     */
    std::string getMonth();

    /**
     * @brief Get the year from the user
     *
     * @return a string representation of the year
     */
    std::string getYear();

    /**
     * @brief Handles a IRV election.
     *
     * @param day Day from the user.
     * @param month Month from the user.
     * @param year Year from the user.
     * @return a pointer to an IRV object that represents the calculated winners of the election.
     */
    IRV* handleIRV(std::string day, std::string month, std::string year);

    /**
     * @brief Handles a CPL election.
     *
     * @param day Day from the user.
     * @param month Month from the user.
     * @param year Year from the user.
     * @return a pointer to an CPL object that represents the calculated winners of the election.
     */
    CPL* handleCPL(std::string day, std::string month, std::string year);

    /**
     * @brief Gets the metadata for the election.
     *
     * @return an election object that hasn't calculated the winners yet.
     */
    ElectionObject* getMetadata();
private:
    /**
     * @brief The stream that allows the file to be taken in to the program
     *
     */
    std::ifstream file;
};

#endif