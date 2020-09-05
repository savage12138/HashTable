// Test cases file
// Yida Wang
// November 4th, 2019
// Inform the compiler that this included module is written in C instead of C++.
extern "C" {
	#include "hash_table.h"
}
#include "gtest/gtest.h"


// Use the TEST macro to define your tests.
//
// TEST has two parameters: the test case name and the test name.
// After using the macro, you should define your test logic between a
// pair of braces.  You can use a bunch of macros to indicate the
// success or failure of a test.  EXPECT_TRUE and EXPECT_EQ are
// examples of such macros.  For a complete list, see gtest.h.
//
// <TechnicalDetails>
//
// In Google Test, tests are grouped into test cases.  This is how we
// keep test code organized.  You should put logically related tests
// into the same test case.
//
// The test case name and the test name should both be valid C++
// identifiers.  And you should not use underscore (_) in the names.
//
// Google Test guarantees that each test you define is run exactly
// once, but it makes no guarantee on the order the tests are
// executed.  Therefore, you should write your tests in such a way
// that their results don't depend on their order.
//
// </TechnicalDetails>

// The #define directive defines a constant value that can be accessed throughout
// your code. Here it defines the default number of buckets in the hash table.
// You can change this number, but make sure to update the hash function with
// the right algorithm to compute the indices for buckets.
// For example, if the BUCKET_NUM is set to 5, the hash function should map a
// positive number to an integer between 0 and 4.
#define BUCKET_NUM  3

// Dummy value to store in hash table entry
// Please beware that any type of data (e.g. int, double, struct and etc.) can
// be stored in hash table for testing your hash table. Only the pointer to
// the data will be stored in the HashTableEntry.
struct HTItem {};

// Helper function for creating a lot of dummy values.
void make_items(HTItem* result[], unsigned n)
{
	// Populate the array with pointers to the dummy values.
	while(n--)
	{
		result[n] = (HTItem*) malloc(sizeof(HTItem));
	}
}

// A simple hash function that maps a positive number to an integer between 0~(BUCKET_NUM-1).
unsigned int hash(unsigned int key) {
	return key%BUCKET_NUM;
}

////////////////////////
// Initialization tests
////////////////////////
TEST(InitTest, CreateDestroyHashTable)
{
	HashTable* ht = createHashTable(hash, BUCKET_NUM);
	destroyHashTable(ht);
}

////////////////////////////////////////////////////////////////////////////////////////////
// This TEST is testing if the hash table is been correctly destroyed.
// If the hash table is been destroyed, getItem will not work properly
// since there is no hash table ht for getItem's parameter.
// If uncomment this test, the compiler will give a Segmentation fault (core dumped) error.
// This test should still pass without getItem (comment those three lines).
////////////////////////////////////////////////////////////////////////////////////////////
//TEST(InitTest, DestroyHashTable)
//{
//    HashTable* ht = createHashTable(hash, BUCKET_NUM);
//
//    // Create a list of items.
//    size_t num_items = 3;
//    HTItem* m[num_items];
//    make_items(m, num_items);
//
//    // Insert a list of items.
//    insertItem(ht, 3, m[0]);
//    insertItem(ht, 7, m[1]);
//    insertItem(ht, 19, m[2]);
//
//    destroyHashTable(ht);
//
//    // Check if it's been destroyed.
//    EXPECT_EQ(NULL, getItem(ht,3));
//    EXPECT_EQ(NULL, getItem(ht,7));
//    EXPECT_EQ(NULL, getItem(ht,19));
//
//}

////////////////
// Access tests
////////////////
TEST(AccessTest, GetKey_TableEmpty)
{
	HashTable* ht = createHashTable(hash, BUCKET_NUM);

	// Test when table is empty.
	EXPECT_EQ(NULL, getItem(ht, 0));
	EXPECT_EQ(NULL, getItem(ht, 1));
	EXPECT_EQ(NULL, getItem(ht, 2));

	// Test with index greater than the number of buckets.
	EXPECT_EQ(NULL, getItem(ht, 10));

	destroyHashTable(ht);
}

TEST(AccessTest, GetSingleKey)
{
  HashTable* ht = createHashTable(hash, BUCKET_NUM);

  // Create list of items
  size_t num_items = 1;
  HTItem* m[num_items];
  make_items(m, num_items);

  insertItem(ht, 0, m[0]);
  EXPECT_EQ(m[0], getItem(ht, 0));

  destroyHashTable(ht);    // dummy item is also freed here
}

TEST(AccessTest, GetKey_KeyNotPresent)
{
	HashTable* ht = createHashTable(hash, BUCKET_NUM);

	// Create a list of items to add to hash table.
	size_t num_items = 1;
	HTItem* m[num_items];
	make_items(m, num_items);

	// Insert one item into the hash table.
	insertItem(ht, 0, m[0]);

	// Test if the hash table returns NULL when the key is not found.
	EXPECT_EQ(NULL, getItem(ht, 1));

	// Destroy the hash table together with the inserted values
	destroyHashTable(ht);
}

TEST(AccessTest, GetKey_InvalidKey)
{
    HashTable* ht = createHashTable(hash, BUCKET_NUM);
    size_t num_items = 1;
    HTItem* m[num_items];
    make_items(m, num_items);

    // Insert one item into the hash table.
    insertItem(ht, 3, m[0]);

    EXPECT_EQ(m[0], getItem(ht, 3));
//    EXPECT_EQ(NULL, getItem(ht, -7));

    destroyHashTable(ht);
}

///////////////////
// Removal tests
///////////////////
TEST(RemoveTest, SingleValidRemove)
{
	HashTable* ht = createHashTable(hash, BUCKET_NUM);

	// Create a list of items to add to hash table.
	size_t num_items = 1;
	HTItem* m[num_items];
	make_items(m, num_items);

	// Insert one item into the hash table.
	insertItem(ht, 0, m[0]);

	// After removing an item with a specific key, the data stored in the
	// corresponding entry should be returned. If the key is not present in the
	// hash table, then NULL should be returned.
	void* data = removeItem(ht, 0);

	// Since the key we want to remove is present in the hash table, the correct
	// data should be returned.
	EXPECT_EQ(m[0], data);

	// Free the data
	free(data);

	destroyHashTable(ht);
}

TEST(RemoveTest, SingleInvalidRemove)
{
	HashTable* ht = createHashTable(hash, BUCKET_NUM);

	// When the hash table is empty, the remove funtion should still work.
	EXPECT_EQ(NULL, removeItem(ht, 1));

	destroyHashTable(ht);
}

TEST(RemoveTest, RemovedSameAsInserted)
{
    HashTable* ht = createHashTable(hash, BUCKET_NUM);

    // Create a list of items to add to hash table.
    size_t num_items = 3;
    HTItem* m[num_items];
    make_items(m, num_items);

    // Insert multiple items into the hash table.
    insertItem(ht, 3, m[0]);
    insertItem(ht, 7, m[1]);
    insertItem(ht, 19, m[2]);

    // Check if removed value is same as inserted value.
    EXPECT_EQ(m[0], removeItem(ht, 3));
    EXPECT_EQ(m[1], removeItem(ht, 7));
    EXPECT_EQ(m[2], removeItem(ht, 19));

    destroyHashTable(ht);
    free(m[0]);
    free(m[1]);
    free(m[2]);
}

TEST(RemoveTest, EmptyAfterRemoved)
{
    HashTable* ht = createHashTable(hash, BUCKET_NUM);

    // Create a list of items to add to hash table.
    size_t num_items = 3;
    HTItem* m[num_items];
    make_items(m, num_items);

    // Insert multiple items into the hash table.
    insertItem(ht, 3, m[0]);
    insertItem(ht, 7, m[1]);
    insertItem(ht, 19, m[2]);

    // Remove all the items we just inserted.
    removeItem(ht, 3);
    removeItem(ht, 7);
    removeItem(ht, 19);

    // Check if the current value is empty.
    EXPECT_EQ(NULL, getItem(ht, 3));
    EXPECT_EQ(NULL, getItem(ht, 7));
    EXPECT_EQ(NULL, getItem(ht, 19));

    destroyHashTable(ht);
    free(m[0]);
    free(m[1]);
    free(m[2]);
}

TEST(RemoveTest, RemoveHead)
{
    HashTable* ht = createHashTable(hash, BUCKET_NUM);

    // Create a list of items to add to hash table.
    size_t num_items = 3;
    HTItem* m[num_items];
    make_items(m, num_items);

    // Insert multiple items into the hash table.
    insertItem(ht, 3, m[0]);
    insertItem(ht, 7, m[1]);
    insertItem(ht, 19, m[2]);

    // Remove the head entry.
    removeItem(ht, 3);

    // Check if the entry is been removed.
    EXPECT_EQ(NULL, getItem(ht, 3));

    destroyHashTable(ht);
    free(m[0]);
}

TEST(RemoveTest, RemoveNext)
{
    HashTable* ht = createHashTable(hash, BUCKET_NUM);

    // Create a list of items to add to hash table.
    size_t num_items = 3;
    HTItem* m[num_items];
    make_items(m, num_items);

    // Insert multiple items into the hash table.
    insertItem(ht, 3, m[0]);
    insertItem(ht, 7, m[1]);
    insertItem(ht, 19, m[2]);

    // Remove the next entry after head.
    removeItem(ht, 7);

    // Check if the entry is been removed.
    EXPECT_EQ(NULL, getItem(ht, 7));

    destroyHashTable(ht);
    free(m[1]);
}

TEST(RemoveTest, RemoveLastItem)
{
    HashTable* ht = createHashTable(hash, BUCKET_NUM);

    // Create a list of items to add to hash table.
    size_t num_items = 3;
    HTItem* m[num_items];
    make_items(m, num_items);

    // Insert multiple items into the hash table.
    insertItem(ht, 3, m[0]);
    insertItem(ht, 7, m[1]);
    insertItem(ht, 19, m[2]);

    // Remove the last entry in bucket.
    removeItem(ht, 19);

    // Check if the entry is been removed.
    EXPECT_EQ(NULL, getItem(ht, 19));

    destroyHashTable(ht);
    free(m[2]);
}

///////////////////
// Insertion tests
///////////////////
TEST(InsertTest, InsertAsOverwrite)
{
	HashTable* ht = createHashTable(hash, BUCKET_NUM);

	// Create list of items to be added to the hash table.
	size_t num_items = 2;
	HTItem* m[num_items];
	make_items(m, num_items);

	// Only insert one item with key=0 into the hash table.
	insertItem(ht, 0, m[0]);

	// When we are inserting a different value with the same key=0, the hash table
	// entry should hold the new value instead. In the test case, the hash table entry
	// corresponding to key=0 will hold m[1] and return m[0] as the return value.
	EXPECT_EQ(m[0], insertItem(ht, 0, m[1]));

	// Now check if the new value m[1] has indeed been stored in hash table with
	// key=0.
	EXPECT_EQ(m[1], getItem(ht,0));

	destroyHashTable(ht);
	free(m[0]);    // don't forget to free item 0
}

TEST(InsertTest, InsertAll)
{
    HashTable* ht = createHashTable(hash, BUCKET_NUM);

    // Create list of items to be added to the hash table.
    size_t num_items = 4;
    HTItem* m[num_items];
    make_items(m, num_items);

    // Insert multiple items into the hash table.
    insertItem(ht, 3, m[0]);
    insertItem(ht, 7, m[1]);
    insertItem(ht, 19, m[2]);
    insertItem(ht, 38239, m[3]);

    // Test if the items are inserted.
    EXPECT_EQ(m[0], getItem(ht, 3));
    EXPECT_EQ(m[1], getItem(ht, 7));
    EXPECT_EQ(m[2], getItem(ht, 19));
    EXPECT_EQ(m[3], getItem(ht, 38239));

    destroyHashTable(ht);
}

TEST(InsertTest, InsertSameKey)
{
    HashTable* ht = createHashTable(hash, BUCKET_NUM);

    // Create list of items to be added to the hash table.
    size_t num_items = 1;
    HTItem* m[num_items];
    make_items(m, num_items);

    // Only insert one item with key=3 into the hash table.
    insertItem(ht, 3, m[0]);

    // Test if insert function return the previous value.
    EXPECT_EQ(m[0], insertItem(ht, 3, m[0]));

    destroyHashTable(ht);
}

TEST(InsertTest, InsertNull)
{
    HashTable* ht = createHashTable(hash, BUCKET_NUM);

    // Insert Null into the hash table.
    insertItem(ht, 3, NULL);

    // Check if the value is NULL after insertion
    EXPECT_EQ(NULL, getItem(ht, 3));
    // Check if the previous value is NULL.
    EXPECT_EQ(NULL, insertItem(ht, 3, NULL));

    destroyHashTable(ht);
}

TEST(InsertTest, InsertAfterRemove)
{
    HashTable* ht = createHashTable(hash, BUCKET_NUM);

    // Create list of items to be added to the hash table.
    size_t num_items = 2;
    HTItem* m[num_items];
    make_items(m, num_items);

    // Check if the initialized value is NULL.
    EXPECT_EQ(NULL, insertItem(ht, 3, m[0]));
    // Check if removed entry value is the inserted value.
    EXPECT_EQ(m[0], removeItem(ht,3));
    // Check if the previous value is been removed by insert a new value.
    EXPECT_EQ(NULL, insertItem(ht, 3, m[1]));

    destroyHashTable(ht);
    free(m[0]);
}

TEST(InsertTest, InsertAfterDelete)
{
    HashTable* ht = createHashTable(hash, BUCKET_NUM);

    // Create list of items to be added to the hash table.
    size_t num_items = 2;
    HTItem* m[num_items];
    make_items(m, num_items);

    // Check if the initialized value is NULL.
    EXPECT_EQ(NULL, insertItem(ht, 4, m[0]));
    // Delete the entry we just created with insertion.
    deleteItem(ht,4);
    // Check if the previous value is been deleted.
    // EXPECT_EQ(NULL, insertItem(ht, 4, m[0]));
    // m[0] is already been deleted, so we cannot reinserted it.
    // We can only insert a new item.
    EXPECT_EQ(NULL, insertItem(ht, 4, m[1]));
    destroyHashTable(ht);
}

TEST(InsertTest, EmptyBeforeInsert)
{
    HashTable* ht = createHashTable(hash, BUCKET_NUM);

    // Create list of items to be added to the hash table.
    size_t num_items = 1;
    HTItem* m[num_items];
    make_items(m, num_items);

    // Test if the initial value is empty before insert
    EXPECT_EQ(NULL, insertItem(ht, 3, m[0]));
    // Test if the previous value is correctly inserted.
    EXPECT_EQ(m[0], insertItem(ht, 3, m[0]));

    destroyHashTable(ht);
}

//////////////////
// Delete Tests
//////////////////

TEST(DeleteTest, SingleValidDelete)
{
    HashTable* ht = createHashTable(hash, BUCKET_NUM);

    // Create a list of items to add to hash table.
    size_t num_items = 1;
    HTItem* m[num_items];
    make_items(m, num_items);

    // Insert one item into the hash table.
    insertItem(ht, 0, m[0]);
    deleteItem(ht, 0);

    // Since the item is been deleted, the value at key should be NULL.
    EXPECT_EQ(NULL, getItem(ht, 0));

    destroyHashTable(ht);
}

TEST(DeleteTest, MultipleValidDelete)
{
    HashTable* ht = createHashTable(hash, BUCKET_NUM);

    // Create a list of items to add to hash table.
    size_t num_items = 3;
    HTItem* m[num_items];
    make_items(m, num_items);

    // Insert multiple items into the hash table.
    insertItem(ht, 0, m[0]);
    insertItem(ht, 10, m[1]);
    insertItem(ht, 20, m[2]);

    // Delete all entrys.
    deleteItem(ht, 0);
    deleteItem(ht, 10);
    deleteItem(ht, 20);

    // Check if the entries are empty.
    EXPECT_EQ(NULL, getItem(ht, 0));
    EXPECT_EQ(NULL, getItem(ht, 10));
    EXPECT_EQ(NULL, getItem(ht, 20));

    destroyHashTable(ht);
}

TEST(DeleteTest, SingleInvalidDelete)
{
    HashTable* ht = createHashTable(hash, BUCKET_NUM);

    // When the hash table is empty, the delete function should still work.
    deleteItem(ht, 0);
    EXPECT_EQ(NULL, getItem(ht, 0));

    destroyHashTable(ht);
}

TEST(DeleteTest, DeleteHead)
{
    HashTable* ht = createHashTable(hash, BUCKET_NUM);

    // Create a list of items to add to hash table.
    size_t num_items = 3;
    HTItem* m[num_items];
    make_items(m, num_items);

    // Insert multiple items into the hash table.
    insertItem(ht, 3, m[0]);
    insertItem(ht, 7, m[1]);
    insertItem(ht, 19, m[2]);

    // Delete the head entry.
    deleteItem(ht, 3);

    // Check if the head is empty.
    EXPECT_EQ(NULL, getItem(ht, 3));

    destroyHashTable(ht);
}

TEST(DeleteTest, DeleteNext)
{
    HashTable* ht = createHashTable(hash, BUCKET_NUM);

    // Create a list of items to add to hash table.
    size_t num_items = 3;
    HTItem* m[num_items];
    make_items(m, num_items);

    // Insert multiple items into the hash table.
    insertItem(ht, 3, m[0]);
    insertItem(ht, 7, m[1]);
    insertItem(ht, 19, m[2]);

    // Delete the next entry after head.
    deleteItem(ht, 7);

    // Check if the next entry is been deleted.
    EXPECT_EQ(NULL, getItem(ht, 7));

    destroyHashTable(ht);
}

TEST(DeleteTest, DeleteLastItem)
{
    HashTable* ht = createHashTable(hash, BUCKET_NUM);

    // Create a list of items to add to hash table.
    size_t num_items = 3;
    HTItem* m[num_items];
    make_items(m, num_items);

    // Insert multiple items into the hash table.
    insertItem(ht, 3, m[0]);
    insertItem(ht, 7, m[1]);
    insertItem(ht, 19, m[2]);

    // Delete the last entry.
    deleteItem(ht, 19);

    // Check if the last entry is been deleted.
    EXPECT_EQ(NULL, getItem(ht, 19));

    destroyHashTable(ht);
}

TEST(DeleteTest, DeleteNULL)
{
    HashTable* ht = createHashTable(hash, BUCKET_NUM);

    // Create a list of items to add to hash table.
    size_t num_items = 3;
    HTItem* m[num_items];
    make_items(m, num_items);

    // Insert multiple items into the hash table.
    insertItem(ht, 3, m[0]);
    insertItem(ht, 7, m[1]);
    insertItem(ht, 19, m[2]);

    // Delete a NULL key
    // deleteItem(ht, NULL);

    // Check if the items still exist.
    EXPECT_EQ(m[0], getItem(ht, 3));
    EXPECT_EQ(m[1], getItem(ht, 7));
    EXPECT_EQ(m[2], getItem(ht, 19));

    destroyHashTable(ht);
}

TEST(DeleteTest, InvalidDelete)
{
    HashTable* ht = createHashTable(hash, BUCKET_NUM);

    // Create a list of items to add to hash table.
    size_t num_items = 3;
    HTItem* m[num_items];
    make_items(m, num_items);

    // Insert multiple items into the hash table.
    insertItem(ht, 3, m[0]);
    insertItem(ht, 7, m[1]);
    insertItem(ht, 19, m[2]);

    // Delete some random keys that are not in the hash table.
    deleteItem(ht, 879);
    // deleteItem(ht, -823);

    // Check if the items still exist.
    EXPECT_EQ(m[0], getItem(ht, 3));
    EXPECT_EQ(m[1], getItem(ht, 7));
    EXPECT_EQ(m[2], getItem(ht, 19));

    destroyHashTable(ht);
}

TEST(DeleteTest, EmptyDelete)
{
    HashTable* ht = createHashTable(hash, BUCKET_NUM);

    deleteItem(ht, 343);
    // deleteItem(ht, -233);

    EXPECT_EQ(NULL, getItem(ht, 19));
    EXPECT_EQ(NULL, getItem(ht, 7));

    destroyHashTable(ht);
}

//////////////
// Get Test
//////////////

TEST(GetTest, ValidGet)
{
    HashTable* ht = createHashTable(hash, BUCKET_NUM);

    // Create a list of items to add to hash table.
    size_t num_items = 3;
    HTItem* m[num_items];
    make_items(m, num_items);

    // Insert multiple items into the hash table.
    insertItem(ht, 3, m[0]);
    insertItem(ht, 7, m[1]);
    insertItem(ht, 19, m[2]);

    // Test if getItem reads inserted items.
    EXPECT_EQ(m[0], getItem(ht, 3));
    EXPECT_EQ(m[1], getItem(ht, 7));
    EXPECT_EQ(m[2], getItem(ht, 19));

    destroyHashTable(ht);
}

TEST(GetTest, EmptyGet)
{
    HashTable* ht = createHashTable(hash, BUCKET_NUM);

    // Check if the entry is empty using getItem.
    EXPECT_EQ(NULL, getItem(ht, 19));

    destroyHashTable(ht);
}

TEST(GetTest, InvalidGet)
{
    HashTable* ht = createHashTable(hash, BUCKET_NUM);

    // Create a list of items to add to hash table.
    size_t num_items = 3;
    HTItem* m[num_items];
    make_items(m, num_items);

    // Insert multiple items into the hash table.
    insertItem(ht, 3, m[0]);
    insertItem(ht, 7, m[1]);
    insertItem(ht, 19, m[2]);

    // Check if getItem return NULL for invalid keys.
    EXPECT_EQ(NULL, getItem(ht, 1129));
    // EXPECT_EQ(NULL, getItem(ht, '122'));
    // EXPECT_EQ(NULL, getItem(ht, NULL));

    destroyHashTable(ht);
}

//////////////////////
// Combination Test
//////////////////////
TEST(CombTest, Test1)
{
    HashTable* ht = createHashTable(hash, BUCKET_NUM);

    // Create a list of items to add to hash table.
    size_t num_items = 3;
    HTItem* m[num_items];
    make_items(m, num_items);

    // Check if the initial value is empty before insert
    EXPECT_EQ(NULL,insertItem(ht, 13, m[0]));
    EXPECT_EQ(NULL,insertItem(ht, 72, m[1]));
    EXPECT_EQ(NULL,insertItem(ht, 39, m[2]));

    // Check if the previous value is correctly
    // returned after insertion.
    EXPECT_EQ(m[2],insertItem(ht, 39, m[2]));
    EXPECT_EQ(m[2],insertItem(ht, 39, m[2]));
    EXPECT_EQ(m[0],insertItem(ht, 13, m[0]));
    EXPECT_EQ(m[1],insertItem(ht, 72, m[1]));

    // Check for invalid get.
    EXPECT_EQ(NULL, getItem(ht, 1129));
    // EXPECT_EQ(NULL, getItem(ht, '1231'));
    // EXPECT_EQ(NULL, getItem(ht, NULL));

    // Check for valid get.
    EXPECT_EQ(m[0], getItem(ht, 13));
    EXPECT_EQ(m[0], getItem(ht, 13));
    EXPECT_EQ(m[1], getItem(ht, 72));

    // Remove test
    EXPECT_EQ(NULL, removeItem(ht, 3));
    EXPECT_EQ(m[0], removeItem(ht, 13));
    EXPECT_EQ(NULL, removeItem(ht, 3));
    EXPECT_EQ(NULL, removeItem(ht, 13));
    EXPECT_EQ(m[1], removeItem(ht, 72));
    EXPECT_EQ(m[2], removeItem(ht, 39));
    EXPECT_EQ(NULL, removeItem(ht, 72));

    // Check the hash table after remove.
    EXPECT_EQ(NULL, getItem(ht, 13));
    EXPECT_EQ(NULL, getItem(ht, 72));
    EXPECT_EQ(NULL, getItem(ht, 39));

    destroyHashTable(ht);
    free(m[0]);
    free(m[1]);
    free(m[2]);
}

TEST(CombTest, Test2)
{
    HashTable* ht = createHashTable(hash, BUCKET_NUM);

    // Create a list of items to add to hash table.
    size_t num_items = 3;
    HTItem* m[num_items];
    make_items(m, num_items);

    // Invalid delete.
    deleteItem(ht, 987);
    // Test if there's anything in key=987.
    EXPECT_EQ(NULL, getItem(ht, 987));
    EXPECT_EQ(NULL,insertItem(ht, 987, m[2]));

    // Valid delete.
    deleteItem(ht, 987);
    // m[2] is no longer exist.
    // Test if the entry is NULL before insert.
    EXPECT_EQ(NULL,insertItem(ht, 987, m[1]));

    destroyHashTable(ht);
    free(m[0]);
    // Since num_items = 3, and we deleted m[2] and destroyed m[1],
    // we need to free m[0].
}