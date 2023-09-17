//
// Created by caleb on 3/15/23.
//

#ifndef SRC_IRV_AUDIT_H_
#define SRC_IRV_AUDIT_H_

#include <fstream>
#include <vector>

#include "Candidate.h"
#include "ElectionAudit.h"
#include "IRV.h"
#include "ElectionObject.h"

/**
 * @brief File name: IRVAudit.h. Description: Contains method signatures
 * for the IRVAudit class, which defines the creation and writing to of the
 * IRV Audit file by inheriting from the ElectionAudit class.
 * Author: Caleb Otto.
*/

class IRVAudit: public ElectionAudit {
public:
    /**
     * @brief The overwritten function for writing the Metadata of the audit file
     * @param object the CPL election object
     */
    void writeMetadata(IRV* object);

    /**
     * @brief Writes the final results to the audit file
     * @param winner the winner of the election
     * @param percentage the percentage they won with
     */
    void writeFinalResults(Candidate winner, float percentage);

    /**
     * @brief Writes candidates that someone voted for in a ballot in their ranking
     * @param votes the ranking of votes
     * @param candidates the vector of candidates in the election
     */
    void writeBallot(std::vector<int> votes, std::vector<Candidate> candidates);

    /**
     * @brief Writes how the votes were redistributed in the election
     * @param candidates the vector of candidates in the election
     * @param running the vector of booleans indicating which candidates are still in the running
     * @param eliminated the vector of candidates who were eliminated
     * @param redist the number of redistribution that we are on
     */
    void writeVoteTransfer(std::vector<Candidate> candidates, std::vector<bool> running, Candidate eliminated, int redist);

    /**
     * @brief Writes the candidates who were tied for winning and who won
     * @param original the original vector of candidates before tiebreak
     * @param winner the winner of the tie breaker for lead
     */
    void writeTieLead(std::vector<ElectionEntity*> original, ElectionEntity* winner);

    /**
     * @brief Writes the candidates who were tied for losing and who won/who was eliminated
     * @param original the original vector of candidates before tiebreak
     * @param winner the winner of the tie breaker for least number of votes
     */
    void writeTieLose(std::vector<ElectionEntity*> original, ElectionEntity* winner);
};
#endif //SRC_IRVAUDIT_H


