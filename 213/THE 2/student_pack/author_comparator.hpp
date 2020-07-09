#ifndef _author_h__
#define _author_h__

#include "book.hpp"
#include <cstring>

class AuthorComparator
{
  public:
    bool operator( ) (const Book::SecondaryKey & key1, 
                      const Book::SecondaryKey & key2) const
    {
      //if key1 is less than key2 wrt specifications
      //return true
      //otherwise
      //return false
      
      if(key1.getAuthor()<key2.getAuthor()){
        return true;
      }
      else if(key1.getAuthor()==key2.getAuthor()){
          if(key1.getTitle()<key2.getTitle()){
            return true;
          }
          else{
            return false;
          }
      }
      else{
        return false;
      }
    }

};

#endif
