#ifndef SRC_FILE_PARSER_H_
#define SRC_FILE_PARSER_H_

#include <iostream>
#include <vector>
#include <fstream>

#include "ElectionObject.h"
#include "IRV.h"
#include "CPL.h"
#include "PO.h"
#include "IRVAudit.h"
#include "CPLAudit.h"
class FileParser {
public:
    /**
     * @brief Constructor for FileParser, initializes files vector
     * 
     */
    FileParser();

    /**
     * @brief Destructor for FileParser, closes all files before destroying
     * 
     */
    ~FileParser();

    /**
     * @brief Opens a file with the given name in FileParser.
     *
     * @param filename File to open
     * @param index Index into file vector which to use
     * @return 0 if success, 1 if fail. 
     */
    int openFile(std::string filename, int index);

    /**
     * @brief Opens multiple files with the given name in FileParser.
     * 
     * @param filename File to open
     * @return 0 if success, 1 if fail on one or more files. 
     */
    int openMultiple(std::vector<std::string> filenames);

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
     * @return number of ballots from the file.
     */
    void readBallots(CPL* election);

    /**
     * @brief Read the ballots into a IRV object.
     *
     * @param election Pointer to a IRV object to read ballots into.
     * @return number of ballots from the file.
     */
    void readBallots(IRV* election);

    /**
     * @brief Read the ballots into a PO object.
     *
     * @param election Pointer to a PO object to read ballots into.
     */
    void readBallots(PO* election);

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
     * @brief Handles a IRV election with multiple files.
     * 
     * @param day Day from the user.
     * @param month Month from the user.
     * @param year Year from the user.
     * @return a pointer to an IRV object that represents the calculated winners of the election. 
     */
    IRV* handleMultipleIRV(std::string day, std::string month, std::string year);

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
     * @brief Handles a CPL election with multiple files.
     * 
     * @param day Day from the user.
     * @param month Month from the user.
     * @param year Year from the user.
     * @return a pointer to an IRV object that represents the calculated winners of the election. 
     */
    CPL* handleMultipleCPL(std::string day, std::string month, std::string year);

    // /**
    //  * @brief Handles a PO election.
    //  * 
    //  * @param day Day from the user.
    //  * @param month Month from the user.
    //  * @param year Year from the user.
    //  * @return a pointer to an PO object that represents the calculated winners of the election. 
    //  */
    PO* handlePO(std::string day, std::string month, std::string year);

    /**
     * @brief Gets the metadata for the election.
     *
     * @return an election object that hasn't calculated the winners yet.
     */
    ElectionObject* getMetadata();

    /**
     * @brief Gets the metadata for the election from multiple files.
     * 
     * @return an election object that hasn't calculated the winners yet.
     */
    ElectionObject* getMultipleMetadata();
private:
    /**
     * @brief The stream that allows the files to be taken in to the program
     * 
     */
    std::vector<std::ifstream*> files;
};

#endif