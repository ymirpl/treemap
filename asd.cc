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
	 * Method swapping two elements
	 */
	static void swap(TreeNode *one, TreeNode *two) {
		//najpierw podmienimy dzieci
		TreeNode* tmp;
		
		std::cerr << "Swappin " << one->data.first << " with: " << two->data.first << std::endl;

		tmp = one->left; // swapping left son for two
		one->left = two->left;
		if (two->left!=NULL)
			two->left->parent = one; 
		
		two->left = tmp; // for one
		if (tmp!=NULL)
			tmp->parent = two;

		tmp = one->right; // right sons
		one->right = two->right;
		if (two->right!=NULL)
			two->right->parent = one;
		two->right = tmp;
		if (tmp!=NULL)
			tmp->parent = two;

		//swapping parents
		if (one->parent->left == one)
			one->parent->left = two;
		else
			one->parent->right = two;
		
		if (two->parent->left == two)
			two->parent->left = one;
		else
			two->parent->right = one;
		
		tmp = one->parent;
		one->parent = two->parent;
		two->parent = tmp;
	}
	
	/**
	 * Method recursively deleting all elements
	 */
	static void deleteAll(TreeNode* node) {
		if (node==NULL)
			return;
		if (node->left != NULL)
			deleteAll(node->left);
		if (node->right != NULL)
			deleteAll(node->right);
		
		std::cerr << "killing : " << node->data.first << std::endl;
		delete node;
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
			std::cerr << "Insert Exists " << crawler << " with parent " << crawler->parent << " w lc : " << crawler->left << " w rc: " << crawler->right << std::endl;
			return std::make_pair(iterator(crawler), false);
		}
		if (entry.first < crawler->data.first)
			crawler = crawler->left;
		else {
			crawler = crawler->right;
			tokenNewBegin = false;
		}
	}
	
	Node* newNode = new TreeNode(entry, parent, NULL, NULL); 
	
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
	Node* crawler = root->left; // first significant node
	Node* parent = root;
	
	// Token sais whether we took right son. If so, new element isn't new begin().
	// Otherwise it is. 
	bool tokenNewBegin = true; // no right sons taken
							
	
	while (crawler != NULL) {
		parent = crawler;
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
		return iterator(newNode);
		
	}
	
	if (entry.first < parent->data.first)
		parent->left = newNode;
	else
		parent->right = newNode; // link to the proper side of parent
	
	std::cerr << "inserted " << newNode << " with Parent " << newNode->parent << std::endl;
	
	return iterator(newNode);
}

// Returns an iterator addressing the location of the entry in the map
// that has a key equivalent to the specified one or the location succeeding the
// last element in the map if there is no match for the key.
TreeMap::iterator TreeMap::find(const Key& k) {
	Node* crawler = root->left; // first significant node
	Node* parent = root;
	
	std::cerr << "find given jkey : " << k << std::endl;

	while (crawler != NULL) {
		parent = crawler;
		if (crawler->data.first == k) {// element exists
			std::cerr << " find Exists " << crawler->data.first << " with parent "
					<< crawler->parent << " w lc : " << crawler->left
					<< " w rc: " << crawler->right << std::endl;
			return iterator(crawler);
		}
		if (k < crawler->data.first)
			crawler = crawler->left;
		else
			crawler = crawler->right;
	}
	// element not found
	return end();
}

TreeMap::const_iterator TreeMap::find(const Key& k) const {
	Node* crawler = root->left; // first significant node
	Node* parent = root;

	while (crawler != NULL) {
		parent = crawler;
		if (crawler->data.first == k) {// element exists
			std::cerr << "cons_find Exists " << crawler << " with parent "
					<< crawler->parent << " w lc : " << crawler->left
					<< " w rc: " << crawler->right << std::endl;
			return const_iterator(crawler);
		}
		if (k < crawler->data.first)
			crawler = crawler->left;
		else
			crawler = crawler->right;
	}
	// element not found
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
	Node* node;
	node = i.node; // node is element to delete
	Node* tmp;
	TreeMap::iterator ret;
	int k = i->first;
	

	   if (i.node == root) // can't delete sentinel
		return end();
	
	 /*
	   //OKAZUJE SIE ZE NIE MA ZNACZENIA CZY SZUKAM CZY NIE
	///////////////////////////   
	Node* crawler = root->left; // first significant node
	Node* parent = root;

	while (crawler != NULL) {
		parent = crawler;
		if (crawler->data.first == k) {// element exists
			node = crawler;
		}
		if (k < crawler->data.first)
			crawler = crawler->left;
		else
			crawler = crawler->right;
		
	}
	
	if(node != i.node)
		std::cerr << "WIELKI CHUJ" << std::endl;
	if(node == root) {
		std::cerr << "ALE TO END" << std::endl;
		node = i.node;
	}
	///////////////////////////////////////////////
	*/
	   
	if(i == begin()) { //deletion of begin
		ret = i;
		ret++;
		if(node->parent != root)
			node->parent->left = NULL; // node is left leaf
		else
			root->left = ret.node;
		
		root->right = ret.node; // new begin;
		std::cerr << "Deletrig " << node->data.first << " returning: " << ret.node->data.first << "  " << ret.node->data.second << std::endl;
		
		delete node;
		
		(root->data).first--; // decrease size
		return ret;
	}
	
	if (node->right == NULL && node->left == NULL) { // node is leaf
		ret = i;
		ret++;
		
		std::cerr<< "Lisc " <<std::endl;
		std::cerr << "Erasing: " << node->data.first << " ret: " << ret.node->data.first <<  "  " << ret.node->data.second << std::endl;
				
		if (node->parent->left == node)
			node->parent->left = NULL;
		else
			node->parent->right = NULL;

		delete node;
		(root->data).first--; // decrease size
		
		return ret;
	}
	
	if (node->right!=NULL && node->left != NULL) { // node has two subtrees
		std::cerr<< "Dwa poddrze swap" <<std::endl;
		
		TreeMap::iterator ret = i;
		ret++;
		
		TreeMapDetail::swap(i.node, ret.node );
		std::cerr << "Erasing: " << node->data.first << " ret: " << ret.node->data.first <<  "  " << ret.node->data.second << std::endl;
		
		erase(i); // proper erase
		return ret;
		
	}
		
	
	
	if (node->right != NULL) { // one right subtree
		std::cerr<< "jeden prawy" <<std::endl;
		ret = i; // we return node after delted
		ret++;
		
		// we're linking "over" deleted node
		if (node->parent->right == node)
			node->parent->right = node->right;
		else
			node->parent->left = node->right;

		node->right->parent = node->parent;
		
		
		std::cerr << "Erasing: " << i.node->data.first << " ret: " << ret.node->data.first << "  " << ret.node->data.second << std::endl;

		delete node;
		(root->data).first--; // decrease size

		return ret;
	} else { // node has one left subtree
		std::cerr<< "jeden lewy" <<std::endl;
		 ret = i;
		 ret++;
		
		 // linking "over" delted node 
		if (node->parent->right == node)
			node->parent->right = node->left;
		else
			node->parent->left = node->left;

		node->left->parent = node->parent;
		std::cerr << "Erasing: " << i.node->data.first << " ret: " << ret.node->data.first << "  " << ret.node->data.second << std::endl;

		
		delete node;
		(root->data).first--; // decrease size
	
		return ret;

	}

}

// Removes a range of elements from the map.
// The range is defined by the key values of the first and last iterators
// first is the first element removed and last is the element just beyond the last elemnt removed.
// @returns The iterator that designates the first element remaining beyond any elements removed.
TreeMap::iterator TreeMap::erase(TreeMap::iterator f, TreeMap::iterator l) {
	while (f != l) {
		std::cerr << "Deleting: " << f->first << " " << f->second << std::endl; 
		std::cerr << "Last: " << l->first << " " << l->second << std::endl; 
		f = erase(f);
	}

}

// Removes an element from the map.
// @returns The number of elements that have been removed from the map.
//          Since this is not a multimap itshould be 1 or 0.
TreeMap::size_type TreeMap::erase(const Key& key)
{
	std::cerr << "Given key: " << key << std::endl;
	TreeMap::iterator iter;
	iter = find(key);
	std::cerr << "Iter sec: " << iter->second << std::endl;
	if(iter != end())  {// key exists
		erase(iter);
		return 1;
	}
	return 0;
}

// Erases all the elements of a map.
void TreeMap::clear() {
	(root->data).first = 0;
	root->right = root; // begin = end
	root->left = NULL;
	TreeMapDetail::deleteAll(root->left);
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
 * begin-- returns end()
 */
TreeMap::const_iterator& TreeMap::const_iterator::operator--() {

	if (node->left != NULL) {// node has left son. Seek for the rightmost son of left subtree
		node = node->left;
		while (node->right != NULL)
		node = node->right;
		return *this;
	}

	if (node == node->parent->right) { // node is right son. Return parent
		node = node->parent;
		return *this;
	}

	while (node->parent->parent != NULL && node == node->parent->left) // node is left son
		node = node->parent;
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

void TreeMap::draw() {
	TreeMap::iterator i = begin();
	for(;i!=end();i++) {
		std::cout <<  i->first <<" p: " << i.node->parent->data.first << std::endl;

		if(i.node->left != NULL)
			std::cout << " w l: " << i.node->left->data.first << std::endl;
		if(i.node->right != NULL)
			std::cout<< " r: "<<i.node->right->data.first << std::endl;
		
		std::cout<< " --- " <<std::endl;
	}
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
   std::cerr<<p.first<<", "<<p.second<<std::endl;
}

#include <map>

/// The big mean test function ;)
void test()
{
   // A typedef used by the test.
   //typedef std::map<int, std::string> TEST_MAP;
   //typedef SmallMap<int, std::string> TEST_MAP;
   typedef TreeMap TEST_MAP;


   std::cerr << "Testy uzytkownika" << std::endl;

        TEST_MAP m;
        TreeMap::iterator iterator;
   
   m[2] = "Merry";
   m[4] = "Jane";
   m[8] = "Korwin";
   m[6] = "Weiss";
   m[7] = "Kain";
   m[0] = "Abel";
   m[9] = "Moses";
   m[20];m[100];m[19];m[23];m[21];m[40];m[22];m[33];m[18];m[60];m[12];m[80];
   m.draw();
   
   TreeMap::iterator eraseIterator = m.begin();
   
   TreeMap::iterator i;
   
   
   eraseIterator++;
   std::cout << "Eiter: " << eraseIterator->first  <<std::endl;
   TreeMap::iterator ender = eraseIterator;
   ender++;ender++;ender++;ender++;ender++;ender++;
   std::cout << "Eiter: " << eraseIterator->first  <<std::endl;
   std::cout << "Ender: " << ender->first  <<std::endl;
         
   m.erase(eraseIterator, ender);
   m.draw();

   for(i = m.end(); i != m.begin(); --i)
	   std::cout << i->first << " " << i->second << std::endl;
   std::cout << i->first << " " << i->second << std::endl;
   std::cout << "Size after erase : " << m.size() << std::endl;
   

   
   m.clear();
   std::cout << "CLEAR! " << std::endl;
   m.draw();
   std::cout << "CLEAR" << std::endl;
   for(i = m.end(); i != m.begin(); --i)
	   std::cout << i->first << " " << i->second << std::endl;
   std::cout << i->first << " " << i->second << std::endl;
   std::cout << "Size after erase : " << m.size() << std::endl;

   
   m[2] = "Merry";
   m[4] = "Jane";
   m[8] = "Korwin";
   m[6] = "Weiss";
   m[7] = "Kain";
   m[0] = "Abel";
   m[9] = "Moses";
   m[20];m[100];m[19];m[23];m[21];m[40];m[22];m[33];m[18];m[60];m[12];m[80];
   m.draw();
   std::cout << "Size after insert : " << m.size() << std::endl;
   


//	   m.erase(eraseIterator, shower);
/*   for( int i = 3; i < 17 ; i++) {

	   //eraseIterator = m.begin();
	   //for(int j =0; j < 17-i ; j++ {
	   eraseIterator++;
	   //}
	  std::cerr << "Eiter: " << eraseIterator->first  << " " << eraseIterator->second << std::endl;
	 m.erase(eraseIterator);
	   m.draw();
	  std::cerr << "Size after erase : " << m.size() << std::endl;
      

		   
	 }
    */

   //for_each(m.begin(), m.end(), print );
   //system("PAUSE");
}

//////////////////////////////////////////////////////////////////////////////
// main - jest w pliku /home/common/dyd/aisdi/z2/main.cc
//////////////////////////////////////////////////////////////////////////////

//int main()
//{
//   std::cerr << "AISDI cwiczenie 4: wchodze do funkcji main." << std::endl;
//   test();
//   // Biblioteka z bardziej rygorystyczna wersja tych testow bedzie udostepniona na nastepnych zajeciach.
//   Test2();
//   //system("PAUSE");
//   return EXIT_SUCCESS;
//}
