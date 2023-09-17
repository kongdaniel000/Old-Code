#ifndef SRC_IRV_H_
#define SRC_IRV_H_

#include <vector>

#include "ElectionObject.h"
#include "Candidate.h"
#include "BallotLinkedList.h"
class IRVAudit;

/**
* @brief File name: IRV.h. Description: Contains method signatures and 
* attributes for the IRV class, which carries out IRV processes. Author: Jacob.
*
*/

class IRV : public ElectionObject {
    public:
    /**
    * @brief Destructor for IRV, necessary to prevent memory leaks
    **/
    ~IRV() {delete audit;}

    /**
    * @brief Determines the winner of the election. Makes calls to report results to audit and screen.
    **/
    void calculateWinners() override;

    /**
    * @brief Redistributes votes from the loser to the rest of the non-eliminated candidates
    * 
    * @param loser The candidate that is being eliminated
    * @param inRunning Boolean array, representing candidates still in the running.
    * @param nLoop Number of redistribution we are on
    **/
    void redistribute(Candidate loser, std::vector<bool> inRunning, int nLoop);

    /**
    * @brief Returns the candidate that is being eliminated.
    * 
    * @return A candidate object; the candidate being eliminated
    **/
    Candidate getLoser(std::vector<bool> inRunning);

    /**
    * @brief Finds the candidate with the most votes, and sees if they have a majority.
    * 
    * @param inRunning Boolean array, representing candidates still in the running.
    * @return A null pointer if the max candidate did not have majority, and a pointer to the max candidate if they did.
    **/
    Candidate* maxCandWinCheck(std::vector<bool> inRunning);

    /**
    * @brief Determines the winner when two candidates remain.
    * 
    * @param inRunning Boolean array, representing candidates still in the running.
    * @return A pointer to the winning candidate object.
    **/
    Candidate* getWinnerTwoLeft(std::vector<bool> inRunning);

    /**
    * @brief Looks through ballots and sets each candidate's first place votes and working votes.
    **/
    void fillFirstPlaceVotes();

    /**
    * @brief Prints the results of the IRV election to the terminal.
    * 
    * @param winner The candidate that won the election.
    * @param redists The number of redistributions required to find the winner.
    **/
    void printResults(Candidate winner, int redists);

    /**
    * @brief Returns the Candidates field.
    * 
    * @return The candidate vector.
    **/
    std::vector<Candidate> getCandidates();

    /**
    * @brief Sets the Candidates field.
    * 
    * @param candidateArray The vector containing candidate objects
    **/
    void setCandidates(std::vector<Candidate> candidateArray);

    /**
    * @brief Sets the ballotList field.
    * 
    * @param linkedList The ballotLinkedList to set to the ballotList attribute.
    **/
    void setBallotList(std::vector<BallotLinkedList>* linkedList);

    /**
    * @brief Returns the ballotList field.
    * 
    * @return The ballotList field.
    **/
    std::vector<BallotLinkedList>* getBallotList();

    /**
    * @brief Sets the numCandidates field.
    * 
    * @param sum The numCandidates.
    **/
    void setNumCandidates(int sum);

    /**
    * @brief Sets the remainCandiates field.
    * 
    * @param newRemainCandidates the new field value for remainCandidates.
    **/
    void setRemainCandidates(int newRemainCandidates);

    /**
    * @brief Sets the audit field.
    * 
    * @param aud the new audit.
    **/
    void setAudit(IRVAudit* aud) {audit = aud;}

    /**
    * @brief Gets the remainCandiates field.
    * 
    * @return The remainCandidates field.
    **/
    int getRemainCandidates();

    /**
    * @brief Returns the numCandidates field.
    * 
    * @return The numCandidates field.
    **/
    int getNumCandidates();

    /**
    * @brief Returns the audit field.
    * 
    * @return The audit field.
    **/
    IRVAudit* getAudit() {return audit;}

    private:
    int numCandidates;  //!< The number of candidates in the election.
    int remainCandidates;   //!< The number of candidates that remain in the process of determining the winner.
    std::vector<Candidate> candidates;  //!< The candidate objects representing candidates in the election. 
    std::vector<BallotLinkedList>* ballotList;   //!< Vector of ballotLinkedLists, which each represent a ballot.
    IRVAudit* audit;
};

#endif  // SRC_IRV_H_
