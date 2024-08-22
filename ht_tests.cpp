//=================================================================
// Copyright 2022 Georgia Tech.  All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must not be made publicly available anywhere.
//=================================================================

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

	// Destroy the hash table togeter with the inserted values
	destroyHashTable(ht);
}

////////////////////////////
// Removal and delete tests
////////////////////////////
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
TEST(RemoveTest, MultipleRemove)
{
    HashTable* ht = createHashTable(hash, BUCKET_NUM);

    // Create a list of items to add to hash table.
    size_t num_items = 3;
    HTItem* m[num_items];
    make_items(m, num_items);

    // Insert items into the hash table.
    insertItem(ht, 0, m[0]);
    insertItem(ht, 1, m[1]);
    insertItem(ht, 2, m[2]);

    // Remove items one by one and check if they are removed correctly.
    void* removed1 = removeItem(ht, 0);
    void* removed2 = removeItem(ht, 1);
    void* removed3 = removeItem(ht, 2);

    // Check if the removed items match the inserted items.
    EXPECT_EQ(m[0], removed1);
    EXPECT_EQ(m[1], removed2);
    EXPECT_EQ(m[2], removed3);

    // Check if the hash table is empty after removals.
    EXPECT_EQ(NULL, getItem(ht, 0));
    EXPECT_EQ(NULL, getItem(ht, 1));
    EXPECT_EQ(NULL, getItem(ht, 2));

    destroyHashTable(ht);
    free(removed1);
    free(removed2);
    free(removed3);
}


///////////////////
// Insersion tests
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
TEST(EdgeCaseTest, FullHashTable)
{
    // Set the number of buckets to a small value for testing purposes.
    const int SMALL_BUCKET_NUM = 2;
    HashTable* ht = createHashTable(hash, SMALL_BUCKET_NUM);

    // Create more items than the number of buckets to fill the hash table.
    size_t num_items = SMALL_BUCKET_NUM + 1;
    HTItem* m[num_items];
    make_items(m, num_items);

    // Insert items into the hash table.
    for (size_t i = 0; i < num_items; ++i) {
        insertItem(ht, i, m[i]);
    }

    // Check if all items can be retrieved correctly.
    for (size_t i = 0; i < num_items; ++i) {
        EXPECT_EQ(m[i], getItem(ht, i));
    }

    destroyHashTable(ht);
    for (size_t i = 0; i < num_items; ++i) {
        free(m[i]);
    }
}
/////////////////////////////
// More Insertion tests
/////////////////////////////
TEST(InsertTest, InsertNullPointer) {
    HashTable* ht = createHashTable(hash, BUCKET_NUM);

    // Insert a null pointer with a key
    EXPECT_EQ(NULL, insertItem(ht, 1, NULL));

    // Check that the item with key 1 is still null (not inserted)
    EXPECT_EQ(NULL, getItem(ht, 1));

    destroyHashTable(ht);
}

///////////////////////////
// More Removal tests
///////////////////////////
TEST(RemoveTest, RemoveNonExistentKey) {
    HashTable* ht = createHashTable(hash, BUCKET_NUM);

    // Attempt to remove an item with a key that was never inserted
    EXPECT_EQ(NULL, removeItem(ht, 99));

    destroyHashTable(ht);
}

TEST(RemoveTest, RemoveAfterCollision) {
    HashTable* ht = createHashTable(hash, BUCKET_NUM);

    HTItem* item1 = (HTItem*)malloc(sizeof(HTItem));
    HTItem* item2 = (HTItem*)malloc(sizeof(HTItem));

    // Insert two items that will collide
    insertItem(ht, 0, item1);
    insertItem(ht, BUCKET_NUM, item2); // This should cause a collision with key 0

    // Remove the item with the original key
    EXPECT_EQ(item1, removeItem(ht, 0));

    // The item with the colliding key should still be retrievable
    EXPECT_EQ(item2, getItem(ht, BUCKET_NUM));

    free(item1);
    free(item2);
    destroyHashTable(ht);
}
///////////////////////////
// More Access tests
///////////////////////////
TEST(AccessTest, GetItemAfterRemove) {
    HashTable* ht = createHashTable(hash, BUCKET_NUM);

    HTItem* item = (HTItem*)malloc(sizeof(HTItem));
    insertItem(ht, 0, item);
    removeItem(ht, 0);

    // After removal, the item should not be retrievable
    EXPECT_EQ(NULL, getItem(ht, 0));

    free(item);
    destroyHashTable(ht);
}

TEST(AccessTest, GetItemWithCollision) {
    HashTable* ht = createHashTable(hash, BUCKET_NUM);

    HTItem* item1 = (HTItem*)malloc(sizeof(HTItem));
    HTItem* item2 = (HTItem*)malloc(sizeof(HTItem));
    insertItem(ht, 0, item1);
    insertItem(ht, BUCKET_NUM, item2); // This should cause a collision with key 0

    // Both items should be retrievable despite the collision
    EXPECT_EQ(item1, getItem(ht, 0));
    EXPECT_EQ(item2, getItem(ht, BUCKET_NUM));

    free(item1);
    free(item2);
    destroyHashTable(ht);
}

///////////////////////////
// More EdgeCase tests
///////////////////////////

TEST(EdgeCaseTest, StressTestInsertsAndRemoves) {
    const int NUM_OPERATIONS = 10000;
    HashTable* ht = createHashTable(hash, BUCKET_NUM);

    HTItem* items[NUM_OPERATIONS];

    // Insert a large number of items
    for (int i = 0; i < NUM_OPERATIONS; ++i) {
        items[i] = (HTItem*)malloc(sizeof(HTItem));
        insertItem(ht, i, items[i]);
    }

    // Remove the items
    for (int i = 0; i < NUM_OPERATIONS; ++i) {
        HTItem* removed = (HTItem*)removeItem(ht, i);
        EXPECT_EQ(items[i], removed);
        free(removed);
    }

    destroyHashTable(ht);
}






