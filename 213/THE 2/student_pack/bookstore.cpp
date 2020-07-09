#include "bookstore.hpp"

BookStore::BookStore( ) //implemented, do not change
{
}

void
BookStore::insert(const Book & book)
{
  primaryIndex.insert(book.getISBN(),book);
  BSTP::Iterator it = primaryIndex.find(book.getISBN());
  const Book &re = *it;
  SKey sKey = SKey(book);
  secondaryIndex.insert(sKey,&re);
  Book *ref = &(*it);
  ternaryIndex.insert(sKey,ref);
}

void
BookStore::remove(const std::string & isbn)
{
  BSTP::Iterator it1 = primaryIndex.find(isbn);
  const std::string title = (*it1).getTitle();
  const std::string author = (*it1).getAuthor();
  SKey a = SKey(title,author);
  secondaryIndex.remove(a);
  ternaryIndex.remove(a);
  primaryIndex.remove(isbn);
}

void
BookStore::remove(const std::string & title,
                  const std::string & author)
{
  SKey a =  Book::SecondaryKey(title,author);
  BSTT::Iterator it3 = ternaryIndex.find(a);
  const std::string isbn = (*(*it3)).getISBN();
  std::cout<<(*it3)<<std::endl;
  primaryIndex.remove(isbn);
  secondaryIndex.remove(a);
  ternaryIndex.remove(a);
}

void
BookStore::removeAllBooksWithTitle(const std::string & title)
{
  BSTP::Iterator it;
  for (it=primaryIndex.begin(); it != primaryIndex.end();)
  {
    std::string aut = title;
    std::string aut2 = (*it).getTitle();
    for(unsigned int i=0;i<aut.length();i++)
    {
        aut[i]=tolower(aut[i]);
    }
    for(unsigned int i=0;i<aut2.length();i++)
    {
        aut2[i]=tolower(aut2[i]);
    }
    if(aut==aut2){
      const std::string isbn = (*it).getISBN();
      const std::string title = (*it).getTitle();
      const std::string author = (*it).getAuthor();
      SKey a = SKey(title,author);
      secondaryIndex.remove(a);
      ternaryIndex.remove(a);
      primaryIndex.remove(isbn);
      it=primaryIndex.begin();
    }
    else{
      ++it;
    }
  }
}

void
BookStore::makeAvailable(const std::string & isbn)
{
  BSTP::Iterator it1 = primaryIndex.find(isbn);
  (*it1).setAvailable() ;
  // const std::string & title = (*it1).getTitle();
  // const std::string & author = (*it1).getAuthor();
  // SKey a = Book::SecondaryKey(title,author);
  // BSTT::Iterator it3 = ternaryIndex.find(a);
  // (*(*it3)).setAvailable(); 
}

void
BookStore::makeUnavailable(const std::string & title,
                           const std::string & author)
{
  SKey a =  Book::SecondaryKey(title,author);
  BSTT::Iterator it3 = ternaryIndex.find(a);
  (*(*it3)).setUnavailable();
  const std::string isbn = (*(*it3)).getISBN();
  BSTP::Iterator it1 = primaryIndex.find(isbn);
  (*it1).setUnavailable() ;
}

void
BookStore::updatePublisher(const std::string & author, 
                           const std::string & publisher)
{
  BSTP::Iterator it;
  for (it=primaryIndex.begin(); it != primaryIndex.end(); ++it)
  {
    std::string aut = author;
    std::string aut2 = (*it).getAuthor();
    for(unsigned int i=0;i<aut.length();i++)
    {
        aut[i]=tolower(aut[i]);
    }
    for(unsigned int i=0;i<aut2.length();i++)
    {
        aut2[i]=tolower(aut2[i]);
    }
    if(aut == aut2){
      (*it).setPublisher(publisher);
    }
  }
}

void
BookStore::printBooksWithISBN(const std::string & isbn1,
                              const std::string & isbn2,
                              unsigned short since) const
{
  BSTP::Iterator it;
  for (it=primaryIndex.begin(); it != primaryIndex.end(); ++it)
  {
    
    if((*it).getISBN()>=isbn1 && (*it).getISBN()<=isbn2){
      if((*it).getYear()>=since){
        std::cout << *it << std::endl;
      }
    }
  }
}

void
BookStore::printBooksOfAuthor(const std::string & author,
                              const std::string & first,
                              const std::string & last) const
{
  BSTP::Iterator it;
  for (it=primaryIndex.begin(); it != primaryIndex.end(); ++it)
  {
    std::string ib = (*it).getTitle();
    std::string f = first;
    std::string l = last;
    std::string aut = author;
    std::string aut2 = (*it).getAuthor();
    for(unsigned int i=0;i<aut.length();i++)
    {
        aut[i]=tolower(aut[i]);
    }
    for(unsigned int i=0;i<aut2.length();i++)
    {
        aut2[i]=tolower(aut2[i]);
    }
    for(unsigned int i=0;i<f.length();i++)
    {
        f[i]=tolower(f[i]);
    }
    for(unsigned int i=0;i<l.length();i++)
    {
        l[i]=tolower(l[i]);
    }
    for(unsigned int i=0;i<ib.length();i++)
    {
        ib[i]=tolower(ib[i]);
    }
    if(ib>=f && ib<=l){
      if(aut==aut2){
        std::cout << *it << std::endl;
      }
    }
  }
}

void //implemented, do not change
BookStore::printPrimarySorted( ) const
{
  BSTP::Iterator it;
  for (it=primaryIndex.begin(); it != primaryIndex.end(); ++it)
  {
    std::cout << *it << std::endl;
  }
}

void //implemented, do not change
BookStore::printSecondarySorted( ) const
{
  BSTS::Iterator it;

  for (it = secondaryIndex.begin(); it != secondaryIndex.end(); ++it)
  {
    std::cout << *(*it) << std::endl;
  }
}

void //implemented, do not change
BookStore::printTernarySorted( ) const
{
  BSTT::Iterator it;

  for (it = ternaryIndex.begin(); it != ternaryIndex.end(); ++it)
  {
    std::cout << *(*it) << std::endl;
  }
}

