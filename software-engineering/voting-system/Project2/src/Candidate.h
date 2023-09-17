#ifndef CANDIDATE_H_
#define CANDIDATE_H_

#include <string>
#include <vector>

#include "ElectionEntity.h"

/**
* @brief File name: Candidate.h. Description: Contains method signatures and 
* attributes for the Candidate class. Author: Ruolei.
*
*/
class Candidate: public ElectionEntity {
    //candidate's attributes
    private:
        int workingVotes; //!< the number of all votes
        std::vector<int> gainedVotes; //!< the number of votes the every candidates received
        std::vector<int> exhausted;
        int index; //!< the index of the candidate
        int initialFirstVotes; //!< the number of the initialFirstVotes in the Candidate
    public:
        //constructor
        Candidate(int numCandidates);
        //setters and getters
        /**
        * @brief increment the workingVotes
        * @param amt the workingVotes to be add to 
        * @return void.
        **/
        void incWorkingVotes(int amt = 1);
        /**
        * @brief Returns the workingVotes attribute.
        * 
        * @return The workingVotes field.
        **/ 
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
        * @brief increment the gainedVotes value at index.
        * 
        * @param votes the gainedVotes array index 
        **/
        void incGainedVotes(int roundIndex);

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
        * @brief Returns the gainedVotes attribute.
        * 
        * @return The gainedVotes field.
        **/          
        std::vector<int> getGainedVotes();

        /**
        * @brief Sets the gainedVotes attribute at a certain index to -1.
        * 
        * @param index The index to set in gained votes
        **/ 
        void setGainedVotes(int index);
        /**
        * @brief Set the workingVotes attribute.
        * 
        * @param newWorkingVotes the workingVotes to be set to
        **/
        void setWorkingVotes(int newWorkingVotes);
    //destructor
        ~Candidate();
};

#endif
