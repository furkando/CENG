#ifndef _LINKEDLIST_H_
#define _LINKEDLIST_H_

#include <iostream>
#include "Node.hpp"

using namespace std;

/*....DO NOT EDIT BELOW....*/
template <class T> 
class LinkedList {
    private:
		/*First node of the linked-list*/
        Node<T>* head;
        /*Last node of the linked-list*/
		Node<T>* tail;
		/*size of the linked-list*/
		size_t  size; 
    public:

        LinkedList();
        LinkedList(const LinkedList<T>& ll);
        LinkedList<T>& operator=(const LinkedList<T>& ll);
        ~LinkedList();

        /* Return head of the linked-list*/
        Node<T>* getHead() const;
        /* Set head of the linked-list*/
        void setHead(Node<T>* n);
        /* Return tail of the linked-list*/
        Node<T>* getTail() const;
        /* Set tail of the linked-list*/
        void setTail(Node<T>* n);
        /* Get the previous node of the node that contains the data item. 
         * If the head node contains the data item, this method returns NULL.*/
        Node<T>* findPrev(const T& data) const;
        /* Get the node that stores the data item. 
         * If data is not found in the list, this function returns NULL.*/
        Node<T>* findNode(const T& data) const;
        /* Insert a new node to store the data item. 
         * The new node should be placed after the “prev” node. 
         * If prev is NULL then insert new node to the head.*/
        void insertNode(Node<T>* prev, const T& data); 
        /* This method is used to delete the node that is next to “prevNode”. 
         * PS:prevNode is not the node to be deleted. */
        void deleteNode(Node<T>* prevNode);  
        /* This method is used to clear the contents of the list.*/
        void clear();
        /* This method returns true if the list empty, otherwise returns false.*/
        bool isEmpty() const;
        /* This method returns the current size of the list. */
        size_t getSize() const;
        /*Prints the list. This method was already implemented. Do not modify.*/
        void print() const;
};

template <class T>
void LinkedList<T>::print() const{
    const Node<T>* node = head;
    while (node) {
        std::cout << node->getData();
        node = node->getNext();
    }
    cout<<std::endl;
}

/*....DO NOT EDIT ABOVE....*/

/* TO-DO: method implementations below */

template <class T>
LinkedList<T>::LinkedList(){
    head=NULL;
    tail=NULL;
    size=0;
}


template <class T>
LinkedList<T>::LinkedList(const LinkedList<T>& ll){
    Node<T> *tmp;
    Node<T> * o = ll.head,*n = tmp;
    while (o) {
        n->setNext(new Node<T>(o->getData())) ;
        n = n->getNext();
        o = o->getNext();
    }
    this->head = tmp->getNext();
}

template <class T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList<T>& ll){
    if (this != &ll){
        clear();
        
        const Node<T>* node = ll.head;
        while (node) {
            insertNode(tail,node->getData());
            node = node->getNext();
        }
    }
    return *this;
}



template <class T>
LinkedList<T>::~LinkedList()
{
    
    Node<T> * curr = head;
    while (head)
    {
        head = head->getNext();
        delete curr;
        curr = head;
    }
    
}


template <class T>
Node<T> * LinkedList<T>:: getHead() const{
    return head;
}


template <class T>
Node<T> * LinkedList<T>:: getTail() const{
    return tail;
}

template <class T>
size_t LinkedList<T>:: getSize() const{
    return size;
}

template <class T>
void LinkedList<T>::setHead(Node<T>* n){
    if(size==0){
        head=n;
        tail=n;
        size++;
    }
    else{
        n->setNext(head);
        head=n;
        size++;
    }
}

template<class T>
void LinkedList<T>::setTail(Node<T>* n){
    if(size==0){
        head=n;
        tail=n;
        size++;
    }
    else{
        tail->setNext(n);
        tail=n;
        size++;
    }
}

template<class T>
Node<T> * LinkedList<T>:: findPrev(const T& data) const{
    Node<T>* previousNode ;
    Node<T>* currentNode = head;
    if(currentNode->getData()==data){
        return NULL;
    }
    while(currentNode != NULL ){
        previousNode = currentNode;
        currentNode = currentNode->getNext();
        if(currentNode->getData()==data){
            return previousNode;
        }
    }
    
}

template<class T>
Node<T> * LinkedList<T>:: findNode(const T& data) const{
    Node<T>* current = head;
    while (current != NULL)
    {
        if (current->getData() == data)
            return current;
        current = current->getNext();
    }
    return NULL;
}

template<class T>
void LinkedList<T>::insertNode(Node<T>* prev, const T& data){
    Node<T>* newNode = new Node<T>(data);
    if (head == NULL)
    {
        head = newNode;
        tail = head;
    }
    else if(prev==NULL){
        newNode->setNext(head);
        head=newNode;
    }
    else {
        Node<T>* nexto = prev->getNext();
        newNode->setNext(nexto);
        prev->setNext(newNode);
        
        if (newNode->getNext() == NULL){
            tail = newNode;
        }
        
    }
    size++;
}

template<class T>
void LinkedList<T>::deleteNode(Node<T>* prevNode){
    Node<T>* tmp;
    if (prevNode==tail){
        return ;
    }
    else {
        tmp = prevNode->getNext();
        if(tmp==tail){
            delete tmp;
            tail=prevNode;
        }
        else{
            prevNode->setNext(tmp->getNext());
            delete tmp;
        }
    }
}

template<class T>
void LinkedList<T>::clear(){
    Node<T>* tmp;
    while(head != NULL){
        if(head->getNext()!=NULL and tail!=head){
            tmp = head->getNext();
            delete head;
            head = tmp;
        }
        else {
            tail = NULL;
            head = NULL;
            delete head;
        }
    }
}

template<class T>
bool LinkedList<T>::isEmpty() const{
    if (head == NULL)
        return 1;
    else
        return 0;
}

/* end of your implementations*/

#endif
