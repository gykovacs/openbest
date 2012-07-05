#ifndef _PRIMARY_KEY_TABLE
#define _PRIMARY_KEY_TABLE

#include <stdio.h>
#include <stdlib.h>

/**
 * represents a binary search tree, mapping values to keys
 */ 
typedef struct valuetokeytree
{
  struct valuetokeytree *left, *right;
  int value;
  char** keys;
  int n;
} valueToKeyTree;

/**
 * represents a binary search tree, mapping keys to values
 */ 
typedef struct keytovaluetree
{
  struct keytovaluetree* left, *right;
  int value;
  char* key;
} keyToValueTree;

/**
 * represents a primary key table, based on two binary search trees, mapping values to keys and keys to values
 */ 
typedef struct primarykeytable
{
  valueToKeyTree* vtk;
  keyToValueTree* ktv;
} primaryKeyTable;

/**
 * creates new value-to-key tree element
 * @param key key of the new element
 * @param value value of the new element
 * @returns returns the pointer of the new element
 */ 
valueToKeyTree* newVTKElement(char* key, int value);

/**
 * inserts a new element into an existing value-to-key tree
 * @param root pointer to the root pointer of the tree
 * @param key key of the new element
 * @param value value of the new element
 * @returns the pointer to the memory location, where the key is copied
 */ 
char* insertIntoVTK(valueToKeyTree** root, char* key, int value);

/**
 * creates new key-to-value tree element
 * @param key key of the new element
 * @param value value of the new element
 * @returns returns the pointer of the new element
 */ 
keyToValueTree* newKTVElement(char* key, int value);

/**
 * inserts a new element into an existing key-to-value tree
 * @param root pointer to the root pointer of the tree
 * @param key key of the new element
 * @param value value of the new element
 */ 
void insertIntoKTV(keyToValueTree** root, char* key, int value);

/**
 * looks up and returns the key-array of the value in the value-to-key tree
 * @param root pointer of the root element
 * @param value value to look up
 * @returns returns the array of keys belonging to the value
 */ 
char** lookupKeysVTK(valueToKeyTree* root, int value);

/**
 * looks up and returns the key-array of the value in the primary key tree
 * @param pkt primary key tree pointer
 * @param value value to look up
 * @returns returns the array of keys belonging to the value
 */ 
char** lookupKeysPKT(primaryKeyTable* pkt, int value);

/**
 * looks up and returns the value belonging to a given key in a key-to-value tree
 * @param root pointer of the root element
 * @param key key to look up
 * @returns returns the value of the key
 */ 
int lookupValueKTV(keyToValueTree* root, char* key);

/**
 * looks up and returns the value of the key in the primary key tree
 * @param pkt primary key tree pointer
 * @param key key to look up
 * @returns the value of the key
 */
int lookupValuePKT(primaryKeyTable* pkt, char* key);

/**
 * creates and returns a new primary key table
 * @returns primary key table
 */ 
primaryKeyTable createPrimaryKeyTableN();

/**
 * creates and returns the pointer of a new primary key table
 * @returns primary key table pointer
 */ 
primaryKeyTable* createPrimaryKeyTableP();

/**
 * insert a key-value pair into the primary key table
 * @param pkt primary key table to insert into
 * @param key key to insert
 * @param value value to insert
 */ 
void insertIntoPKT(primaryKeyTable* pkt, char* key, int value);

/**
 * destroys the value-to-key tree
 * @param root pointer of the value-to-key to destroy
 */ 
void destroyVTK(valueToKeyTree** root);

/**
 * destroys the key-to-value tree
 * @param root pointer of the key-to-value to destroy
 */ 
void destroyKTV(keyToValueTree** root);

/**
 * destroys the primary key table
 * @param pkt pointer of the primary key table to destroy
 */ 
void destroyPKT(primaryKeyTable* pkt);

/**
 * prints the primary key table by value to stdout
 * @param pkt primary key table to print
 */ 
void displayPKTbyValue(primaryKeyTable* pkt);

/**
 * prints the primary key table by keys to stdout
 * @param pkt primary key table to print
 */ 
void displayPKTbyKeys(primaryKeyTable* pkt);

/**
 * prints the value-to-key tree to stdout
 * @param root root pointer of the value-to-key tree to print out
 */ 
void displayVTK(valueToKeyTree* root);

/**
 * prints the key-to-value tree to stdout
 * @param root root pointer of the value-to-key tree to print out
 */ 
void displayKTV(keyToValueTree* root);

#endif