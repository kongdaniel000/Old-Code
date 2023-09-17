//
// Created by caleb on 3/15/23.
//

#ifndef SRC_CPL_AUDIT_H_
#define SRC_CPL_AUDIT_H_

#include "ElectionAudit.h"
#include "Party.h"
class CPL;

/**
 * @brief File name: CPLAudit.h. Description: Contains method signatures
 * for the CPLAudit class, which defines the creation and writing to of the
 * CPL Audit file by inheriting from the ElectionAudit class.
 * Author: Caleb Otto.
*/

class CPLAudit: public ElectionAudit{
public:
    /**
     * @brief The overwritten function for writing the Metadata of the audit file
     * @param object the CPL election object
     */
    void writeMetadata(CPL* object);

    /**
     * @brief Writes the initial vote counts for all parties to the file
     * @param parties the vector containing all the parties in the election
     */
    void writeInitialVotes(std::vector<Party> parties);

    /**
     * @brief Writes the final results of how many seats each party won
     * @param parties the vector containing all the parties in the election
     */
    void writeFinalResults(std::vector<Party> parties);

    /**
     * @brief Writes how many seats were given to each party in the first allocations
     * @param parties the vector containing all the parties in the election
     * @param quota the number of seats available in the election
     */
    void writeFirstAllocation(std::vector<Party> parties, int quota);

    /**
     * @brief Writes how many seats were given to each party in the second allocation
     * @param parties the vector containing all the parties in the election
     */
    void writeSecondAllocation(std::vector<Party> parties);

    /**
     * @brief Writes if there was a seat lottery and it's outcome
     * @param source whichever party had excess seats
     * @param destination the party that won the excess seats
     */
    void writeLottery(Party source, Party destination);

    /**
     * @brief Writes to which party the ballot voted for
     * @param party the party the ballot is voting for
     */
    void writeBallot(Party party);

    /**
     * @brief Writes if there was a tiebreaker as well as it's details
     * @param original the original vector of parties before tiebreaker
     * @param winners the winner vector of parties following the tiebreaker
     */
    void writeTiebreaker(std::vector<ElectionEntity *> original, std::vector<ElectionEntity *> winners);

    void writeQuota(int quota);
};


#endif //SRC_CPLAUDIT_H

