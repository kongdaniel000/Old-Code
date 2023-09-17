#ifndef SRC_BALLOT_NODE_H_
#define SRC_BALLOT_NODE_H_

/**
* @brief File name: BallotNode.h. Description: Contains method signatures and 
* attributes for the BallotNode class, which is a node in a BallotLinkedList. Author: Jacob.
*
*/

class BallotNode {
    public: 

    /**
    * @brief Returns the next attribute.
    * 
    * @return The next field.
    **/
    BallotNode* getNext();

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
    void setIndex(int newIndex);

    /**
    * @brief Sets the next pointer attribute.
    * 
    * @param newNode pointer to the new node to be the next field.
    **/
    void setNext(BallotNode* newNode);

    private: 
    BallotNode* next = nullptr; //!< pointer to the next node in the list
    int index = -1; //!< the index of the candidate in the Candidate array/vector
}; 
#endif  // SRC_BALLOT_NODE_H_