#ifndef PO_CANDIDATE_H_
#define PO_CANDIDATE_H_

#include <string>
#include <vector>

#include "ElectionEntity.h"

/**
* @brief File name: PO_Candidate.h. Description: Contains method signatures and
* attributes for the PO Candidate class. Author: Caleb.
*
*/
class POCandidate: public ElectionEntity {
    //candidate's attributes
private:
    int workingVotes; //!< the number of all votes
    std::vector<int> gainedVotes; //!< the number of votes the every candidates received
    int index; //!< the index of the candidate
    int initialFirstVotes; //!< the number of the initialFirstVotes in the Candidate
    char party;
public:
    //constructor
    POCandidate(int numCandidates);
    //setters and getters

    int getWorkingVotes();
    /**
    * @brief Returns the index attribute.
    *
    * @return The index field.
    **/
    int getIndex();

    /**
    * @brief Sets the index field.
    *
    * @param newIndex the index to be set to.
    **/
    void setIndex(int newIdx);

    /**
    * @brief Returns the initialFirstVotes attribute.
    *
    * @return The initialFirstVotes field.
    **/
    int getInitialFirstVotes();

    /**
    * @brief increment the initialFirstVotes value.
    *
    * @param votes the initialFirstVotes to be added to
    **/
    void incInitialFirstVotes(int votes = 1);

    /**
    * @brief Set the workingVotes attribute.
    *
    * @param newWorkingVotes the workingVotes to be set to
    **/
    void setWorkingVotes(int newWorkingVotes);
    //destructor

    void setParty(char newparty);

    char getParty();

    ~POCandidate();
};

#endif