
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
protected:
	friend class TreeMap;
	friend class TreeMap::const_iterator;
	unsigned int size; // TreeSize

	/// Stupid example of a method that modifies a protected field in 
	/// the TreeMap class. Feel free to remove this method or add new
	/// ones here.
	//static void erase(TreeMap* tm, const TreeMap::Key& k)
	//{
	//	tm->root=NULL; // we just modified a protected field in tree map
	//}

	// returns next element with key k (iterative)
	static TreeNode* treeSearch(TreeNode* node, const TreeMap::Key& k)
	{
		while (node != NULL && node->data.first != k)
		{
			if (k <= node->data.first)
				node = node->left;
			else
				node = node->right;
		}
		return node;
	}
	// returns next element in inorder traverse
	static TreeNode* pred(TreeNode* node) {
		assert(node != NULL);
		/*	TreeNode * tmp = node;
		 if (tmp -> left != NULL)
		 return TreeMapDetail::treeMaximum(tmp->left);
		 TreeNode * y = tmp->parent;
		 while (y != NULL && tmp == y->left)
		 {
		 tmp = y;
		 y = y->parent;
		 }
		 return y; */
		if (node->left != NULL) {// node has left son. Seek for the rightmost son of left subtree
			node = node->left;
			while (node->right != NULL)
				node = node->right;
			return node;
		}

		if (node == node->parent->right) { // node is right son. Return parent
			node = node->parent;
			return node;
		}

		while (node->parent->parent != NULL && node == node->parent->left)
			// node is left son
			node = node->parent;
		node = node->parent;

		return node;
	}
	// returns next element in inorder traverse
	static TreeNode* succ(TreeNode* node)
	{
		assert(node != NULL);
		/*TreeNode * tmp = node;
		if (tmp->right != NULL)
			return TreeMapDetail::treeMinimum(tmp->right);
		TreeNode * y = tmp->parent;
		while (y != NULL && tmp == y->right)
		{
			tmp = y;
			y = y->parent;
		}
		return y; */

		if (node->parent == NULL) { // trying to ++end()
			return node;
		}

		if (node->right != NULL) { // node has right son
			node = node->right;

			while (node->left != NULL)
				node = node->left;

			return node;
		}

		if (node == node->parent->left) { // node is left son
			node = node->parent;
			return node;
		}

		while (node->parent->parent != NULL && node == node->parent->right) { //Node is rright son. p->p == NULL tells root apart.  
			node = node->parent;
		}
		node = node->parent;

		return node;
	}
	// returns the rightmost tree element
	static TreeNode* treeMinimum(TreeNode* node) {
		if (node == NULL)
			return NULL;
		while (node->left != NULL)
			node = node->left;
		return node;
	}
	// returns the leftmost tree element
	static TreeNode* treeMaximum(TreeNode* node) {
		if (node == NULL)
			return NULL;
		while (node->right != NULL)
			node = node->right;
		return node;
	}
	// recursive delete tree
	static void deleteAll(TreeNode* node) {
		if (node == NULL)
			return;
		if (node->left != NULL)
			deleteAll(node->left);
		if (node->right != NULL)
			deleteAll(node->right);
		delete node;
	}
	static TreeNode* copyAll(TreeNode* node, TreeNode* parent) {
		if (node == NULL)
			return NULL;
		TreeNode* ret = new TreeNode(node->data, parent);
		ret->left = copyAll(node->left, ret);
		ret->right = copyAll(node->right, ret);
		return ret;
	}
};


//////////////////////////////////////////////////////////////////////////////
// TreeMap and TreeMap::iterator methods
//////////////////////////////////////////////////////////////////////////////

TreeMap::TreeMap()
{
	root = new TreeNode(std::make_pair(INT_MAX, "__SENTINEL")); // sentinel
	detail = new TreeMapDetail();
	detail->size = 0; // size 
};

/// Content of existing TreeMap object is copied into the new object. 
TreeMap::TreeMap(const TreeMap& m) {

	root = new TreeNode(std::make_pair(INT_MAX, "__SENTINEL")); // sentinel
	detail = new TreeMapDetail();
	detail->size = m.detail->size; 
	root->left = TreeMapDetail::copyAll(m.root->left, root);
}
;

TreeMap::~TreeMap() {
	clear();
	delete root;
	delete detail;
}
;

// Inserts an element into the map.

// @returns A pair whose bool component is true if an insertion was
//			 made and false if the map already contained an element
//			 associated with that key, and whose iterator component coresponds to
//			 the address where a new element was inserted or where the element
//			 was already located.
std::pair<TreeMap::iterator, bool> TreeMap::insert(const std::pair<Key, Val>& entry)
{
	///@todo Finnish this (Poor Finnish people)
/*	TreeNode* node = root->left;
	if (node == NULL)
	{
		root->left = new TreeNode(entry, root);
		++detail->size;
		return std::make_pair(iterator(root->left), true);
	}
	while (/* node != NULL && *///node->data.first != entry.first )
/*	{
		if (node->data.first > entry.first)
		{
			if (node->left == NULL)
			{
				node->left = new TreeNode(entry, node);
				++detail->size;
				return std::make_pair(iterator(node->left), true);
			}
			else
				node = node->left;
		}
		else
		{
			if (node->right == NULL)
			{
				node->right = new TreeNode(entry, node);
				++detail->size;
				return std::make_pair(iterator(node->right), true);
			}
			else
				node = node->right;
		}
	}
	//if (node->data.first == entry.first)
		return std::make_pair(iterator(node), false);

	// shouldn't happen
	//return std::make_pair(unsafe_insert(entry), true);
*/
	Node* crawler = root->left; // first significant node
	Node* parent = root;
	
	while (crawler != NULL) {
		parent = crawler;
		if (crawler->data.first == entry.first) {// element already exists
			return std::make_pair(iterator(crawler), false);
		}
		if (entry.first <= crawler->data.first)
			crawler = crawler->left;
		else 
			crawler = crawler->right;
	}
	
	Node* newNode = new TreeNode(entry, parent, NULL, NULL); 
	
	(detail->size)++; // increase size counter
	
	if (parent == root) {
		root->left = newNode; // we only link to the left sie of root (sentinel)
		std::cerr << "inserted " << newNode << " with Parent root" << std::endl;
		return std::make_pair(iterator(newNode), true);
		
	}
	
	if (entry.first <= parent->data.first)
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
	TreeNode * node = root->left;
	while ( node != NULL )
	{
		if (node->data.first > entry.first)
		{
			if (node->left == NULL)
			{
				node->left = new TreeNode(entry, node);
				++detail->size;
				return iterator(node->left);
			}
			else
				node = node->left;
		}
		else
		{
			if (node->right == NULL)
			{
				node->right = new TreeNode(entry, node);
				++detail->size;
				return iterator(node->right);
			}
			else
				node = node->right;
		}
	}
	++detail->size;
	root->left = new TreeNode(entry, root);
	return iterator(root->left);
}

// Returns an iterator addressing the location of the entry in the map
// that has a key equivalent to the specified one or the location succeeding the
// last element in the map if there is no match for the key.
TreeMap::iterator TreeMap::find(const Key& k)
{
	///@todo Implement this
	TreeNode * node = TreeMapDetail::treeSearch(root->left, k);
	if (node == NULL)
		return end();
	else
		return iterator(node);
}

TreeMap::const_iterator TreeMap::find(const Key& k) const
{
	///@todo Implement this
	TreeNode * node = TreeMapDetail::treeSearch(root->left, k);
	if (node == NULL)
		return end();
	else
		return const_iterator(node);
}

// Inserts an element into a map with a specified key value
// if one with such a key value does not exist.
// @returns Reference to the value component of the element defined by the key.
TreeMap::Val& TreeMap::operator[](const Key& k)
{
	///@todo Implement this
	iterator i = insert(std::make_pair(k, "")).first;
	return i->second;
}

// Tests if a map is empty.
bool TreeMap::empty( ) const
{
	return root->left==NULL;
}

// Returns the number of elements in the map.
TreeMap::size_type TreeMap::size( ) const
{
	///@todo Implement this
	return detail->size;
}

// Returns the number of elements in a map whose key matches a parameter-specified key.
TreeMap::size_type TreeMap::count(const Key& _Key) const
{
	///@todo Implement this
	return find(_Key) == end()? 0 : 1;  // this is not a multimap
}

// Removes an element from the map.
// @returns The iterator that designates the first element remaining beyond any elements removed.
TreeMap::iterator TreeMap::erase(TreeMap::iterator i)
{
	///@todo Implement this
	//assert(0);
	TreeNode * node = i.node;
	assert(node != root);

	TreeNode * y;
	TreeNode * x;
	TreeNode * ret = TreeMapDetail::succ(node);
	if (node->left == NULL || node->right == NULL)
		y = node;
	else
		y = ret;

	if (y->left != NULL)
		x = y->left;
	else
		x = y->right;

	if (x != NULL)
	{
		x->parent = y->parent;
	}
	if (y == root->left)
	{
		root->left = x;
		if (x != NULL)
			x->parent = root;
	}
	else
	{
		if (y == y->parent->left)
		{
			y->parent->left = x;
		}
		else
			y->parent->right = x;
	}
	if (y != node)
	{
		// przepinamy y w miejsce node
		if (node->parent->left == node)
			node->parent->left = y;
		else
			node->parent->right = y;
		y->parent = node->parent;
		y->left = node->left;
		if (y->left != NULL)
			y->left->parent = y;
		y->right = node->right;
		if (y->right != NULL)
			y->right->parent = y;
		x = y;
	}
	else
	{
		x = ret;
	}
	delete node;
	--detail->size;
	return iterator(x);
}

// Removes a range of elements from the map.
// The range is defined by the key values of the first and last iterators
// first is the first element removed and last is the element just beyond the last elemnt removed.
// @returns The iterator that designates the first element remaining beyond any elements removed.
TreeMap::iterator TreeMap::erase(TreeMap::iterator f, TreeMap::iterator l)
{
	///@todo Implement this
	assert(f.node->data.first <= l.node->data.first); // sentinel (end) is INT_MAX so this assert is ok
	while (f != l)
	{
		f = erase(f);
	}
	return l;
}

// Removes an element from the map.
// @returns The number of elements that have been removed from the map.
//			 Since this is not a multimap itshould be 1 or 0.
TreeMap::size_type TreeMap::erase(const Key& key)
{
	///@todo Implement this
	iterator i = find(key);
	if (i == end()) return 0; // nie znaleziono
	erase(i);
	return 1;
}

// Erases all the elements of a map.
void TreeMap::clear( )
{
	//TreeMapDetail::erase(this, 0);  /// Stupid helper usage example
	///@todo Implement this
	TreeMapDetail::deleteAll(root->left);
	root->left = NULL;
	detail->size = 0;
}

bool TreeMap::struct_eq(const TreeMap& another) const
{
	///@todo Implement this
	if (size() != another.size())
		return false;

	for (iterator i = another.begin(), j = begin(); j != end(); ++i, ++j)
	{
		if (*i != *j)
		{
			return false;
		}
	}

	return true;
}

bool TreeMap::info_eq(const TreeMap& another) const
{
	///@todo Implement this
	if (size() != another.size())
		return false;

	for (iterator i = another.begin(); i != another.end(); ++i)
	{
		if (find(i->first) == end())
		{
			return false;
		}
	}

	return true;
}

// preincrement
TreeMap::const_iterator& TreeMap::const_iterator::operator ++()
{
	///@todo Implement this
	assert(node->parent != NULL);
	node = TreeMapDetail::succ(node);
	return *this;
}

// postincrement
TreeMap::const_iterator TreeMap::const_iterator::operator++(int)
{
	///@todo Implement this
	assert(node->parent != NULL);
	const_iterator tmp = *this;
	node = TreeMapDetail::succ(node);
	return tmp;
}

// predecrement
TreeMap::const_iterator& TreeMap::const_iterator::operator--()
{
	///@todo Implement this
	TreeNode * t = TreeMapDetail::pred(node);
	assert(t != NULL);	// zapobiega przejsciu przed pierwszy element
	node = t;
	return *this;
}

// postdecrement
TreeMap::const_iterator TreeMap::const_iterator::operator--(int)
{
	///@todo Implement this
	const_iterator tmp = *this;
	TreeNode * t = TreeMapDetail::pred(node);
	assert(t != NULL);	// zapobiega przejsciu przed pierwszy element
	node = t;
	return tmp;
}


/// Assignment operator copy the source elements into this object.
TreeMap& TreeMap::operator=(const TreeMap& another)
{
	///@todo Implement this
	clear();
	root->left = TreeMapDetail::copyAll(another.root->left, root);
	detail->size = another.detail->size; // tree size ;-)
	return *this;
}
		
/// Returns an iterator addressing the first element in the map
TreeMap::iterator TreeMap::begin()
{ 
	///@todo Implement this
	return iterator(TreeMapDetail::treeMinimum(root));
}

TreeMap::const_iterator TreeMap::begin() const
{ 
	///@todo Implement this
	return iterator(TreeMapDetail::treeMinimum(root));
}

/// Returns an iterator that addresses the location succeeding the last element in a map
TreeMap::iterator TreeMap::end()
{ 
	///@todo Implement this
	return iterator(root);
}

/// Returns an iterator that addresses the location succeeding the last element in a map
TreeMap::const_iterator TreeMap::end() const
{ 
	///@todo Implement this
	return iterator(root);
}

void test()
{
	
	   // A typedef used by the test.
	   //typedef std::map<int, std::string> TEST_MAP;
	   //typedef SmallMap<int, std::string> TEST_MAP;
	   typedef TreeMap TEST_MAP;


	   std::cerr << "Testy uzytkownika" << std::endl;

	        TEST_MAP m;
	        TreeMap::iterator iterator, i;
	   
	   m[2] = "Merry";
	   m[4] = "Jane";
	   m[8] = "Korwin";
	   m[6] = "Weiss";
	   m[7] = "Kain";
	   m[0] = "Abel";
	   m[9] = "Moses";
	   m[201];m[589];m[495];m[927];m[126];m[661];m[339];m[265];m[498];m[23];m[641];m[111];m[709];m[229];m[291];m[924];m[313];m[147];m[755];m[189];m[19];m[209];m[413];m[728];m[491];m[180];m[510];m[734];m[578];m[24];m[430];m[362];m[630];m[80];m[352];m[462];m[139];m[50];m[75];m[584];m[385];m[445];m[191];m[386];m[505];m[325];m[216];m[464];m[844];m[290];m[593];m[482];m[694];m[615];m[286];m[698];m[19];m[866];m[321];m[901];m[395];m[262];m[526];m[372];m[424];m[637];m[313];m[294];m[399];m[777];m[433];m[548];m[510];m[838];m[425];m[881];m[637];m[39];m[613];m[884];m[344];m[507];m[267];m[217];m[69];m[420];m[489];m[613];m[729];m[248];m[931];m[28];m[480];m[570];m[217];m[596];m[197];m[89];m[71];m[602];m[660];m[639];m[645];m[723];m[187];m[874];m[285];m[407];m[154];m[445];m[110];m[346];m[807];m[806];m[527];m[769];m[181];m[839];m[770];m[843];   TreeMap::iterator eraseIterator = m.begin();
	   
	   for(i = m.end(); i != m.begin(); --i)
		   std::cout << i->first << " " << i->second << std::endl;
	   
	   
	   
	   for(i = m.end(); i != m.begin(); --i)
		   std::cout << i->first << " " << i->second << std::endl;
	   
	   iterator = m.begin();
	   iterator++;iterator++;iterator++;
	   
	   m.erase(m.begin(), iterator);
	   
	   for(i = m.end(); i != m.begin(); --i)
		   std::cout << i->first << " " << i->second << std::endl;
	   
	   for(i = m.begin(); i !=m.end(); i++)
		   std::cout << i->first << " " << i->second << std::endl;

	   std::cout << " beg : " <<  (m.begin())->first << " " << (m.begin())->second << std::endl;
	   std::cout << " end : " << (m.end())->first << " " << (m.end())->second << std::endl;
	   m.erase(m.begin(), m.end());
}

//////////////////////////////////////////////////////////////////////////////
// main - jest w pliku /home/common/dyd/aisdi/z2/main.cc
//////////////////////////////////////////////////////////////////////////////
//
//int main()
//{
//	std::cout << "AISDI cwiczenie 4: wchodze do funkcji main." << std::endl;
//	test();
//	// Biblioteka z bardziej rygorystyczna wersja tych testow bedzie udostepniona na nastepnych zajeciach.
//	//Test2();
//	system("PAUSE");
//	return EXIT_SUCCESS;
//}
