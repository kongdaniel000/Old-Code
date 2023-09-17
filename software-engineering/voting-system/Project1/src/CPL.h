#ifndef SRC_CPL_H_
#define SRC_CPL_H_

#include <iostream>
#include <vector>
#include <fstream>

#include "ElectionObject.h"
#include "CPLAudit.h"
#include "Party.h"
class CPL: public ElectionObject {
public:
    /**
    * @brief Destructor for CPL, necessary to prevent memory leaks
    **/
    ~CPL() {delete audit;}

    /**
     * @brief Lotteries seats from a party that has too many
     * 
     * @param allowed The allowed parties to receive a seat
     * @param source The party the seats are coming from
     * @param extraSeats How many seats are being lotteried
     * @param isRandom If the lottery is random or deterministic, defaults to true.
     */
    void seatLottery(std::vector<bool>* allowed, Party source, int extraSeats, bool isRandom = true);
    
    /**
     * @brief Does the first round of allocation of CPL.
     * 
     * @param allowed A vector representing if a party is allowed to have anymore seats
     * @param quota The quota for CPL
     */
    void firstAllocation(std::vector<bool>* allowed, int quota);

    /**
     * @brief Does the second round of allocation of IRV.
     * 
     * @param allowed A vector representing if a party is allowed to have anymore seats
     * @param isRandom If the lottery inside the second round is random or deterministic, defaults to true.
     */
    void secondAllocation(std::vector<bool>* allowed, bool isRandom = true);

    /**
     * @brief Prints the results of the CPL election to the screen.
     * 
     */
    void printResults();

    /**
     * @brief Calculates the who gets how many seats for the CPL election, then prints results.
     * 
     */
    void calculateWinners();

    /**
     * @brief Set numParties.
     * 
     * @param num Value to set numParties to.
     */
    void setNumParties(int num) {numParties = num;}

    /**
     * @brief Set the parties object.
     * 
     * @param partyArray Party vector to set parties to.
     */
    void setParties(std::vector<Party> partyArray) {parties = partyArray;}

    /**
     * @brief Set seats.
     * 
     * @param num Value to set seats to.
     */
    void setSeats(int num) {totalSeats = num; seatsLeft = num;}

    /**
     * @brief Set the audit object.
     * 
     * @param aud Pointer to set the audit to.
     */
    void setAudit(CPLAudit* aud) {audit = aud;}

    /**
     * @brief Get the numParties.
     * 
     * @return the number of parties. 
     */
    int getNumParties() {return numParties;}

    /**
     * @brief Get the parties object.
     * 
     * @return the parties vector.
     */
    std::vector<Party> getParties() {return parties;}

    /**
     * @brief Get the number of total seats.
     * 
     * @return the total seats.
     */
    int getSeats() {return totalSeats;}

    /**
     * @brief Get the audit object.
     * 
     * @return the CPLAudit pointer.
     */
    CPLAudit* getAudit() {return audit;}

private:
    /**
     * @brief Number of parties in CPL election.
     * 
     */
    int numParties;
    
    /**
     * @brief Vector containing all the parties in the election.
     * 
     */
    std::vector<Party> parties;

    /**
     * @brief Total seats to be gotten in the election.
     * 
     */
    int totalSeats;

    /**
     * @brief Number of seats left to be given out.
     * 
     */
    int seatsLeft;

    /**
     * @brief Pointer to an audit object that writes to the audit file.
     * 
     */
    CPLAudit* audit;
};

#endif