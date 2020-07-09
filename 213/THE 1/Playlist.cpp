#include "Playlist.hpp"
#include "Entry.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

Playlist::Playlist() { srand(15); }

int Playlist::getRandomNumber(int i, int n) const {
  int range = n - i;
  int random = rand() % range + i;
  return random;
}

void Playlist::print() {
  cout << "[PLAYLIST SIZE=" << entries.getSize() << "]";
  entries.print();
}
void Playlist::printHistory() {
  cout << "[HISTORY]";
  history.print();
}

/* TO-DO: method implementations below */

/*load the list of playlist entries from the given file.*/
void Playlist::load(std::string fileName) {
  ifstream playlist(fileName.c_str());
  if (playlist.is_open()) {
    string line;
    while (getline(playlist, line)) {
      string _title;
      string _genre;
      string _year;
      istringstream tokenizer(line);
      getline(tokenizer, _title, ';');
      getline(tokenizer, _genre, ';');
      getline(tokenizer, _year, ';');
      Entry *ele = new Entry(_title, _genre, _year);
      insertEntry(*ele);
    }
    playlist.close();
  }
}

/*Inserts a new entry to the end of playlist entries.
 *For UNDO operation, you should save the insert operation.*/
void Playlist::insertEntry(const Entry &e) {
  if (entries.findNode(e) == NULL) {
    Node<Entry> *newEntry = new Node<Entry>(e);
    entries.insertNode(entries.getTail(), newEntry->getData());
    Entry hist = newEntry->getData();
    Operation newOp = INSERT;
    HistoryRecord operation(newOp, hist);
    history.push(operation);
  }
}

/*Deletes the entry with given title from the list.
 *If the delete operation is successful (i.e. the entry with given title is in
 *the playlist and deleted successfully).. you should save the this  operation
 *for UNDO operation.*/
void Playlist::deleteEntry(const std::string &_title) {
  Node<Entry> *current = entries.getHead();
  Node<Entry> *current2 = NULL, *current3;
  while (current) {
    current3 = current2;
    current2 = current;
    if (current->getData().getTitle() == _title){
        break;
    }
      current = current->getNext();
  }
  Node<Entry> *newEntry = new Node<Entry>(current2->getData());
  Entry hist = newEntry->getData();
  Operation newOp = DELETE;
  HistoryRecord operation(newOp, hist);
  history.push(operation);
  entries.deleteNode(current3);
}

/*Moves the entry with given title to the left.*/
void Playlist::moveLeft(const std::string &title) {
    
    Node<Entry> *newNode = entries.getHead();
    Node<Entry> *newNextNode = entries.getHead()->getNext();
	while (newNextNode->getNext()) {
        if (newNextNode->getData().getTitle() == title && newNode == entries.getHead())
		{
			Entry newEntry(newNode->getData().getTitle(),
            newNode->getData().getGenre(), newNode->getData().getYear());
			entries.deleteNode(NULL);
			entries.insertNode(newNextNode, newEntry);
			break;
		}
		else if (newNextNode->getNext()->getData().getTitle() == title) {
			if (newNextNode->getNext()->getNext() == NULL) {
				entries.setTail(newNextNode);
			}
			newNode->setNext(newNextNode->getNext());
			newNextNode->setNext(newNextNode->getNext()->getNext());
			newNode->getNext()->setNext(newNextNode);
			break;
        }
        newNode = newNode->getNext();
        newNextNode = newNextNode->getNext();
}   
    

        // Node<Entry> *current = entries.getHead();
        // Node<Entry> *current2 = NULL, *current3 = NULL;
      
        // while (current->getNext()) {
        //     current3 = current2;
        //     current2 = current;
        //   if (current->getNext()->getData().getTitle() == title){
        //       break;
        //   }  
        //     current = current->getNext();
        // }
        // // cout<<"TEST"<<current3->getData()<<current2->getData()<<endl;
        // current3->setNext(current2->getNext());
        // current2->getNext()->setNext(current2);
        // current2->setNext(current2->getNext()->getNext());
    
}

/*Moves the entry with given title to the right.*/
void Playlist::moveRight(const std::string &title) {}

/*Reverse the playlist entries.*/
void Playlist::reverse() {}

/*Sort the entries of the playlist from lowest to highest according to their
 * “title”s.*/
void Playlist::sort() {
    Node<Entry> *i, *j;
    string temp;
    cout<<"TEST"<<entries.getSize()<<endl;
	for(i=entries.getHead();i!=NULL;i=i->getNext()){
	   for(j=i->getNext();j!=NULL;j=j->getNext()){
	        if(i->getData().getTitle() > j->getData().getTitle()){
                cout<<"TEST1"<<entries.getSize()<<endl;
                cout<<"TEST5"<<j->getData().getTitle()<<endl;
                cout<<"TEST3"<<i->getData().getTitle()<<endl;
                moveLeft(j->getData().getTitle());
                cout<<"TEST2"<<entries.getSize()<<endl;
	       }
	   }
    }
}

/*Merge the sorted lists while keeping the sort order*/
void Playlist::merge(const Playlist &pl) {}

/*Shuffle the playlist entries. Use getRandomNumber function to generate a
 * random number. */
void Playlist::shuffle() {}

/*UNDO the the operations*/
void Playlist::undo() {}
/*print the list entries. It is already implemented. DO NOT MODIFY
void print(){

}
  Prints the contents of the history stack from top to bottom.
void printHistory(){

}
*/
