#ifndef SRC_PO_H_
#define SRC_PO_H_

#include <vector>

#include "ElectionObject.h"
#include "PO_Candidate.h"
#include "BallotLinkedList.h"

/**
* @brief File name: PO.h. Description: Contains method signatures and
* attributes for the PO class, which carries out PO processes. Author: Caleb Otto.
*
*/

class PO : public ElectionObject {
public:

    /**
    * @brief Determines the winner of the election. Makes calls to report results to audit and screen.
    **/
    void calculateWinners() override;

    /**
//    * @brief Determines the winner when two candidates remain.
//    *
//    * @param inRunning Boolean array, representing candidates still in the running.
//    * @return A pointer to the winning candidate object.
//    **/
//    POCandidate* getWinnerTwoLeft(std::vector<bool> inRunning);
//
//    /**
//    * @brief Prints the results of the IRV election to the terminal.
//    *
//    * @param winner The candidate that won the election.
//    * @param redists The number of redistributions required to find the winner.
//    **/
//    void printResults(POCandidate winner);

    /**
    * @brief Returns the Candidates field.
    *
    * @return The candidate vector.
    **/
    std::vector<POCandidate> getCandidates();

    /**
    * @brief Sets the Candidates field.
    *
    * @param candidateArray The vector containing candidate objects
    **/
    void setCandidates(std::vector<POCandidate> candidateArray);

    /**
    * @brief Sets the numCandidates field.
    *
    * @param sum The numCandidates.
    **/
    void setNumCandidates(int sum);

    /**
    * @brief Returns the numCandidates field.
    *
    * @return The numCandidates field.
    **/
    int getNumCandidates();


private:
    int numCandidates;  //!< The number of candidates in the election.
//    int remainCandidates;   //!< The number of candidates that remain in the process of determining the winner.
    std::vector<POCandidate> candidates;  //!< The candidate objects representing candidates in the election.
//    std::vector<BallotLinkedList>* ballotList;   //!< Vector of ballotLinkedLists, which each represent a ballot.
//    IRVAudit* audit;
};

#endif  // SRC_PO_H_
