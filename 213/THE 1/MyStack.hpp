#ifndef MYSTACK_HPP
#define MYSTACK_HPP
#include "Node.hpp"

/*You are free to add private/public members inside this template..*/
template <class T>
class MyStack{
    private:
    Node<T> *top;                
    public:
    /*Default constructor*/
    MyStack();                     					
    /*copy constructor*/
    MyStack(const MyStack<T>& rhs);  				
    /*destructor*/
    ~MyStack(); 
    /*overloaded = operator*/
    MyStack<T>& operator=(const MyStack<T>& rhs);  	
    /*returns true if stack is empty*/
    bool isEmpty() const;
    /*push newItem to stack*/
    void push(const T& newItem);
    /*pop item from stack*/
    void pop();
    /*return top item of the stack*/
    Node<T>* Top() const;
	/*Prints the stack entries. This method was already implemented. Do not modify.*/
    void print() const;
    /* This method is used to clear the contents of the list.*/
    void clear();
};

template <class T>
void MyStack<T>::print() const{
    const Node<T>* node = top;
    while (node) {
        std::cout << node->getData();
        node = node->getNext();
    }
    cout<<std::endl;
}

/* TO-DO: method implementations below */

template <class T>
MyStack<T>::MyStack(){
    top=NULL;
}

template <class T>
MyStack<T>::MyStack(const MyStack<T>& rhs){
Node<T> tmp;
        Node<T> * o = rhs.top,*n = &tmp;
        while (o) {
            n->setNext(new Node<T>(o->getData())) ;                
            n = n->getNext();
            o = o->getNext();
        }
        this->top = tmp.getNext();   
}

template <class T>
MyStack<T>::~MyStack(){
    while ( !isEmpty() ) {
        pop();
    }
}
template <class T>
MyStack<T>& MyStack<T>::operator=(const MyStack<T>& rhs){
    if (this != &rhs){
        clear();
        Node<T> tmp;
        Node<T> * o = rhs.top,*n = &tmp;
        while (o) {
            n->setNext(new Node<T>(o->getData())) ;      
            n = n->getNext();
            o = o->getNext();
        }
        this->top = tmp.getNext();
    }
    return *this;
}

template <class T>
bool MyStack<T>::isEmpty() const{
    if (top == NULL) {
        return true;
    }
    else {
        return false;
    }
}

template <class T>
void MyStack<T>::push(const T& newItem) {
    Node<T> * newNode = new Node<T>(newItem);
    newNode->setNext(top);
    top=newNode;
}

template <class T>
void MyStack<T>::pop() {
    Node<T>* tmp = top;
    top = top->getNext();
    delete tmp;
}

template <class T>
Node<T>* MyStack<T>::Top() const{
    return top;
}

template <class T>
void MyStack<T>::clear(){
    Node<T>* node = top;
    while (node!= NULL) {
        Node<T>* tmp = node;
        node = node->getNext();
        delete tmp;
    }
    top = NULL;
    delete top;
}

#endif /* MYSTACK_HPP */

