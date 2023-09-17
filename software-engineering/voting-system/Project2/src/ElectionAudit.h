//
// Created by caleb on 3/15/23.
//

#ifndef SRC_ELECTION_AUDIT_H_
#define SRC_ELECTION_AUDIT_H_

#include "fstream"

class ElectionObject;

/**
 * @brief File name: ElectionAudit.h. Description: Contains method signatures for the
 * abstract ElectionAudit class, which is the class that both the CPL and IRV audit
 * classes inherit from for making the audit file.
 * Author: Caleb Otto.
*/

class ElectionAudit {
public:
    virtual ~ElectionAudit() {auditFile->close(); delete auditFile;}
    void setAuditFile(std::ofstream* aud) {auditFile = aud;}
    std::ofstream* getAuditFile() {return auditFile;}
protected:
    std::ofstream* auditFile;
};
#endif //SRC_ELECTIONAUDIT_H