/*
 Yida Wang
 Last date modified: November 4th, 2019

=======================
Hash Table Project:
=======================
This file provides definition for the structs and functions declared in the
header file. It also contains helper functions that are not accessible from
outside of the file.

===================================
Naming conventions in this file:
===================================
1. All struct names use camel case where the first letter is capitalized.
  e.g. "HashTable", or "HashTableEntry"

2. Variable names with a preceding underscore "_" will not be called directly.
  e.g. "_HashTable", "_HashTableEntry"

  Recall that in C, we have to type "struct" together with the name of the struct
  in order to initialize a new variable. To avoid this, in hash_table.h
  we use typedef to provide new "nicknames" for "struct _HashTable" and
  "struct _HashTableEntry". As a result, we can create new struct variables
  by just using:
    - "HashTable myNewTable;"
     or
    - "HashTableEntry myNewHashTableEntry;"

  The preceding underscore "_" simply provides a distinction between the names
  of the actual struct definition and the "nicknames" that we use to initialize
  new structs.
  [See Hidden Definitions section for more information.]

3. Functions, their local variables and arguments are named with camel case, where
  the first letter is lower-case.
  e.g. "createHashTable" is a function. One of its arguments is "numBuckets".
       It also has a local variable called "newTable".

4. The name of a struct member is divided by using underscores "_". This serves
  as a distinction between function local variables and struct members.
  e.g. "num_buckets" is a member of "HashTable".

*/

/****************************************************************************
* Include the Public Interface
*
* By including the public interface at the top of the file, the compiler can
* enforce that the function declarations in the the header are not in
* conflict with the definitions in the file. This is not a guarantee of
* correctness, but it is better than nothing!
***************************************************************************/
#include "hash_table.h"


/****************************************************************************
* Include other private dependencies
*
* These other modules are used in the implementation of the hash table module,
* but are not required by users of the hash table.
***************************************************************************/
#include <stdlib.h>   // For malloc and free
#include <stdio.h>    // For printf


/****************************************************************************
* Hidden Definitions
*
* These definitions are not available outside of this file. However, because
* the are forward declared in hash_table.h, the type names are
* available everywhere and user code can hold pointers to these structs.
***************************************************************************/
/**
 * This structure represents an a hash table.
 * Use "HashTable" instead when you are creating a new variable. [See top comments]
 */
struct _HashTable {
  /** The array of pointers to the head of a singly linked list, whose nodes
      are HashTableEntry objects */
  HashTableEntry** buckets;

  /** The hash function pointer */
  HashFunction hash;

  /** The number of buckets in the hash table */
  unsigned int num_buckets;
};

/**
 * This structure represents a hash table entry.
 * Use "HashTableEntry" instead when you are creating a new variable. [See top comments]
 */
struct _HashTableEntry {
  /** The key for the hash table entry */
  unsigned int key;

  /** The value associated with this hash table entry */
  void* value;

  /**
  * A pointer pointing to the next hash table entry
  * NULL means there is no next entry (i.e. this is the tail)
  */
  HashTableEntry* next;
};


/****************************************************************************
* Private Functions
*
* These functions are not available outside of this file, since they are not
* declared in hash_table.h.
***************************************************************************/
/**
* createHashTableEntry
*
* Helper function that creates a hash table entry by allocating memory for it on
* the heap. It initializes the entry with key and value, initialize pointer to
* the next entry as NULL, and return the pointer to this hash table entry.
*
* @param key The key corresponds to the hash table entry
* @param value The value stored in the hash table entry
* @return The pointer to the hash table entry
*/
static HashTableEntry* createHashTableEntry(unsigned int key, void* value) {

    // Allocate memory for the new HashTableEntry struct on heap
    HashTableEntry* newEntry = (HashTableEntry*)malloc(sizeof(HashTableEntry));

    // Initialize the components of the new HashTableEntry struct
    newEntry -> key = key;              // key
    newEntry -> value = value;          // value
    newEntry -> next = NULL;            // next entry = NULL

    return newEntry;                    // return the pointer to this hash table entry
}

/**
* findItem
*
* Helper function that checks whether there exists the hash table entry that
* contains a specific key.
*
* @param hashTable The pointer to the hash table.
* @param key The key corresponds to the hash table entry
* @return The pointer to the hash table entry, or NULL if key does not exist
*/
static HashTableEntry* findItem(HashTable* hashTable, unsigned int key) {
    // retrieve key from hashTable for buckets's index
    unsigned int index = hashTable->hash(key);
    // initialize thisNode as the head of the bucket
    HashTableEntry* thisNode = hashTable->buckets[index];
    // while thisNode is not NULL
    while (thisNode) {
        if (thisNode->key == key) return thisNode;  // if key is the same, return that entry
        thisNode = thisNode->next;                  // if not, go to next entry
    }
    // return NULL if thisNode is not found.
    return NULL;
}

/****************************************************************************
* Public Interface Functions
*
* These functions implement the public interface as specified in the header
* file, and make use of the private functions and hidden definitions in the
* above sections.
****************************************************************************/
// The createHashTable is provided for you as a starting point.
HashTable* createHashTable(HashFunction hashFunction, unsigned int numBuckets) {
  // The hash table has to contain at least one bucket. Exit gracefully if
  // this condition is not met.
  if (numBuckets==0) {
    printf("Hash table has to contain at least 1 bucket...\n");
    exit(1);
  }

  // Allocate memory for the new HashTable struct on heap.
  HashTable* newTable = (HashTable*)malloc(sizeof(HashTable));

  // Initialize the components of the new HashTable struct.
  newTable->hash = hashFunction;
  newTable->num_buckets = numBuckets;
  newTable->buckets = (HashTableEntry**)malloc(numBuckets*sizeof(HashTableEntry*));

  // As the new buckets contain indeterminant values, init each bucket as NULL.
  unsigned int i;
  for (i=0; i<numBuckets; ++i) {
    newTable->buckets[i] = NULL;
  }

  // Return the new HashTable struct.
  return newTable;
}

void destroyHashTable(HashTable* hashTable) {
    // loop through all buckets
    for (unsigned int i = 0; i < (hashTable->num_buckets); ++i) {
        // if bucket's head entry exist, delete all entries
        if (hashTable->buckets[i])
        {
            // keep track of entries
            // thisNode is the current entry, or the ith index in the bucket
            HashTableEntry* thisNode = hashTable->buckets[i];
            // nextNode is the next entry
            HashTableEntry* nextNode = thisNode->next;
            // while the next entry still exist, free current entry
            while(nextNode)
            {
                free(thisNode->value);          // free the value in current entry
                free(thisNode);                 // free the current entry
                thisNode = nextNode;            // current entry become the next entry
                nextNode = nextNode->next;      // next entry become the entry after next entry
            }
            // if the next entry is NULL, delete current entry
            free(thisNode->value);
            free(thisNode);
        }
    }
    // destroy hashTable
    free(hashTable->buckets);
    free(hashTable);
}

void* insertItem(HashTable* hashTable, unsigned int key, void* value) {
    // retrieve key from hashTable for buckets's index
    unsigned int bucketIndex = hashTable->hash(key);
    // initialize currentNode from findItem function using the key
    HashTableEntry* currentNode = findItem(hashTable, key);
    // if current entry exist
    if (currentNode)
    {
        // declare a previous value in the entry
        // retrieve the previous value from the entry
        void* previousValue = currentNode->value;;
        // replace the value in current entry with value from parameter
        currentNode->value = value;
        // return the previous value
        return previousValue;
    }
    // if the current entry does not exist, create a new entry
    // with specified key and value from parameter
    HashTableEntry* thisNode = createHashTableEntry(key, value);
    // if create entry failed, return NULL
    if (!thisNode) return NULL;
    // the next entry points to the head to make the list loop
    thisNode->next = hashTable->buckets[bucketIndex];
    // head points to current entry
    hashTable->buckets[bucketIndex] = thisNode;
    // return NULL since we created this new entry
    return NULL;
}

void* getItem(HashTable* hashTable, unsigned int key) {
    // initialize currentNode from findItem function using the key
    HashTableEntry* currentNode = findItem(hashTable, key);
    // if current entry exist
    if (currentNode)
    {
        // return the value in current entry
        return currentNode->value;
    }
    // otherwise return NULL
    return NULL;
}

void* removeItem(HashTable* hashTable, unsigned int key) {
    // retrieve key from hashTable for buckets's index
    unsigned int bucketIndex = hashTable->hash(key);
    // initialize thisNode as the head of the bucket
    HashTableEntry* thisNode = hashTable->buckets[bucketIndex];
    // if the head exist AND the head has the key we looking for
    if (thisNode && thisNode->key == key)
    {
        // retrieve the value from the head and store it
        void* removedEntryValue = thisNode->value;
        // change the head points to the next entry
        hashTable->buckets[bucketIndex] = thisNode->next;
        // free the head
        free(thisNode);
        // return the value was in the head
        return removedEntryValue;
    }
    // while the head AND next entry exist
    while (thisNode && thisNode->next)
    {
        // if the next entry has the key
        if (thisNode->next->key == key)
        {
            // store next entry pointer in tmp pointer
            HashTableEntry* tmp = thisNode->next;
            // retrieve the value from tmp and store it
            void* removedEntryValue = tmp->value;
            // the next entry points to the entry after next entry
            thisNode->next = thisNode->next->next;
            // free the tmp pointer
            free(tmp);
            // return the value was in the next entry
            return removedEntryValue;
        }
        // if key is not in next entry, go to next entry
        thisNode = thisNode->next;
    }
    // return NULL if the key is not present in the table
    return NULL;
}

void deleteItem(HashTable* hashTable, unsigned int key) {
    // retrieve key from hashTable for buckets's index
    unsigned int bucketIndex = hashTable->hash(key);
    // initialize thisNode as the head of the bucket
    HashTableEntry* thisNode = hashTable->buckets[bucketIndex];
    // if the head does not exist, return
    if (findItem(hashTable, key) == NULL) return;
    // if the head exist and the key is in the head
    if (thisNode && thisNode->key == key)
    {
        // redirect the head points to the next entry
        hashTable->buckets[bucketIndex] = thisNode->next;
        // delete the value in the head
         free(thisNode->value);
        // delete the current entry
        free(thisNode);
        return;
    }
    // while the head and next entry exist
    while (thisNode && thisNode->next)
    {
        // if the next entry has the key
        if(thisNode->next->key == key)
        {
            // initialize a tmp pointer points to the next entry
            HashTableEntry* tmp = thisNode->next;
            // redirect the next entry points to the entry after next entry
            thisNode->next = thisNode->next->next;
            // delete the value in next entry
             free(tmp->value);
            // free the tmp pointer
            free(tmp);
            return;
        }
        // if key is not in next entry, go to next entry
        thisNode = thisNode->next;
    }
}