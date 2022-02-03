#pragma once
#include <iostream>
#include "Entry.hpp"
typedef std::string Key;

class HashTable {
public:
   //Creates empty HashTabes with size(DEFAULT_CAPACITY by default ) capacity
   HashTable(size_t size = DEFAULT_CAPACITY);
   ~HashTable();

   //Creates HashTabes based on b. Old value of hashtable deleted
   HashTable& operator=(const HashTable& b);

   //Creates HashTabes based on b.
   HashTable(const HashTable& b);

   //swaps the HashTabes
   void swap(HashTable& b);

   //clears the HashTabes, capacity doesn't changed
   void clear();

   //Removes the element (if one exists) with the key equivalent to key.
   //Returns false if the key equivalent to k doesn't exists
   bool erase(const Key& k);
   
   //Inserts element into the container
   //Returns false if the key already exists, but will overwrite it
   bool insert(const Key& k, const Value& v);

   //checks if the Value contains element with specific key
   //Returns false if the key equivalent to key doesn't exists
   bool contains(const Key& k) const;

   //Return a reference to the value of the existing element whose is equivalent to k.
   //Reference to the mapped value of the new element if no element with key k existed.  
   //Changing an object that was created using a non-existent key, all objects created in this way are changed same.
   Value& operator[](const Key& k);

   //Return a reference to the value of the existing element whose is equivalent to k.
   //exception out_of_range if no element with key k existed. 
   Value& at(const Key& k);
   const Value& at(const Key& k) const;
   
   //Returns the number of elements
   size_t size() const;

   //Returns the capacity of HashTable
   size_t capacity() const;

   //checks whether the hashtable is empty
   bool empty() const;

   //Returns true if a equal b. Return false if a not equal b
   friend bool operator==(const HashTable& a, const HashTable& b);

   //Returns true if a not equal b. Return false if a equal b
   friend bool operator!=(const HashTable& a, const HashTable& b);

   //Prints HashTable
   void operator<<(const HashTable& a) const;
   
private:
   class HashList{
    public:

      HashList();

      HashList(HashList & b);

      HashList(const Key& k, const Value& v, Entry* next = nullptr);

      ~HashList();

      void insert(const Key& k, const Value& v);

      bool search(const Key& k) const;

      void printList() const;

      bool remove(const Key& k);

      Value* search(const Key& k);

      HashList& operator=(const HashList& other);

      Entry* pop();

      Entry* get_head();

      void freeList();

      void reverse();

      bool operator==(const HashList& b);

      bool operator!=(const HashList& b);

      size_t get_size();
    private:
      Entry* head_ = nullptr;
   };
   HashList** list_;
   size_t capacity_;
   size_t size_;
   static const size_t DEFAULT_CAPACITY = 16;
   static constexpr double RESIZE_ON = 0.75;

   size_t hashF(const Key& k) const;

   //if hashtable overflows сhange the size 
   bool resize();

   Value& get_value_by_key(const Key& k) const;
   bool is_not_equal_table(const HashTable& b) const;
};