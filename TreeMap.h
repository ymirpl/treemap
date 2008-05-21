/** 
@file TreeMap.h

Zawiera deklaracje klasy TreeMap.
Jest to mapa (Slownik) zrobiona na drzewie binarnym.
Implementacja tej klasy jest przedmiotem cwiczenia 4
na laboratoriach z AISDI.
      
@author
Pawel Cichocki, Michal Nowacki

@date  
last revision
- 2005.12.01 Pawel Cichocki: More TreeNode constructors
- 2005.12.01 Pawel Cichocki: TreeNodeDetail class
- 2005.12.01 Michal Nowacki: Lab #4
- 2005.11.17 Michal Nowacki: prev pointer
- 2005.11.04 Pawel Cichocki: cosmetic code improvements
- 2005.11.04 Pawel Cichocki: a public method for obtaining the count from CCount
- 2005.11.03 Pawel Cichocki: const_iterator done properly now
- 2005.10.27 Pawel Cichocki: cosmetic changes
- 2005.10.27 Michal Nowacki: removed templates, and moved the implementation to another file
- 2005.10.25 Pawel Cichocki: wrote it

COPYRIGHT:
Copyright (c) 2005 Instytut Informatyki, Politechnika Warszawska
ALL RIGHTS RESERVED
*******************************************************************************/

#include <assert.h>
#include <stdlib.h>
#include <iterator>

#include <string>

/// A simple instance counter for detecting memory leaks.
class CCount
{
private:
  static int count;
  CCount() {count++;}
  ~CCount()
  {
     assert(count>0);
     //if(count>0) 
        count--;
     //else
     //   cerr<<"ERROR (CCount): More destructors than constructors called!"<<std::endl;
  }
  friend struct TreeNode;

public:
   /// A public method for getting the count.
   static int getCount() { return count; }
};

//////////////////////////////////////////////////////////////////////////////
// TreeMap and related classes
//////////////////////////////////////////////////////////////////////////////

/// A class to package the data into so it has the left and right hook for the tree.
struct TreeNode : CCount
{
   typedef std::pair<int,std::string> T;
   TreeNode* parent;  ///< Parent node
   TreeNode* left;    ///< The left child in the tree
   TreeNode* right;   ///< The right child in the tree
   T data;            ///< User's data
   short b;            ///< balance
   TreeNode(const T& d) : parent(NULL), left(NULL), right(NULL), data(d), b(0) {}
   TreeNode(const T& d, TreeNode* l, TreeNode* r) : parent(NULL), left(l), right(r), data(d), b(0) {}
   TreeNode(const T& d, TreeNode* p) : parent(p), left(NULL), right(NULL), data(d), b(0) {}
   TreeNode(const T& d, TreeNode* p, TreeNode* l, TreeNode* r) : parent(p), left(l), right(r), data(d), b(0) {}
   TreeNode(const T& d, short bal, TreeNode* p) : parent(p), left(NULL), right(NULL), data(d), b(bal) {} 
};

/// A map with a similar interface to std::map.
/// This map should be implemented as a binary tree.
class TreeMap
{
   friend class TreeMapDetail;
public:
   typedef int Key;
   typedef std::string Val;

protected:
   typedef TreeNode/*<Key, Value>*/ Node;
   Node* root;   ///< The root of the tree
   TreeMapDetail* detail;
public:
   typedef size_t size_type;
   typedef std::pair<Key, Val> P;

   TreeMap();
   TreeMap( const TreeMap& );
   ~TreeMap();

   /// A const_iterator.
   /// It also serves as a base for the (not const) iterator.
   class const_iterator : public std::iterator<std::bidirectional_iterator_tag,
                                               std::pair<Key, Val> >
   {
   public:
      typedef std::pair<Key, Val> T;

   protected:
      /// Points to the tree element
      Node* node;
      friend class TreeMap;

      const_iterator(Node* x) : node(x) {}
   public:
      const_iterator() {}
      const_iterator(const const_iterator& a) : node(a.node) {}

      inline const T& operator*() const
      {
         return node->data;
      }

      inline const T* operator->() const
      {
         return &(node->data);
      }

      // preincrement
      /*inline*/ const_iterator& operator++();
      // postincrement
      /*inline*/ const_iterator operator++(int);
      // predecrement
      /*inline*/ const_iterator& operator--();
      // postdecrement
      /*inline*/ const_iterator operator--(int);

      inline bool operator==(const const_iterator& a) const
      {
         return node == a.node;
      }

      inline bool operator!=(const const_iterator& a) const
      {
         return node != a.node;
      }
   };

   /// An iterator.
   class iterator : public const_iterator
   {
      iterator(Node* x) : const_iterator(x) {}
      friend class TreeMap;

   public:
      iterator() {}
      iterator(const const_iterator& a) : const_iterator(a) {}
      iterator(const iterator& a) { node = a.node; }

      inline T& operator*() const
      {
         return node->data;
      }
      inline T* operator->() const
      {
         return &(node->data);
      }

      iterator& operator++()
      {  // preincrement
         ++(*(const_iterator*)this);
         return (*this);
      }

      iterator operator++(int)
      {  // postincrement
         iterator temp = *this;
         ++*this;
         return temp;
      }

      iterator& operator--()
      {  // predecrement
         --(*(const_iterator*)this);
         return (*this);
      }

      iterator operator--(int)
      {  // postdecrement
         iterator temp = *this;
         --*this;
         return temp;
      }
   };
   
   /// Returns an iterator addressing the first element in the map
   iterator begin();
   /// Returns a const_iterator addressing the first element in the map
   const_iterator begin() const;

   /// Returns an iterator that addresses the location succeeding the last element in a map
   iterator end();
   /// Returns a const_iterator that addresses the location succeeding the last element in a map
   const_iterator end() const;
   
   /// Inserts an element into the map.
   /// @returns A pair whose bool component is true if an insertion was
   ///          made and false if the map already contained an element
   ///          associated with that key, and whose iterator component coresponds to
   ///          the address where a new element was inserted or where the element
   ///          was already located.
   std::pair<iterator, bool> insert(const std::pair<Key, Val>& entry);

   /// Inserts an element into the map.
   /// This method assumes there is no value asociated with
   /// such a key in the map.
   iterator unsafe_insert(const std::pair<Key, Val>& entry);

   /// Returns an iterator addressing the location of the entry in the map
   /// that has a key equivalent to the specified one or the location succeeding the
   /// last element in the map if there is no match for the key.
   iterator find(const Key& k);
   const_iterator find(const Key& k) const;
 
   //bool belongs( const Key& k) const;

   /// Inserts an element into a map with a specified key value
   /// if one with such a key value does not exist.
   /// @returns Reference to the value component of the element defined by the key.
   Val& operator[](const Key& k);   

   /// Tests if a map is empty.
   bool empty( ) const;

   /// Returns the number of elements in the map.
   size_type size() const;

   /// Returns the number of elements in a map whose key matches a parameter-specified key.
   size_type count(const Key& _Key) const;

   /// Removes an element from the map.
   /// @returns The iterator that designates the first element remaining beyond any elements removed.
   iterator erase(iterator i);
   
   /// Removes a range of elements from the map.
   /// The range is defined by the first and last iterators
   /// first is the first element removed and last is the element just beyond the last elemnt removed.
   /// @returns The iterator that designates the first element remaining beyond any elements removed.
   iterator erase(iterator first, iterator last);
   
   /// Removes an element from the map.
   /// @returns The number of elements that have been removed from the map.
   ///          Since this is not a multimap itshould be 1 or 0.
   size_type erase(const Key& key);

   /// Erases all the elements of a map.
   void clear( );
   
   /// Returns true if this map's internal structure is identical to another map's structure.
   bool struct_eq(const TreeMap& another) const;
   /// Returns true if this map contains exactly the same key-value pairs as the another map.
   bool info_eq(const TreeMap& another) const;

   /// Returns true if this map contains exactly the same key-value pairs as the another map. 
   inline bool TreeMap::operator==(const TreeMap& a) const { return info_eq(a); }
   
   /// Assignment operator copy the source elements into this object.
   TreeMap& operator=(const TreeMap& );
};

