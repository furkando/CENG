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
    // Node<T> *oldPointer = ll.getHead();
	// Node<T> *newNode;
	// for (unsigned int i = 0; i < ll.getSize() - 1; i++) {
	// 	newNode = new Node<T>(oldPointer->getData());
	// 	newNode->setNext(NULL);
	// 	this->setTail(newNode);
	// 	oldPointer->getNext();
	// }
    // this->getTail()->setNext(NULL);
    /*
    if (ll.head == NULL) {
        head = NULL;
    }
    else {
        head = new Node<T>(ll.head->getData());
        Node<T> *current = head;
        Node<T> *objHead = ll.head;
        Node<T> *currentObj = objHead;
        while (currentObj->getNext() != NULL) {
            current->setNext(new Node<T>(currentObj->getNext()->getData()));
            currentObj = currentObj->getNext();
            current = current->getNext();
        }
    }
*/
    
    head = NULL;
    tail = NULL;
    size=0;
    Node<T> tmp;
    Node<T> * o = ll.head,*n = &tmp;
    while (o) {
        n->setNext(new Node<T>(o->getData())) ; 
        size++;               
        n = n->getNext();
        o = o->getNext();
    }
    this->head = tmp.getNext(); 
    
}

template <class T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList<T>&ll){
    // Node<T> *oldPointer = ll.getHead();
	// Node<T> *newNode;
	// for (unsigned int i = 0; i < ll.getSize() - 1; i++) {
	// 	newNode = new Node<T>(oldPointer->getData());
	// 	newNode->setNext(NULL);
	// 	this->setTail(newNode);
	// 	oldPointer->getNext();
	// }
	// this->getTail()->setNext(NULL);
	// return *this;
    if (this != &ll){
        clear();
        Node<T> tmp;
        Node<T> * o = ll.head,*n = &tmp;
        while (o) {
            n->setNext(new Node<T>(o->getData())) ;       
            size++;         
            n = n->getNext();
            o = o->getNext();
        }
        this->head = tmp.getNext();
    }
    return *this;
    
    // if (this != &ll){
    //     clear();
        
    //     const Node<T>* node = ll.head;
    //     while (node) {
    //         insertNode(tail,node->getData());
    //         node = node->getNext();
    //     }
    // }
    // return *this; 
    
}

template <class T>
LinkedList<T>::~LinkedList()
{
    Node<T>* node = head;
    while (node!= NULL) {
        Node<T>* tmp = node;
        node = node->getNext();
        delete tmp;
    }
    head = NULL;
    tail = NULL;
    delete head;
    size=0;
}


template <class T>
Node<T>* LinkedList<T>::getHead() const{
    return head;
}

template <class T>
Node<T>* LinkedList<T>::getTail() const{
    return tail;
}

template <class T>
size_t LinkedList<T>::getSize() const{
    return size;
}

template <class T>
void LinkedList<T>::setHead(Node<T>* n){
    // if(size==0){
    //     head=n;
    //     tail=n;
    // }
    // else{
    //     n->setNext(head);
    //     head=n;
    // }
    head = n;
    size++;    
}

template<class T>
void LinkedList<T>::setTail(Node<T>* n){
    // if(size==0){
    //     head=n;
    //     tail=n;
    // }
    // else{
    //     tail->setNext(n);
    //     tail=n;
    // } 
    tail = n;
    size++;    
}

template<class T>
Node<T>* LinkedList<T>::findPrev(const T& data) const{
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
    {   // first
        head = newNode;
        tail = head;
    }
    else if(prev==NULL){
        // add head
        newNode->setNext(head);
        head=newNode;
    }
    else if(prev==tail){
        // add tail
        prev->setNext(newNode);
        tail = newNode;
    }
    else {
        
        newNode->setNext(prev->getNext());
        prev->setNext(newNode);
    }
    size++;
}

template<class T>
void LinkedList<T>::deleteNode(Node<T>* prevNode){
    Node<T>* tmp;


    if (prevNode) {

        if (prevNode->getNext()){
            prevNode->setNext(prevNode->getNext()->getNext());
        }else{
            prevNode->setNext(NULL);
            tail = prevNode;
        }
    }else {

        head  = head->getNext();
    }
    size--;

    /*
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
    size--; */
}

template<class T>
void LinkedList<T>::clear(){
    Node<T>* node = head;
    while (node!= NULL) {
        Node<T>* tmp = node;
        node = node->getNext();
        delete tmp;
    }
    head = NULL;
    tail = NULL;
    delete head;
    //delete node;
    size=0;
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
