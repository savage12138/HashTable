# Hash Table
Building to a given specification (the Application Programmer Interface, or API) and verifying correctness of the implementation are important concerns in software engineering. APIs establish a sort of contract between the creators and users of a software library about its expected behavior and functionality. The interface also allows developers to separate the concerns of design and implementation of large projects. Early on, design can proceed without distraction from the details of implementation by instead producing a specification of required components and their interfaces. Later, the implementation of each module can proceed more independently from the design of the project as a whole. 

## Hash Table Library
The hash table is made of an array of buckets that can hold any arbitrary data from users. There
will be a hash function provided by the user that maps a key to an index for a specific bucket.
Each bucket can hold multiple entries of data and will be implemented as a singly linked list. An
overview of the hash table implementation is:

**Structs:**
* HashTable
* HashTableEntry

**Public Interface Functions:**
* createHashTable
* destroyHashTable
* insertItem
* getItem
* removeItem
* deleteItem

**Private Helper Functions:** (only in hash_table.c)
* createHashTableEntry
* findItem
* (Any other useful helper functions)

## Automated Testing
For this project, we introduce more powerful tools for writing
automated tests. By generating a comprehensive test suite that can run automatically, we can be
confident that our implementation meets the API specification. Automated tests are also useful
during development, since they let us evaluate changes quickly and alert us immediately if
anything breaks. 

We'll be using a combination of two tools for this part of the project: Google Test Framework, a
library for writing and automatically executing tests; and make, a build tool to ease compilation. 
