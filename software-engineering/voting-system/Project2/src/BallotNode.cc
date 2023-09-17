#include "BallotNode.h"

/**
* @brief File name: BallotNode.cc. Description: Contains method definitions
* for the BallotNode class, which is a node in a BallotLinkedList. Author: Jacob.
*
*/

BallotNode* BallotNode::getNext() {
    return next;
}

int BallotNode::getIndex() {
    return index;
}

void BallotNode::setNext(BallotNode* newNode) {
    next = newNode;
    return;
}

void BallotNode::setIndex(int newIndex) {
    index = newIndex;
    return;
}