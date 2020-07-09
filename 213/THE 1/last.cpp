#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <iostream>
#include <cstddef>
#include <stdexcept>
#include "Node.hpp"

using namespace std;

template <class T> 
class LinkedList {
    private:
        /* pointer to the first node */
        Node<T>* front;
        /* pointer to the last node */
        Node<T>* back;

    public:
        /* Do not modify the interface */

        LinkedList();
        LinkedList(const LinkedList<T>& ll);
        LinkedList<T>& operator=(const LinkedList<T>& ll);
        ~LinkedList();

        /* returns the first node of the linked list */
        Node<T>& getFront() const;
        /* returns the last node of the linked list */
        Node<T>& getBack() const;
        /* returns the node in given "pos"ition of the linked list */
        Node<T>& getNodeAt(int pos) const;
        /* returns the pointer of the node in given 
           "pos"ition of the linked list */
        Node<T>* getNodePtrAt(int pos) const;
        
        /* inserts a new node containing "data" 
           after the node "prev" 
           */
        void insert(Node<T>* prev, const T& data);
        /* inserts a new node containing "data" 
           at "pos"ition in the linked list 
           */
        void insertAt(int pos, const T& data);
        /* erases the given "node" from the linked list */
        void erase(Node<T>* node);
        /* erases the node in given "pos"ition from the linked list */
        void eraseFrom(int pos);
        /* clears the contents of the linked list */
        void clear();

        /* inserts a new node containing "data" 
           to the front of the linked list 
           */
        void pushFront(const T& data);
        /* inserts a new node containing "data" 
           to the back of the linked list
           */
        void pushBack(const T& data);

        /* removes the first node */
        void popFront();
        /* removes the last node */
        void popBack();

        /* returns true if the list is empty, false otherwise */
        bool isEmpty() const;
        /* returns the number of items in the list */
        size_t getSize() const;
        /* prints the contents of the linked list 
           one node data per line
           assumes the objects in the node have operator<< overloaded 
           */
        void print() const;

        /* Do not modify the interface */
};


template <class T>
LinkedList<T>::LinkedList(){
    front = NULL;
    back = NULL;
}

template <class T>
LinkedList<T>::LinkedList(const LinkedList<T>& rhs){

    front = NULL;
    back = NULL;

    const Node<T>* node = rhs.front;
    while (node!=NULL) {
        pushBack(node->getData());
        node = node->getNext();
    }
}

template <class T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList<T>&rhs){

    if (this != &rhs){
        clear();

        const Node<T>* node = rhs.front;
        while (node!=NULL) {
            pushBack(node->getData());
            node = node->getNext();
        }
    }
    return *this;
}

template <class T>
LinkedList<T>::~LinkedList(){
    clear();
}

template <class T>
Node<T>& LinkedList<T>::getFront() const{
    return *front;
}

template <class T>
Node<T>& LinkedList<T>::getBack() const{
    return *back;
}

template <class T>
Node<T>& LinkedList<T>::getNodeAt(int pos) const{
    return *getNodePtrAt(pos);
}

template <class T>
Node<T>* LinkedList<T>::getNodePtrAt(int pos) const{

    if (pos<0)
        throw invalid_argument("invalid index");

    Node<T>* node = front;
    for (int i = 0; i < pos && node!=NULL; ++i){
        node = node->getNext();
    }

    if (node==NULL)
        throw invalid_argument("invalid index");

    return node;
}

template <class T>
void LinkedList<T>::insert(Node<T>* prev, const T& data){

    Node<T>* node = new Node<T>(data);
    //cout << "INSERT" << endl;
    //cout << *node << endl;
    node->setPrev(prev);
    //cout << *node << endl;
    // first node
    if (front==NULL){ // back==NULL prev==NULL
        front = node;
        back = node;
        //cout << "first" << endl;
    }
    // front
    else if (prev == NULL){
        node->setNext(front);
        front->setPrev(node);
        front = node;
        //cout << "front" << endl;
    }
    // back
    else if (prev == back){
        prev->setNext(node);
        back = node;
        //cout << "back" << endl;
    }
    // middle
    else{
        node->setNext(prev->getNext());
        prev->getNext()->setPrev(node);
        prev->setNext(node);
        //cout << "middle" << endl;
    }
    //cout << *node << endl;
    //cout << "INSERT" << endl;
}

template <class T>
void LinkedList<T>::insertAt(int pos, const T& data){

    Node<T>* prev = pos == 0 ? NULL : getNodePtrAt(pos-1);
    insert(prev, data);
}

template <class T>
void LinkedList<T>::pushFront(const T& data){
    insert(NULL, data);
}

template <class T>
void LinkedList<T>::pushBack(const T& data){
    insert(back, data);
}

template <class T>
void LinkedList<T>::erase(Node<T>* node){

    Node<T>* prev = node->getPrev();
    Node<T>* next = node->getNext();
    // last node : prev == NULL && next == NULL
    //             node == front && node == back
    if (prev == NULL && next == NULL){
        front = NULL;
        back = NULL;
    }
    // front : prev == NULL or node == front
    else if (prev == NULL){
        front = next;
        next->setPrev(NULL);
    }
    // back : next == NULL or node == back
    else if (next == NULL){
        back = prev;
        prev->setNext(NULL);
    }
    // middle
    else{
        prev->setNext(next);
        next->setPrev(prev);
    }
    delete node;
}

template <class T>
void LinkedList<T>::eraseFrom(int pos){

    Node<T>* node = getNodePtrAt(pos);
    erase(node);
}

template <class T>
void LinkedList<T>::clear(){

    Node<T>* node = front;
    while (node) {
        Node<T>* tmp = node;
        node = node->getNext();
        delete tmp;
    }
    front = NULL;
    back = NULL;
}

template <class T>
void LinkedList<T>::popFront(){
    erase(front);
}

template <class T>
void LinkedList<T>::popBack(){
    erase(back);
}

template <class T>
bool LinkedList<T>::isEmpty() const{
    return front==NULL && back==NULL;
}

template <class T>
size_t LinkedList<T>::getSize() const{
    size_t size = 0;
    const Node<T>* node = front;
    while (node) {
        size++;
        node = node->getNext();
    }
    return size;
}

template <class T>
void LinkedList<T>::print() const{
    const Node<T>* node = front;
    while (node) {
        std::cout << node->getData() << std::endl;
        //std::cout << *node << std::endl;
        node = node->getNext();
    }
}

#endif