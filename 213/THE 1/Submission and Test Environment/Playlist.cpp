#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "Playlist.hpp"
#include "Entry.hpp"

using namespace std;


Playlist::Playlist()
{
    srand(15);
}

int Playlist::getRandomNumber(int i, int n) const
{
    int range=n-i;
    int random = rand() % range + i;
    return random;
}

void Playlist::print()
{
    cout << "[PLAYLIST SIZE=" << entries.getSize() <<"]";
    entries.print();
}
void Playlist::printHistory()
{
    cout<<"[HISTORY]";
    history.print();
}

/* TO-DO: method implementations below */

/*load the list of playlist entries from the given file.*/
void Playlist::load(std::string fileName){

}

/*Inserts a new entry to the end of playlist entries. 
*For UNDO operation, you should save the insert operation.*/
void Playlist::insertEntry(const Entry &e){

}

/*Deletes the entry with given title from the list. 
*If the delete operation is successful (i.e. the entry with given title is in the playlist and deleted successfully)..
*you should save the this  operation for UNDO operation.*/
void Playlist::deleteEntry(const std::string &_title){

}

/*Moves the entry with given title to the left.*/
void Playlist::moveLeft(const std::string &title){

}

/*Moves the entry with given title to the right.*/
void Playlist::moveRight(const std::string &title){

}

/*Reverse the playlist entries.*/
void Playlist::reverse(){

}

/*Sort the entries of the playlist from lowest to highest according to their “title”s.*/
void Playlist::sort(){

}

/*Merge the sorted lists while keeping the sort order*/
void Playlist::merge(const Playlist & pl){

}

/*Shuffle the playlist entries. Use getRandomNumber function to generate a random number. */
void Playlist::shuffle(){

}

/*UNDO the the operations*/
void Playlist::undo(){

}
/*print the list entries. It is already implemented. DO NOT MODIFY
void print(){

}
  Prints the contents of the history stack from top to bottom.
void printHistory(){
    
}
*/

