#ifndef SRC_BALLOT_LINKED_LIST_H_
#define SRC_BALLOT_LINKED_LIST_H_

#include "BallotNode.h"
#include <vector>

/**
 * @brief File name: BallotLinkedList.h. Description: Contains method signatures and 
 * attributes for the BallotLinkedList class, which is a linked list data structure involved 
 * in IR voting. Author: Jacob.
 *
 */

class BallotLinkedList {
    public:
    BallotLinkedList();
    /**
    * @brief This function will remove the node currently add the head of the linked list and make the next node the head (if there is one).
    * 
    **/
    void removeHead();

    /**
    * @brief This function will add a new node containing the given integer to the end of the linked list.
    * 
    * @param candidate The index of the candidate in the Candidate array.
    **/
    void appendNode(int candidate);

    /**
    * @brief This will return the number that is stored in the head node of the linked list.
    * 
    * @return The index that is held in the head of the linked list.
    **/
    int getHead();

    /**
    * @brief This will create a linked list from an integer array. The head node will hold the first array entry and so on.
    * 
    * @param votes An integer array that represents the indices of the nodes in the list. This array is in the same order as the list should be.
    **/
    void populateList(std::vector<int> votes);

    /**
    * @brief This will free all of the nodes in the linked list.
    **/
    void cleanup();

    /**
    * @brief This returns the length attribute.
    * 
    * @return The length of the linked list.
    **/
    int getLength();

    /**
    * @brief This returns a pointer to the tail node. Used for testing.
    * 
    * @return A pointer to the tail node.
    **/
    BallotNode* getTail();

    private:
    BallotNode* head; //!< head node of the list
    BallotNode* tail; //!< tail node of the list
    int length; //!< length of the list
};

#endif  // SRC_BALLOT_LINKED_LIST_H_
