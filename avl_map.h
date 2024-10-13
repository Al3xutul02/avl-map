#ifndef _AVL_MAP_H_
#define _AVL_MAP_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_KEY_LENGHT 256

// Struct definition for a node
typedef struct AVL {
    char key[MAX_KEY_LENGHT];
    struct AVL* pair;
    long long ech;
    long long height;
    struct AVL* left_child;
    struct AVL* right_child;
}node;

// Struct definition of a map
typedef struct _map {
    node* keys;
    node* values;
}map;

// Computing functions
int maxNumber(int x, int y);
int maxPathLength(node* node);
void computeBalanceFactor(node* node);

// Blance functions
node* leftRotation(node* node);
node* rightRotation(node* node);
node* doubleLeftRotation(node* node);
node* doubleRightRotation(node* node);
node* balance(node* node);

// Tree manipulation functions
node* createNode(char key[], node* pair);
node* addNode(node* root, char key[], node* pair, node** inserted_node);
node* deleteNode(node* node, char key[]);

// Tree access functions
node* findNode(node* node, char key[]);
node* getMin(node* node);
node* getMax(node* node);

// Map manipulation functions
map* createMap();
void addPair(map* root, char key[], char value[]);
void deleteKey(map* root, char key[]);
void deleteValue(map* root, char value[]);

// Map access functions
// Return string arrays with the respective key and value
char** findKey(map* root, char key[]);
char** findValue(map* root, char value[]);
char** getMinKey(map* root);
char** getMinValue(map* root);
char** getMaxKey(map* root);
char** getMaxValue(map* root);

#endif // _AVL_MAP_H_