/** 
@file asd.cc

Plik do modyfikacji w w ramach cwiczenia 4 z AISDI.
Zawiera niekompletne implementacje metod klasy TreeMap.
Jest tez prosta funkcja testujaca (void test()), ktora
jest wolana w funkcji main. Mozna w niej zaimplementowac
wlasne testy.
NALEZY ZMODYFIKOWAC I UZUPELNIC CIALA METOD KLASY TreeMap.
    
@author
Pawel Cichocki, Michal Nowacki

@date  
last revision
- 2005.12.01 Pawel Cichocki: TreeNodeDetail class
- 2005.12.01 Michal Nowacki: lab #4
- 2005.11.17 Michal Nowacki: lab #3, copy operator and constructor
- 2005.11.04 Pawel Cichocki: copied comments from the header
- 2005.11.03 Pawel Cichocki: const_iterator done properly now
- 2005.10.27 Pawel Cichocki: cosmetic changes
- 2005.10.26 Michal Nowacki: removed some method bodies
- 2005.10.25 Pawel Cichocki: wrote it

COPYRIGHT:
Copyright (c) 2005 Instytut Informatyki, Politechnika Warszawska
ALL RIGHTS RESERVED
*******************************************************************************/

#include <assert.h>
#include <algorithm>

#include <iostream>

#ifdef _SUNOS
#include "/home/common/dyd/aisdi/tree/TreeMap.h" 
#else
#include "TreeMap.h"
#endif

/// A helper class.
class TreeMapDetail //Helper
{

public:
	friend class TreeMap;
	typedef std::pair<int,std::string> T;

protected:

	/**
	 * Method finding next element (the most left of right subtree)
	 */
	static TreeNode* findNext(TreeNode* node) {
		
		if (node->right != NULL) { // right subtree exists
			node = node->right;
			while (node->left != NULL)
				node = node->left; // go to the leftmost node
			return node;
		}
		
		// No subtree
		if (node->parent->left == node) { // node is left son
			return node->parent;
		}
		// node is right son
		
		//node = node->parent;
		while (node == node->parent->right) { // go up all right sons
			node = node->parent;
		}
		return node->parent;
	}

};

//////////////////////////////////////////////////////////////////////////////
// TreeMap and TreeMap::iterator methods
//////////////////////////////////////////////////////////////////////////////

/**
 * We have one sentinel in root, which is end, all proper tree is his left son
 * His key value is map size. 
 * Right sone of the root is pointer to the begin().
 */
TreeMap::TreeMap() {
	root = new TreeNode(std::make_pair(0,"__SENTINEL"), NULL);
}

/// Content of existing TreeMap object is copied into the new object. 
TreeMap::TreeMap( const TreeMap& m )
{
   ///@todo Implement this
};

TreeMap::~TreeMap() {
	clear();
	delete root;
}

// Inserts an element into the map.
// @returns A pair whose bool component is true if an insertion was
//          made and false if the map already contained an element
//          associated with that key, and whose iterator component coresponds to
//          the address where a new element was inserted or where the element
//          was already located.
std::pair<TreeMap::iterator, bool> TreeMap::insert(const std::pair<Key, Val>& entry) {
	Node* crawler = root->left; // first significant node
	Node* parent = root;
	
	// Token sais whether we took right son. If so, new element isn't new begin().
	// Otherwise it is. 
	bool tokenNewBegin = true; // no right sons taken
							
	
	while (crawler != NULL) {
		parent = crawler;
		if (crawler->data.first == entry.first) {// element already exists
			std::cerr << "Exists " << crawler << " with parent " << crawler->parent << " w lc : " << crawler->left << " w rc: " << crawler->right << std::endl;
			return std::make_pair(iterator(crawler), false);
		}
		if (entry.first < crawler->data.first)
			crawler = crawler->left;
		else {
			crawler = crawler->right;
			tokenNewBegin = false;
		}
	}
	
	Node* newNode = new TreeNode(entry, parent); 
	
	root->data.first++; // increase size counter
	
	if(tokenNewBegin) { // newNode is new begin()
		root->right = newNode; 
	}
	
	if (parent == root) {
		root->left = newNode; // we only link to the left sie of root (sentinel)
		std::cerr << "inserted " << newNode << " with Parent root" << std::endl;
		return std::make_pair(iterator(newNode), true);
		
	}
	
	if (entry.first < parent->data.first)
		parent->left = newNode;
	else
		parent->right = newNode; // link to the proper side of parent
	
	std::cerr << "inserted " << newNode << " with Parent " << newNode->parent << std::endl;
	
	return std::make_pair(iterator(newNode), true);
}

// Inserts an element into the map.
// This method assumes there is no value asociated with
// such a key in the map.
TreeMap::iterator TreeMap::unsafe_insert(const std::pair<Key, Val>& entry)
{
   ///@todo Finnish this
   assert(0);
   return iterator(root);
}

// Returns an iterator addressing the location of the entry in the map
// that has a key equivalent to the specified one or the location succeeding the
// last element in the map if there is no match for the key.
TreeMap::iterator TreeMap::find(const Key& k)
{
   ///@todo Implement this
   assert(0);
   return end();
}

TreeMap::const_iterator TreeMap::find(const Key& k) const
{
   ///@todo Implement this
   assert(0);
   return end();
}

// Inserts an element into a map with a specified key value
// if one with such a key value does not exist.
// @returns Reference to the value component of the element defined by the key.
TreeMap::Val& TreeMap::operator[](const Key& k) {
	iterator i = insert(std::make_pair(k, "")).first;
	return i->second;
}

// Tests if a map is empty.
bool TreeMap::empty() const {
	return root->left == NULL;
}

// Returns the number of elements in the map.
TreeMap::size_type TreeMap::size() const {
	return root->data.first;
}

// Returns the number of elements in a map whose key matches a parameter-specified key.
TreeMap::size_type TreeMap::count(const Key& _Key) const {
	if (find(_Key) != end())
		return 1;
	else
		return 0;// this is not a multimap
}

// Removes an element from the map.
// @returns The iterator that designates the first element remaining beyond any elements removed.
TreeMap::iterator TreeMap::erase(TreeMap::iterator i)
{
   ///@todo Implement this
   assert(0);
   return end();
}

// Removes a range of elements from the map.
// The range is defined by the key values of the first and last iterators
// first is the first element removed and last is the element just beyond the last elemnt removed.
// @returns The iterator that designates the first element remaining beyond any elements removed.
TreeMap::iterator TreeMap::erase(TreeMap::iterator f, TreeMap::iterator l)
{
   ///@todo Implement this
   assert(0);
   return end();
}

// Removes an element from the map.
// @returns The number of elements that have been removed from the map.
//          Since this is not a multimap itshould be 1 or 0.
TreeMap::size_type TreeMap::erase(const Key& key)
{
   ///@todo Implement this
   assert(0);
   return 0;
}

// Erases all the elements of a map.
void TreeMap::clear( )
{
   //TreeMapDetail::erase(this, 0);  /// Stupid helper usage example
   ///@todo Implement this
   //assert(0);
}

bool TreeMap::struct_eq(const TreeMap& another) const
{
   ///@todo Implement this
   assert(0);
   return false;
}

bool TreeMap::info_eq(const TreeMap& another) const
{
   ///@todo Implement this
   assert(0);
   return false;
}

// preincrement
// Going ot be in-order
TreeMap::const_iterator& TreeMap::const_iterator::operator ++() {

	if(node->parent == NULL) { // trying to ++end()
		return *this;
		std::cerr << " jestem root! " << std::endl;
	}

	if (node->right != NULL) { // node has right son
		node = node->right;

		while(node->left != NULL)
		node = node->left;

		return *this;
	}

	if (node == node->parent->left) { // node is left son
		node = node->parent;
		return *this;
	}

	while (node->parent->parent != NULL && node == node->parent->right) { //Node is rright son. p->p == NULL tells root apart.  
		std::cerr << "jestem prawy "<< std::endl;
		node = node->parent;
	}
	node = node->parent;

	return *this;
}

// postincrement
TreeMap::const_iterator TreeMap::const_iterator::operator++(int) {
	const_iterator temp = *this;
	++(*this);
	return temp;
}

// predecrement
/*
 * Going to by in-order type
 */
TreeMap::const_iterator& TreeMap::const_iterator::operator--() {
	
//	if(*this == begin()) // begin-- = begin
//		return *this;
	
	if (node->left != NULL) {// node has left son
		node = node->left;
		while (node->right != NULL)
			node = node->right;
		return *this;
	}
	
	if (node->parent != NULL && node == node->parent->right) { // node is right son
		node = node->parent;
		return *this;
	}
	
	while (node->parent != NULL && node == node->parent->left) // node is left son
		node = node->parent;
	
	return *this;
}

// postdecrement
TreeMap::const_iterator TreeMap::const_iterator::operator--(int) {
	const_iterator temp = *this;
	--(*this);
	return temp;
}


/// Assignment operator copy the source elements into this object.
TreeMap& TreeMap::operator=(const TreeMap& )
{
   ///@todo Implement this
   return *this;
}
      

/// Returns an iterator addressing the first element in the map
TreeMap::iterator TreeMap::begin() {
	if (root->left == NULL) // pusta mapa
		return iterator(root);
	return iterator(root->right);
}

TreeMap::const_iterator TreeMap::begin() const {
	if (root->left == NULL) // pusta mapa
		return const_iterator(root);
	return const_iterator(root->right);
}

/// Returns an iterator that addresses the location succeeding the last element in a map
TreeMap::iterator TreeMap::end() {
	return iterator(root);
}

/// Returns an iterator that addresses the location succeeding the last element in a map
TreeMap::const_iterator TreeMap::end() const {
	return const_iterator(root);
}

//////////////////////////////////////////////////////////////////////////////
// Tests
//////////////////////////////////////////////////////////////////////////////

/// A helper function that outputs a key-value pair.
void print(const std::pair<int, std::string>& p)
{
   std::cout<<p.first<<", "<<p.second<<std::endl;
}

#include <map>

/// The big mean test function ;)
void test()
{
   // A typedef used by the test.
   //typedef std::map<int, std::string> TEST_MAP;
   //typedef SmallMap<int, std::string> TEST_MAP;
   typedef TreeMap TEST_MAP;


   std::cout << "Testy uzytkownika" << std::endl;

        TEST_MAP m;
        TreeMap::iterator iterator;
   
   m[2] = "Merry";
   m[4] = "Jane";
   m[8] = "Korwin";
   m[4] = "Magdalena";
   
   std::cout<<m[2]<<" "<<m[4]<<std::endl;
   //for(iterator=m.begin(); iterator != m.end(); )
   iterator = m.begin();
   
	   std::cerr<<iterator->second<<std::endl;
	   iterator++; std::cerr<<iterator->second<<std::endl;
	   iterator++; std::cerr<<iterator->second<<std::endl;
	   iterator++; std::cerr<<iterator->second<<std::endl;
	   	    
	   //std::cout<<iterator->second;

   //for_each(m.begin(), m.end(), print );
   //system("PAUSE");
}

//////////////////////////////////////////////////////////////////////////////
// main - jest w pliku /home/common/dyd/aisdi/z2/main.cc
//////////////////////////////////////////////////////////////////////////////

//int main()
//{
//   std::cout << "AISDI cwiczenie 4: wchodze do funkcji main." << std::endl;
//   test();
//   // Biblioteka z bardziej rygorystyczna wersja tych testow bedzie udostepniona na nastepnych zajeciach.
//   Test2();
//   //system("PAUSE");
//   return EXIT_SUCCESS;
//}
