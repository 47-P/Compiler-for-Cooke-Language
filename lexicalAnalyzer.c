#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 10 // Size of the hash table


// Defining the structrue for a hash node 
typedef struct HashNode 
{
    char *key;
    int value;
    struct HashNode *next; // Pointer for the next node in the hash table 
} HashNode;

// Defining the structure for the hashmap
typedef struct HashMap
{
    HashNode **table;
    
}