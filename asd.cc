
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

	// BST search
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
	
	
	// returns next element (inorder)
	static TreeNode* pred(TreeNode* node) {
		assert(node != NULL);

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

	// recursively delete tree
	static void deleteAll(TreeNode* node) {
		if (node == NULL)
			return;
		if (node->left != NULL)
			deleteAll(node->left);
		if (node->right != NULL)
			deleteAll(node->right);
		delete node;
	}
	
	// rocursively copy tree
	static TreeNode* copyAll(TreeNode* node, TreeNode* parent) {
		if (node == NULL)
			return NULL;
		TreeNode* ret = new TreeNode(node->data, parent);
		ret->left = copyAll(node->left, ret);
		ret->right = copyAll(node->right, ret);
		return ret;
	}
	
	/**
	 * Method swapping two elements
	 */
	static void swap(TreeNode *one, TreeNode *two) {
		//najpierw podmienimy dzieci
		TreeNode* tmp;
		
		//std::cerr << "Swappin " << one->data.first << " with: " << two->data.first << std::endl;

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
	}
	
	if (entry.first <= parent->data.first)
		parent->left = newNode;
	else
		parent->right = newNode; // link to the proper side of parent
	
//	std::cerr << "inserted " << newNode << " with Parent " << newNode->parent << std::endl;
	
	return std::make_pair(iterator(newNode), true);
}

// Inserts an element into the map.
// This method assumes there is no value asociated with
// such a key in the map.
TreeMap::iterator TreeMap::unsafe_insert(const std::pair<Key, Val>& entry)
{
	///@todo Finnish this (Finnish HIM! :))

	
	Node* crawler = root->left; // first significant node
	Node* parent = root;
	
	// Token sais whether we took right son. If so, new element isn't new begin().
	// Otherwise it is. 
	while (crawler != NULL) {
		parent = crawler;
		if (entry.first < crawler->data.first)
			crawler = crawler->left;
		else 
			crawler = crawler->right;
	}
	
	Node* newNode = new TreeNode(entry, parent); 
	
	++(detail->size); // increase size counter
	
	
	if (parent == root) {
		root->left = newNode; // we only link to the left sie of root (sentinel)
	//	std::cerr << "inserted " << newNode << " with Parent root" << std::endl;
		return iterator(newNode);
		
	}
	
	if (entry.first <= parent->data.first)
		parent->left = newNode;
	else
		parent->right = newNode; // link to the proper side of parent
	
	//std::cerr << "inserted " << newNode << " with Parent " << newNode->parent << std::endl;
	
	return iterator(newNode);
}

// Returns an iterator addressing the location of the entry in the map
// that has a key equivalent to the specified one or the location succeeding the
// last element in the map if there is no match for the key.
TreeMap::iterator TreeMap::find(const Key& k) {
	TreeNode * node = TreeMapDetail::treeSearch(root->left, k);
	if (node == NULL)
		return end();
	else
		return iterator(node);
}

TreeMap::const_iterator TreeMap::find(const Key& k) const {
	TreeNode * node = TreeMapDetail::treeSearch(root->left, k);
	if (node == NULL)
		return end();
	else
		return const_iterator(node);
}

// Inserts an element into a map with a specified key value
// if one with such a key value does not exist.
// @returns Reference to the value component of the element defined by the key.
TreeMap::Val& TreeMap::operator[](const Key& k){
	iterator i = insert(std::make_pair(k, "")).first;
	return i->second;
}

// Tests if a map is empty.
bool TreeMap::empty() const {
	return root->left==NULL;
}

// Returns the number of elements in the map.
TreeMap::size_type TreeMap::size() const {
	return detail->size;
}

// Returns the number of elements in a map whose key matches a parameter-specified key.
TreeMap::size_type TreeMap::count(const Key& _Key) const {
	return find(_Key) != end() ? 1 : 0; // this is not a multimap
}

// Removes an element from the map.
// @returns The iterator that designates the first element remaining beyond any elements removed.
TreeMap::iterator TreeMap::erase(TreeMap::iterator i)
{
	
	Node* node;
	node = i.node; // node is element to delete
	Node* tmp;
	TreeMap::iterator ret;
	

	   if (i.node == root) // can't delete sentinel
		return end();
	
	if (node->right == NULL && node->left == NULL) { // node is leaf
		ret = i;
		ret++;
		
		//std::cerr<< "Lisc " <<std::endl;
		//std::cerr << "Erasing: " << node->data.first << " ret: " << ret.node->data.first <<  "  " << ret.node->data.second << std::endl;
				
		if (node->parent->left == node)
			node->parent->left = NULL;
		else
			node->parent->right = NULL;

		delete node;
		(detail->size)--; // decrease size
		
		return ret;
	}
	
	if (node->right!=NULL && node->left != NULL) { // node has two subtrees
		//std::cerr<< "Dwa poddrze swap" <<std::endl;
		
		TreeMap::iterator ret = i;
		ret++;
		
		TreeMapDetail::swap(i.node, ret.node );
		//std::cerr << "Erasing: " << node->data.first << " ret: " << ret.node->data.first <<  "  " << ret.node->data.second << std::endl;
		
		erase(i); // proper erase
		return ret;
		
	}
		
	
	
	if (node->right != NULL) { // one right subtree
		//std::cerr<< "jeden prawy" <<std::endl;
		ret = i; // we return node after delted
		ret++;
		
		// we're linking "over" deleted node
		if (node->parent->right == node)
			node->parent->right = node->right;
		else
			node->parent->left = node->right;

		node->right->parent = node->parent;
		
		
		//std::cerr << "Erasing: " << i.node->data.first << " ret: " << ret.node->data.first << "  " << ret.node->data.second << std::endl;

		delete node;
		(detail->size)--; // decrease size
		return ret;
	} else { // node has one left subtree
		//std::cerr<< "jeden lewy" <<std::endl;
		 ret = i;
		 ret++;
		
		 // linking "over" delted node 
		if (node->parent->right == node)
			node->parent->right = node->left;
		else
			node->parent->left = node->left;

		node->left->parent = node->parent;
		//std::cerr << "Erasing: " << i.node->data.first << " ret: " << ret.node->data.first << "  " << ret.node->data.second << std::endl;

		
		delete node;
		(detail->size)--; // decrease size
	
		return ret;

	}

}

// Removes a range of elements from the map.
// The range is defined by the key values of the first and last iterators
// first is the first element removed and last is the element just beyond the last elemnt removed.
// @returns The iterator that designates the first element remaining beyond any elements removed.
TreeMap::iterator TreeMap::erase(TreeMap::iterator f, TreeMap::iterator l)
{
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
TreeMap::size_type TreeMap::erase(const Key& key) {
	iterator i = find(key);
	
	if (i == end())
		return 0; // not found
	
	erase(i);
	return 1;
}

// Erases all the elements of a map.
void TreeMap::clear() {
	TreeMapDetail::deleteAll(root->left);
	root->left = NULL;
	detail->size = 0;
}

bool TreeMap::struct_eq(const TreeMap& another) const {
	if (size() != another.size())
		return false;

	for (iterator i = another.begin(), j = begin(); j != end(); ++i, ++j) {
		if (*i != *j) {
			return false;
		}
	}

	return true;
}

bool TreeMap::info_eq(const TreeMap& another) const {
	if (size() != another.size())
		return false;

	for (iterator i = another.begin(); i != another.end(); ++i) {
		if (find(i->first) == end()) {
			return false;
		}
	}

	return true;
}

// preincrement
TreeMap::const_iterator& TreeMap::const_iterator::operator ++() {
	node = TreeMapDetail::succ(node);
	return *this;
}

// postincrement
TreeMap::const_iterator TreeMap::const_iterator::operator++(int) {
	const_iterator tmp = *this;
	node = TreeMapDetail::succ(node);
	return tmp;
}

// predecrement
TreeMap::const_iterator& TreeMap::const_iterator::operator--() {
	TreeNode * t = TreeMapDetail::pred(node);
	//assert(t != NULL);
	node = t;
	return *this;
}

// postdecrement
TreeMap::const_iterator TreeMap::const_iterator::operator--(int){

	const_iterator tmp = *this;
	TreeNode * t = TreeMapDetail::pred(node);
	//assert(t != NULL);
	node = t;
	return tmp;
}


/// Assignment operator copy the source elements into this object.
TreeMap& TreeMap::operator=(const TreeMap& another) {
	clear();
	root->left = TreeMapDetail::copyAll(another.root->left, root);
	detail->size = another.detail->size; 
	return *this;
}
		

/// Returns an iterator addressing the first element in the map
TreeMap::iterator TreeMap::begin() {
	return iterator(TreeMapDetail::treeMinimum(root));
}

TreeMap::const_iterator TreeMap::begin() const {
	return const_iterator(TreeMapDetail::treeMinimum(root));
}

/// Returns an iterator that addresses the location succeeding the last element in a map
TreeMap::iterator TreeMap::end() {
	return iterator(root);
}

/// Returns an iterator that addresses the location succeeding the last element in a map
TreeMap::const_iterator TreeMap::end() const {
	return const_iterator(root);
}

void test()
{
/*	
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
	   m[342];m[532];m[655];m[724];m[826];m[706];m[553];m[166];m[518];m[637];m[769];m[351];m[463];m[546];m[406];m[890];m[529];m[822];m[904];m[734];m[507];m[143];m[611];m[11];m[429];m[909];m[26];m[306];m[319];m[47];m[11];m[851];m[108];m[594];m[204];m[520];m[904];m[254];m[28];m[73];m[707];m[364];m[71];m[734];m[914];m[766];m[178];m[131];m[105];m[334];m[343];m[474];m[712];m[815];m[632];m[459];m[734];m[653];m[582];m[322];m[630];m[825];m[467];m[47];m[195];m[747];m[534];m[577];m[646];m[784];m[678];m[264];m[825];m[128];m[932];m[450];m[403];m[784];m[568];m[822];m[73];m[459];m[61];m[607];m[511];m[826];m[814];m[479];m[502];m[527];m[517];m[662];m[864];m[707];m[759];m[56];m[491];m[499];m[890];m[317];m[871];m[118];m[183];m[782];m[173];m[642];m[872];m[928];m[427];m[648];m[891];m[502];m[437];m[347];m[905];m[551];m[215];m[876];m[621];m[729];	   
	   for(i = m.end(); i != m.begin(); --i)
		   std::cout << i->first << " " << i->second << std::endl;
	   

	   
	   iterator = m.begin();
	   iterator++;iterator++;iterator++;
	   iterator++;iterator++;iterator++;
	   iterator++;iterator++;iterator++;
	   
	   m.erase(m.begin(), iterator);
	   
	   std::cout << "====== AFTER ERASE ======= " << std::endl;
	   for(i = m.end(); i != m.begin(); --i)
		   std::cout << i->first << " " << i->second << std::endl;
	   
	   std::cout << "====== AFTER ERASE2 ======= " << std::endl;
	   for(i = m.begin(); i !=m.end(); i++)
		   std::cout << i->first << " " << i->second << std::endl;

	   std::cout << " beg : " <<  (m.begin())->first << " " << (m.begin())->second << std::endl;
	   std::cout << " end : " << (m.end())->first << " " << (m.end())->second << std::endl;
	   std::cout << " count[201] " << m.count(201) << std::endl;
	   m.erase(m.begin(), m.end());*/
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
