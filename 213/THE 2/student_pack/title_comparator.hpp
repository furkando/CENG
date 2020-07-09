#ifndef title_h_
#define title_h_

#include "book.hpp"
#include <cstring>

class TitleComparator
{
  public:
    bool operator( ) (const Book::SecondaryKey & key1, 
                      const Book::SecondaryKey & key2) const
    {
     
      //if key1 is less than key2 wrt specifications
      //return true
      //otherwise
      //return false
      if(key1.getTitle()<key2.getTitle()){
        return true;
      }
      else if(key1.getTitle()==key2.getTitle()){
        if(key1.getAuthor()<key2.getAuthor()){
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
