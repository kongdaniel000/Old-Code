#include "BallotLinkedList.h"
#include <iostream>
#include <vector>

/**
* @brief File name: BallotLinkedList.cc. Description: Contains method definitions 
* for the BallotLinkedList class, which is a linked list data structure involved 
* in IR voting. Author: Jacob.
*
*/

BallotLinkedList::BallotLinkedList() {
    head = nullptr; 
    tail = nullptr;
    length = 0;
}

void BallotLinkedList::removeHead() {
    if (head == nullptr) {                        // no head to remove
        return;
    }
    else {
        BallotNode* temp = head->getNext();     // free the head and make head's next the new head
        delete head;
        head = temp;
        length--;
        if (length == 0) {                     // tail should be null if no nodes left
            tail = nullptr;
        }
        return;
    }
}

void BallotLinkedList::appendNode(int candidate) {
    BallotNode* newNode = new BallotNode();
    if (head == nullptr) {                     // new node is only node
        head = newNode;
        tail = newNode;
    }
    else {                                     
        tail->setNext(newNode);                // old tail will point to new node (the new tail)
        tail = newNode;
    }
    newNode->setIndex(candidate);              // give the node its data
    length++;
    return;
}

int BallotLinkedList::getHead() {
    if (head == nullptr) {
        return -1;                             // return error valuw
    }
    else {
        return head->getIndex();        
    }
}

void BallotLinkedList::populateList(std::vector<int> votes) {
    for (int i = 0; i < votes.size(); i++) {
        if (votes[i] == -1) {                         // no need to add more nodes when we find a -1
            break;
        }
        else {
            appendNode(votes[i]);
        }
    }
    return;
}

void BallotLinkedList::cleanup() {
    BallotNode* ptr = head;              
    BallotNode* temp = nullptr; 
    while (ptr != nullptr) {              // delete each node, use temp to store next node to free
        temp = ptr->getNext();
        delete ptr;
        ptr = temp;
    }
    tail = nullptr;
    length = 0;
    return;
}

int BallotLinkedList::getLength() {
    return length;
}

BallotNode* BallotLinkedList::getTail() {
    return tail;
}