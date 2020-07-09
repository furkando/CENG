#ifndef _BIN_SEARCH_TREE_H_
#define _BIN_SEARCH_TREE_H_

#include <functional>
#include <cstddef>
#include <stack>
#include <list>
#include <ostream>

//do not add any other library
//modify parts as directed by assignment text & comments here

template <typename Key, typename Object, 
          typename Comparator=std::less<Key> >
class BinarySearchTree 
{
  private: //do not change
    struct BinaryNode //node definition: a dependent type
    {
      Key key; //keys will be kept constant
      Object data; //objects that are referred to by keys may change
      BinaryNode * left;
      BinaryNode * right;
      size_t height; //height info should be updated per insert & delete

      BinaryNode(const Key &, const Object &,
                 BinaryNode *, BinaryNode *, size_t =0);
    };

  public: //do not change except for your own private utility functions
    class Iterator //iterator class will encapsulate the location within the BST
    {
      public:
        Iterator( ); //dummy constructor for type declaration purposes
        Iterator & operator++( ); //inorder increment
 
        Object & operator*( );  //update data
        const Object & operator*( ) const; //view data

        bool operator==(const Iterator & rhs) const; //compare iterators
        bool operator!=(const Iterator & rhs) const; //compare iterators
      
      private:
        BinaryNode * current; //position
        const BinaryNode * root; //for error check not implemented
        std::stack<BinaryNode *> s; //will be used to conduct in order traversal if
        bool useStack; //this variable is set to true, ignored in == and !=
        
      private:
        Iterator(BinaryNode *, const BinarySearchTree &, bool =true);
        //other private utility functions can be declared by you

      friend class BinarySearchTree<Key, Object, Comparator>; 
    };

  public: //do not change
    BinarySearchTree( ); //empty tree
    ~BinarySearchTree( ); //reclaim all dyn allocated mem

    void insert(const Key &, const Object &); //to insert new key,item
    void remove(const Key &); //remove the node with the key value (and also data) 

  public: //do not change
    Iterator find(const Key &) const; //single item
    std::list<Iterator> find(const Key &, const Key &) const;//range queries

    Iterator begin( ) const; //inorder begin
    Iterator end( ) const; //dummy NULL iterator

  public: //do not change
    int height( ) const; //return height of the tree
    size_t size( ) const; //return the number of items in the tree
    bool empty( ) const; //return whether the tree is empty or not
    void print(std::ostream &) const;
  
  private: //do not change
    BinaryNode * root; //designated root
    size_t nodes; //number of nodes 
    Comparator isLessThan; //function object to compare keys

  private:
    /* private utility functions that are implemented */
    void makeEmpty(BinaryNode * &);
    void insert_helper(BinaryNode*, BinaryNode* &);
    BinaryNode * remove_helper(const Key &, BinaryNode* &);
    BinaryNode * findMin(BinaryNode *) ;
    BinaryNode * findMax(BinaryNode *) ;   
    BinaryNode * find(const Key &, BinaryNode *) const;
    int height(BinaryNode *) const;
    void print(BinaryNode *, std::ostream &) const;
    void balance(BinaryNode* &);
    template <typename T> //static utility function
    static const T & max(const T &, const T &);

    //balancing functions
    void rotateWithLeftChild(BinaryNode * &);
    void rotateWithRightChild(BinaryNode * &);
    void doubleWithLeftChild(BinaryNode * &);
    void doubleWithRightChild(BinaryNode * &);
    
    //you may add your own private utility functions down here
    void finder(std::list<Iterator> &ret,BinaryNode*,const Key &, const Key &) const;
  private: //not copiable, DO NOT IMPLEMENT or change
    BinarySearchTree(const BinarySearchTree &);
    const BinarySearchTree & operator=(const BinarySearchTree &);
};

//node constructor, implemented do not change
template <typename K, typename O, typename C>
BinarySearchTree<K,O,C>::BinaryNode::
BinaryNode(const K & _k, const O & _d,
           BinaryNode * _l, BinaryNode * _r, size_t _h)
  : key(_k), data(_d), left(_l), right(_r), height(_h)
{
}

//default constructor, implemented do not change
template <typename K, typename O, typename C>
BinarySearchTree<K,O,C>::BinarySearchTree( )
  : root(NULL), nodes(0) 
{
}

//destructor, implemented do not change
template <typename K, typename O, typename C>
BinarySearchTree<K,O,C>::~BinarySearchTree( )
{
  makeEmpty(root);
}

//private utility function for destructor, do not change
template <typename K, typename O, typename C>
void
BinarySearchTree<K,O,C>::makeEmpty(BinaryNode * & t)
{
  if (t != NULL)
  {
    makeEmpty(t->left);
    makeEmpty(t->right);
    delete t;
    --nodes;
  }

  t = NULL;
}

template <typename K, typename O,typename C>
void 
BinarySearchTree<K,O,C>::insert_helper(BinaryNode* t, BinaryNode* & root){
  if(root == NULL)
  {
      root = t;
  }

  else if(isLessThan(t->key,root->key))
  {
      insert_helper(t, root->left);
      if(height(root->left) - height(root->right) == 2)
      {
          if(isLessThan(t->key,root->left->key))
              rotateWithLeftChild(root);
          else
              doubleWithLeftChild(root);
      }
  }
  else if(isLessThan(root->key,t->key))
  {
      insert_helper(t, root->right);
      if(height(root->right) - height(root->left) == 2)
      {
          if(isLessThan(root->right->key,t->key))
              rotateWithRightChild(root);
          else
              doubleWithRightChild(root);
      }
  }
  root->height = max(height(root->left), height(root->right))+1;
}

//public function to insert into BST, IMPLEMENT
template <typename K, typename O, typename C>
void
BinarySearchTree<K,O,C>::insert(const K & k, const O & x)
{
  BinaryNode* found = find(k, root);
  if(found){
    found -> data = x;
    return ;
  }
  BinaryNode* new_node = new BinaryNode(k, x, NULL, NULL);
  nodes++;
  insert_helper(new_node, root);
}

template <typename K, typename O, typename C>
typename BinarySearchTree<K,O,C>::BinaryNode *
BinarySearchTree<K,O,C>::findMin(BinaryNode * t)
{
  if(t == NULL){
    return NULL;
  }
  else if(t->left == NULL){
    return t;
  }
  else{
    return findMin(t->left);
  }
}

template <typename K, typename O, typename C>
typename BinarySearchTree<K,O,C>::BinaryNode *
BinarySearchTree<K,O,C>::findMax(BinaryNode * t)
{
  if(t == NULL){
    return NULL;
  }
  else if(t->right == NULL){
    return t;
  }
  else{
    return findMax(t->right);
  }
}

template <typename K, typename O, typename C>
void 
BinarySearchTree<K,O,C>::balance(BinaryNode* & t)
{
  t->height = max(height(t->left), height(t->right))+1;

  if(height(t->left) - height(t->right) == 2)
  {
    if(height(t->left->left) - height(t->left->right) == 1){
      rotateWithLeftChild(t);
    }
    
    else{
      doubleWithLeftChild(t);
    }
  }
 
  else if(height(t->right) - height(t->left) == 2)
  {
    if(height(t->right->right) - height(t->right->left) == 1){
      rotateWithRightChild(t);
    }
    else{
      doubleWithRightChild(t);
    }
  }
}

template <typename K, typename O,typename C>
typename BinarySearchTree<K,O,C>::BinaryNode *
BinarySearchTree<K,O,C>::remove_helper(const K & x, BinaryNode* & t){
  BinaryNode* temp;
  if(isLessThan(x,t->key)){
    t->left = remove_helper(x, t->left);
  }
  
  else if(isLessThan(t->key,x)){
    t->right = remove_helper(x, t->right);
  }

  else if(t->left && t->right)
  {
    // temp = findMin(t->right);
    // std::cout<<"12----"<<t->data<<"11----11"<<std::endl;
    // std::cout<<"13------"<<temp->data<<"11----11"<<std::endl;
    // std::cout<<"14------"<<t->key<<"11----11"<<std::endl;
    // std::cout<<"15------"<<temp->key<<"11----11"<<std::endl;
    // t->key = temp->key;
    // t->data = temp->data;
    // std::cout<<"16------"<<t->data<<"11----11"<<std::endl;
    // std::cout<<"17------"<<t->key<<"11----11"<<std::endl;
    // t->right = remove_helper(t->key, t->right);
    if(t->height==1){
      temp = t;
			t = t->right;
			t->left = temp->left;
			delete temp;
    }

    else{
      temp = findMin(t->right);;
      BinaryNode *gec = t->right;
			BinaryNode *prev = NULL;
			while (gec->left != NULL) {
				prev = gec;
				gec = gec->left;
			}
			gec = t;
			t = temp;
			BinaryNode *sag = t->right;
			t->left = gec->left;
			if (prev != NULL) {
				t->right = gec->right;
				prev->left = sag;
			}
			delete gec;
			if (t->right != NULL) balance(t->right);
    }
  }

  else
  {
    temp = t;
    if(t->left == NULL){
      t = t->right;
    }
    else if(t->right == NULL){
      t = t->left;
    }
    delete temp;
  }

  if(t == NULL){
    return t;
  }

  balance(t);
  
  return t;
}

//public function to remove key, IMPLEMENT
template <typename K, typename O, typename C>
void
BinarySearchTree<K,O,C>::remove(const K & k)
{
  BinaryNode* found;
  found = find(k, root);
  if (found){
    nodes--;
    //std::cout<<"11----11"<<found->data<<"11----11"<<std::endl;
    root = remove_helper(k,root);
  }
  else{
    return ;
  }
}

//public function to search elements, do not change
template <typename K, typename O, typename C>
typename BinarySearchTree<K,O,C>::Iterator
BinarySearchTree<K,O,C>::find(const K & key) const
{
  BinaryNode * node = find(key, root);
  if (node == NULL)
  {
    return end();
  }
  else
  {   
    return Iterator(node, *this, false);
  }
}

/*
 * private utility function to search elements
 * do not change
 */
template <typename K, typename O, typename C>
typename BinarySearchTree<K,O,C>::BinaryNode *
BinarySearchTree<K,O,C>::find(const K & key, BinaryNode * t) const
{
  if (t == NULL)
  {
    return NULL;
  }
  else if (isLessThan(key, t->key))
  {
    return find(key, t->left);
  }
  else if (isLessThan(t->key, key))
  {
    return find(key, t->right);
  }
  else //found
  {
    return t;
  }
}

//range queries those within range are inserted to the list
//IMPLEMENT
template <typename K, typename O, typename C>
void
BinarySearchTree<K,O,C>::finder(std::list<Iterator> &ret ,BinaryNode * a,const K & lower, const K & upper) const
{
  if (a == NULL) {
            return;
        }
  finder(ret,a->left, lower, upper);
  
  if ((isLessThan(lower,a->key )||lower==a->key) && (isLessThan(a->key,upper)||a->key==upper)) {
      Iterator it = Iterator(a,*this,true);
      ret.push_back(it);
  }
  finder(ret,a->right, lower, upper);

}

template <typename K, typename O, typename C>
std::list<typename BinarySearchTree<K,O,C>::Iterator>
BinarySearchTree<K,O,C>::find(const K & lower, const K & upper) const
{
  std::list<Iterator> result;
  BinaryNode* a=root;
  finder(result,a,lower,upper);
  return result;
}

//INORDER iterator begins at ++root, do not change
template <typename K, typename O, typename C>
typename BinarySearchTree<K,O,C>::Iterator
BinarySearchTree<K,O,C>::begin() const
{
  return Iterator(root, *this);
}

//no more increment after end() also
//returned in case of unsuccessful search
//or when no more applicance of ++ is possible
//do not change
template <typename K, typename O, typename C>
typename BinarySearchTree<K,O,C>::Iterator
BinarySearchTree<K,O,C>::end() const
{
  return Iterator(NULL, *this);
}

//public function to return height, do not change
template <typename K, typename O, typename C>
int
BinarySearchTree<K,O,C>::height( ) const
{
  return height(root);
}

/* private utility function for computing height */
//do not change
template <typename K, typename O, typename C>
int
BinarySearchTree<K,O,C>::height(BinaryNode * t) const
{
  return (t == NULL) ? -1 : t->height;
}

//public function to return number of nodes in the tree
//do not change
template <typename K, typename O, typename C>
size_t
BinarySearchTree<K,O,C>::size( ) const
{
  return nodes;
}

//public true if empty false o.w.
//do not change
template <typename K, typename O, typename C>
bool
BinarySearchTree<K,O,C>::empty( ) const
{
  return nodes == 0;
}

//public function to print keys inorder to some ostream
//do not change
template <typename K, typename O, typename C>
void
BinarySearchTree<K,O,C>::print(std::ostream & out) const
{
  print(root, out);
  out << '\n';
}

/* private utility function to print, do not change */
template <typename K, typename O, typename C>
void
BinarySearchTree<K,O,C>::print(BinaryNode * t, std::ostream & out) const
{
  if (t != NULL && t->left != NULL) 
  {
    out << '[';
    print( t->left, out );
  }
  else if (t != NULL && t->left == NULL && t->right != NULL)
  {
    out << "[";
  }

  if (t != NULL)
  {
    if (t->left == NULL && t->right == NULL)
    {
      out << '(' << (t->key) << ')';
    }
    else if (t->left != NULL || t->right != NULL)
    {
      out << '{' << (t->key) << ",H" << t->height << '}';
    }
  }
  
  if (t != NULL && t->right != NULL)
  {
    print( t->right, out );
    out << ']';
  }
  else if (t != NULL && t->left != NULL && t->right == NULL)
  {
    out << "]";
  }
}

/* static function to compute maximum of two elements */
//do not change
template <typename K, typename O, typename C>
template <typename T>
const T &
BinarySearchTree<K,O,C>::max(const T & el1, const T & el2)
{
  return el1 > el2 ? el1 : el2;
}

/* ROTATIONS, do not change */
template <typename K, typename O, typename C>
void 
BinarySearchTree<K,O,C>::rotateWithLeftChild(BinaryNode * & k2)
{
  BinaryNode *k1 = k2->left;
  k2->left = k1->right;
  k1->right = k2;
  k2->height = BinarySearchTree<K,O,C>::max(height(k2->left), 
                                            height(k2->right)) + 1;
  k1->height = BinarySearchTree<K,O,C>::max(height(k1->left), 
                                            height(k2)) + 1;
  k2 = k1;
}

template <typename K, typename O, typename C>
void 
BinarySearchTree<K,O,C>::rotateWithRightChild(BinaryNode * & k1)
{
  BinaryNode *k2 = k1->right;
  k1->right = k2->left;
  k2->left = k1;
  k1->height = BinarySearchTree<K,O,C>::max(height(k1->left), 
                                            height(k1->right)) + 1;
  k2->height = BinarySearchTree<K,O,C>::max(height(k2->right), 
                                            height(k1)) + 1;
  k1 = k2;
}

template <typename K, typename O, typename C>
void 
BinarySearchTree<K,O,C>::doubleWithLeftChild(BinaryNode * & k3)
{
  rotateWithRightChild(k3->left);
  rotateWithLeftChild(k3);
}

template <typename K, typename O, typename C>
void 
BinarySearchTree<K,O,C>::doubleWithRightChild(BinaryNode * & k1)
{
  rotateWithLeftChild(k1->right);
  rotateWithRightChild(k1);
}

/* ITERATOR functions */


// dummy constructor, do not change
template <typename K, typename O, typename C>
BinarySearchTree<K,O,C>::Iterator::Iterator( ) 
  : current(NULL), root(NULL), useStack(false)
{
}

// dereferencing operator non-const version, IMPLEMENT
template <typename K, typename O, typename C>
O &
BinarySearchTree<K,O,C>::Iterator::operator*( )
{
  return current->data;  
}

// dereferencing operator const version, IMPLEMENT
template <typename K, typename O, typename C>
const O & 
BinarySearchTree<K,O,C>::Iterator::operator*( ) const
{ 
  return current->data;    
}

// compare Iterators ignoring useStack var, do not change
template <typename K, typename O, typename C>
bool 
BinarySearchTree<K,O,C>::Iterator::
operator==(const Iterator & rhs) const
{ 
  return current == rhs.current && 
         root == rhs.root; 
}

// compare Iterators ignoring useStack var, do not change
template <typename K, typename O, typename C>
bool 
BinarySearchTree<K,O,C>::Iterator::
operator!=(const Iterator & rhs) const
{ 
  return !(*this == rhs); 
}

// increment Iterator to point to the inorder next
// node of then-current node, in case that no further
// advances are possible return an Iterator that is
// equal to end( ) , IMPLEMENT
template <typename K, typename O, typename C>
typename BinarySearchTree<K,O,C>::Iterator &
BinarySearchTree<K,O,C>::Iterator::operator++( )
{
  if (!useStack || current == NULL)
	{
		current = NULL;
		return *this;
	}
	s.push(current);
	C isLessThan;
	K ank = current->key;
	BinaryNode *myRoot = new BinaryNode(root->key, root->data, root->left, root->right, root->height);
	if (current->right != NULL)
	{
    BinaryNode* rooter = NULL;
		current = current->right;
		while (current != NULL)
		{
			s.push(current);
      rooter=current;
			current = current->left;	
		}
		current = rooter;
		s.pop();
		if (current == NULL || isLessThan(current->key, ank)){ 
      current = NULL;
    }
		return *this;
	}
	s.push(myRoot);
	BinaryNode *prev = NULL;
	while (isLessThan(s.top()->key, current->key) || isLessThan(current->key, s.top()->key))
	{
		if (isLessThan(current->key, s.top()->key))
		{
			prev = s.top();
			s.push(s.top()->left);
		}
		else {
      s.push(s.top()->right);
    }
	}
	while (!s.empty() && s.top() != prev){ 
    s.pop();
  }
	current = prev;
	if (current == NULL || isLessThan(current->key, ank)) {
    current = NULL;
  }
	return *this;
}

/* real Iterator constructor will be invoked by 
 * BST member function only. if no inorder iterator
 * is required by the computation designer should 
 * explicitly set useStack variable to false, o.w.
 * it will be assumed to be true. IMPLEMENT
 */

template <typename K, typename O, typename C>
BinarySearchTree<K,O,C>::Iterator::
Iterator(BinaryNode * p, const BinarySearchTree & rhs, bool stk)
{
  // this->current=p;
  // this->root=rhs.root;
  // useStack=stk;
  this->root = rhs.root;
	useStack = stk;
	if (stk){
		BinaryNode *prev = p;
		BinaryNode *now = NULL;
		if (p != NULL){
      now = p->left;
      while (now != NULL)
      {
        prev = now;
        now = now->left;
      }
    }
		this->current = prev;
	} 
  else {
    this->current = p;
  }
}


#endif
