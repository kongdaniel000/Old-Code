#ifndef PARTY_H
#define PARTY_H

#include <string>

#include "ElectionEntity.h"
#include <vector>

/**
* @brief File name: Candidate.h. Description: Contains method signatures and
* attributes for the Candidate class. Author: Ruolei.
*
*/

class Party: public ElectionEntity {
public:
    //constructors and destructors
    Party();
    ~Party();
    //setters
    /**
    * @brief increment the numVotes value by 1.
    * 
    **/    
    void incrementVotes() {numVotes++;}
    /**
    * @brief Set the firstSeats attribute.
    * 
    * @param seats the workingVotes to be set to
    **/    
    void setFirstSeats(int seats) {firstSeats = seats;}
    /**
    * @brief Set the secondSeats attribute.
    * 
    * @param seats the secondSeats to be set to
    **/      
    void setSecondSeats(int seats) {secondSeats = seats;}
    /**
    * @brief Set the candidateString attribute.
    * 
    * @param candidateString the candidateString to be set to
    **/     
    void setCandidates(std::string candidateString);
    /**
    * @brief Set the numVotes attribute.
    * 
    * @param num the numVotes to be set to
    **/     
    void setNumVotes(int num) {numVotes = num;}
    /**
    * @brief Set the leftoverVotes attribute.
    * 
    * @param num the leftoverVotes to be set to
    **/     
    void setLeftoverVotes(int num) {leftoverVotes = num;}
    //getters
    /**
    * @brief Returns the firstSeats attribute.
    * 
    * @return The firstSeats field.
    **/     
    int getFirstSeats() {return firstSeats;}
    /**
    * @brief Returns the secondSeats attribute.
    * 
    * @return The secondSeats field.
    **/  
    int getSecondSeats() {return secondSeats;}
    /**
    * @brief Returns the candidates attribute.
    * 
    * @return The candidates field.
    **/    
    std::string getCandidates() {return candidates;}
    /**
    * @brief Returns the numVotes attribute.
    * 
    * @return The numVotes field.
    **/    
    int getNumVotes() {return numVotes;}
    /**
    * @brief Returns the numCandidates attribute.
    * 
    * @return The numCandidates field.
    **/ 
    int getNumCandidates() { return numCandidates;}
    /**
    * @brief Returns the leftoverVotes attribute.
    * 
    * @return The leftoverVotes field.
    **/ 
    int getLeftoverVotes() {return leftoverVotes;}

//Party's attributes
private:
    int numVotes; //!< the number of votes
    int firstSeats; //!< the number of first seats
    int secondSeats; //!< the number of second seats
    std::string candidates; //!< the name of candidates 
    int numCandidates; //!< the number of candidates
    int leftoverVotes; // !< the left over votes
};

#endif 
