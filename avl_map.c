#include "avl_map.h"

int maxNumber(int x, int y) {return (x > y ? x : y);}

// Get the height of a node
int getHeight(node* node) {return (node == NULL) ? -1 : node->height;}  // If node is NULL, height is -1

// Update the height of a node
void updateHeight(node* node) {
    if (node != NULL)
        node->height = 1 + maxNumber(getHeight(node->left_child), getHeight(node->right_child));
}

// Function for the balance factor, just a difference between child path lenghts
void computeBalanceFactor(node* node) {
    if (node != NULL)
        node->ech = getHeight(node->right_child) - getHeight(node->left_child);
}

// Left rotation of nodes
node* leftRotation(node* moved_node) {
    node* anchor = moved_node->right_child;

    // Rotate the node
    moved_node->right_child = anchor->left_child;
    anchor->left_child = moved_node;

    // Recompute heights for moved_node and anchor
    updateHeight(moved_node);
    updateHeight(anchor);

    // Recompute balance factors for moved_node and anchor
    computeBalanceFactor(moved_node);
    computeBalanceFactor(anchor);
    
    // Return the new current node
    moved_node = anchor;
    return moved_node;
}

// Right rotation of nodes
node* rightRotation(node* moved_node) {
    node* anchor = moved_node->left_child;

    // Rotate the node
    moved_node->left_child = anchor->right_child;
    anchor->right_child = moved_node;

    // Recompute heights for moved_node and anchor
    updateHeight(moved_node);
    updateHeight(anchor);

    // Recompute balance factors for moved_node and anchor
    computeBalanceFactor(moved_node);
    computeBalanceFactor(anchor);
    
    // Return the new current node
    moved_node = anchor;
    return moved_node;
}

// Double left rotation of nodes
node* doubleLeftRotation(node* moved_node) {
    moved_node->right_child = rightRotation(moved_node->right_child);
    moved_node = leftRotation(moved_node);
    return moved_node;
}

// Double right rotation of nodes
node* doubleRightRotation(node* moved_node) {
    moved_node->left_child = leftRotation(moved_node->left_child);
    moved_node = rightRotation(moved_node);
    return moved_node;
}

// Tree balancing
node* balance(node* balanced_node) {
    computeBalanceFactor(balanced_node);

    if (balanced_node->ech == -2) { // Left-leaning
        node* heavy_child = balanced_node->left_child;

        // Decide the type of rotation
        if (heavy_child->ech == 1)
            return doubleRightRotation(balanced_node);
        else
            return rightRotation(balanced_node);
    }
    
    if (balanced_node->ech == 2) { // Right-leaning
        node* heavy_child = balanced_node->right_child;

        // Decide the type of rotation
        if (heavy_child->ech == -1)
            return doubleLeftRotation(balanced_node);
        else
            return leftRotation(balanced_node);
    }

    return balanced_node;
}

// Function for creating a new node
node* createNode(char key[], node* pair) {
    // Allocate memory and create the node
    node* newNode = (node*)malloc(sizeof(node));
    if (newNode == NULL) {
        printf("Memory allocation failed for new node.\n");
        return NULL;
    }

    strncpy(newNode->key, key, sizeof(newNode->key) - 1);
    newNode->key[sizeof(newNode->key) - 1] = '\0';  // Ensure null-termination

    newNode->pair = pair;
    newNode->ech = 0;
    newNode->height = 0;
    newNode->left_child = NULL;
    newNode->right_child = NULL;

    return newNode;
}

// Function adding a value to an AVL
node* addNode(node* root, char key[], node* pair, node** inserted_node) {
    // If null return the node
    if (root == NULL) {
        *inserted_node = createNode(key, pair);
        return *inserted_node;
    }

    // Parse the bst until the node's place is found
    if (strcmp(key, root->key) < 0)
        root->left_child = addNode(root->left_child, key, pair, inserted_node);
    else if (strcmp(key, root->key) > 0)
        root->right_child = addNode(root->right_child, key, pair, inserted_node);
    else {
        printf("The key %s already exists! \n", key);
        *inserted_node = root;
        return root;
    }

    root = balance(root);
    updateHeight(root);
    return root;
}

// Function for extracting one of the values in the node
node* deleteNode(node* root, char key[]) {
    if (root == NULL) {
        printf("Can't delete key %s, it is not in AVL tree!\n", key);
        return root;
    }
    
    // Navigate the tree to find the node to delete
    if (strcmp(key, root->key) < 0) {
        root->left_child = deleteNode(root->left_child, key);
    } else if (strcmp(key, root->key) > 0) {
        root->right_child = deleteNode(root->right_child, key);
    } else {
        // Node found
        node* temp;

        // Case 1: Node has one child or no child
        if (root->left_child == NULL) {
            temp = root->right_child;
            free(root);  // Free the current node
            return temp; // Return the non-null child or NULL
        } else if (root->right_child == NULL) {
            temp = root->left_child;
            free(root);  // Free the current node
            return temp; // Return the non-null child or NULL
        }

        // Case 2: Node has two children
        // Get the inorder successor (smallest in the right subtree)
        temp = root->right_child;
        while (temp->left_child != NULL) {
            temp = temp->left_child;
        }

        // Copy the inorder successor's data to this node
        strcpy(root->key, temp->key);

        // Delete the inorder successor
        root->right_child = deleteNode(root->right_child, temp->key);
        root = balance(root);
        updateHeight(root);
    }

    // Balance the tree after deletion
    return balance(root);
}

// Function for finding a node in an AVL
node* findNode(node* root, char key[]) {
    if (root == NULL) {
        printf("Can't find key %s, it is not in AVL tree!\n", key);
        return root;
    }

    // Parse through the tree until you find the key
    node* current_node = root;
    while (current_node != NULL) {
        int cmp = strcmp(key, current_node->key);
        if (cmp == 0)
            return current_node;
        else if (cmp < 0)
            current_node = current_node->left_child;
        else
            current_node = current_node->right_child;
    }

    // Check for null pointer
    if (current_node == NULL)
        printf("Null pointer encountered, key %s not found!\n", key);

    return NULL;
}

// Get the minimum key from the tree
node* getMin(node* root) {
    if (root == NULL) {
        printf("Can't find the minimum, the AVL tree is empty!\n");
        return root;
    }

    // Parse to the left until you can't
    node* current_node = root;
    while (current_node->left_child != NULL)
        current_node = current_node->left_child;
    
    return current_node;
}

// Get the maximum key from the tree
node* getMax(node* root) {
    if (root == NULL) {
        printf("Can't find the maximum, the AVL tree is empty!\n");
        return root;
    }

    // Parse to the left until you can't
    node* current_node = root;
    while (current_node->right_child != NULL)
        current_node = current_node->right_child;
    
    return current_node;
}

// Function for initialising a blank map
map* createMap() {
    // Initialise components
    map* newMap = (map*)malloc(sizeof(map));
    if (newMap == NULL) {
        printf("Failed to allocate memory for map.\n");
        return NULL;
    }

    newMap->keys = NULL;
    newMap->values = NULL;
    return newMap;
}

// Function for adding a pair to the map
void addPair(map* root, char key[], char value[]) {
    node* key_node = NULL;
    node* value_node = NULL;

    // Create the nodes and locate them
    root->keys = addNode(root->keys, key, NULL, &key_node);
    root->values = addNode(root->values, value, NULL, &value_node);
    
    // Check if the nodes were created successfully
    if (root->keys == NULL || root->values == NULL) {
        printf("Error: Failed to add nodes.\n");
        return;
    }
    
    // Assign the nodes as pairs
    key_node->pair = value_node;
    value_node->pair = key_node;
}

// Function for deleting a pair based on a key
void deleteKey(map* root, char key[]) {
    // Check for null map
    if (root == NULL) {
        printf("The map root is NULL!\n");
        return;
    }

    // Get the nodes that need to be deleted
    node* key_node = findNode(root->keys, key);
    
    // Check if key_node exists and its pair exist
    if (key_node == NULL || key_node->pair == NULL) {
        printf("Key %s not found in map!\n", key);
        return;
    }

    // Delete the nodes from each AVL
    root->keys = deleteNode(root->keys, key_node->key);
    root->values = deleteNode(root->values, key_node->pair->key);
}

// Function for deleting a pair based on a value
void deleteValue(map* root, char value[]) {
    // Check for null map
    if (root == NULL) {
        printf("The map root is NULL!\n");
        return;
    }

    // Get the nodes that need to be deleted
    node* value_node = findNode(root->values, value);
    // Check if value_node exists and its pair exist
    if (value_node == NULL || value_node->pair == NULL) {
        printf("No corresponding value for key %s!\n", value);
        return;
    }

    // Delete the nodes from each AVL
    root->keys = deleteNode(root->keys, value_node->pair->key);
    root->values = deleteNode(root->values, value_node->key);
}

// Function for finding a pair based on a key
char** findKey(map* root, char key[]) {
    // Check for null map
    if (root == NULL) {
        printf("The map root is NULL!\n");
        return NULL;
    }

    // Get the key node and copy the pair
    node* key_node = findNode(root->keys, key);
    if (key_node == NULL || key_node->pair == NULL) {
        return NULL;
    }

    char** found_pair = (char**)malloc(2 * sizeof(char*));
    found_pair[0] = key_node->key;
    found_pair[1] = key_node->pair->key;

    return found_pair;
}

// Function for finding a pair based on a value
char** findValue(map* root, char value[]) {
    // Check for null map
    if (root == NULL) {
        printf("The map root is NULL!\n");
        return NULL;
    }

    // Get the key node and copy the pair
    node* value_node = findNode(root->values, value);
    if (value_node == NULL || value_node->pair == NULL) {
        return NULL;
    }

    char** found_pair = (char**)malloc(2 * sizeof(char*));
    found_pair[0] = value_node->key;
    found_pair[1] = value_node->pair->key;

    return found_pair;
}

// Function for getting the finding the pair with the minimum key
char** getMinKey(map* root) {
    // Check for null map
    if (root == NULL) {
        printf("The map root is NULL!\n");
        return NULL;
    }
    
    // Get the key node and copy the pair
    node* min_node = getMin(root->keys);
    if (min_node == NULL || min_node->pair == NULL) {
        return NULL;
    }

    char** found_pair = (char**)malloc(2 * sizeof(char*));
    found_pair[0] = min_node->key;
    found_pair[1] = min_node->pair->key;

    return found_pair;
}

// Function for getting the finding the pair with the minimum value
char** getMinValue(map* root) {
    // Check for null map
    if (root == NULL) {
        printf("The map root is NULL!\n");
        return NULL;
    }

    // Get the key node and copy the pair
    node* min_node = getMin(root->values);
    if (min_node == NULL || min_node->pair == NULL) {
        return NULL;
    }

    char** found_pair = (char**)malloc(2 * sizeof(char*));
    found_pair[0] = min_node->pair->key;
    found_pair[1] = min_node->key;

    return found_pair;
}

// Function for getting the finding the pair with the maximum key
char** getMaxKey(map* root) {
    // Check for null map
    if (root == NULL) {
        printf("The map root is NULL!\n");
        return NULL;
    }

    // Get the key node and copy the pair
    node* max_node = getMax(root->keys);
    if (max_node == NULL || max_node->pair == NULL) {
        return NULL;
    }

    char** found_pair = (char**)malloc(2 * sizeof(char*));
    found_pair[0] = max_node->key;
    found_pair[1] = max_node->pair->key;

    return found_pair;
}

// Function for getting the finding the pair with the maximum value
char** getMaxValue(map* root) {
    // Check for null map
    if (root == NULL) {
        printf("The map root is NULL!\n");
        return NULL;
    }

    // Get the key node and copy the pair
    node* max_node = getMax(root->values);
    if (max_node == NULL || max_node->pair == NULL) {
        return NULL;
    }

    char** found_pair = (char**)malloc(2 * sizeof(char*));
    found_pair[0] = max_node->pair->key;
    found_pair[1] = max_node->key;
    strcpy(found_pair[0], max_node->pair->key);
    strcpy(found_pair[1], max_node->key);

    return found_pair;
}