/*
Author: Mohammad Fanous
Date: 12/1/2024
this program is an implementation of a binary search tree with string keys
*/

#include "tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Find a node in the binary search tree
struct tree_node* find(const char* name, struct tree_node* root) {
    if (root == NULL) {
        return NULL;
    }

    int cmp = strcmp(name, root->data.name);
    if (cmp == 0) {
        return root;
    } else if (cmp < 0) {
        return find(name, root->left);
    } else {
        return find(name, root->right);
    }
}

// Insert a node into the binary search tree
struct tree_node* insert(const char* name, struct tree_node* root) {
    if (root == NULL) {
        // Create a new node
        struct tree_node* node = (struct tree_node*)malloc(sizeof(struct tree_node));
        if (!node) {
            fprintf(stderr, "Error: Memory allocation failed for tree node\n");
            return NULL;
        }

        // Copy the name into the fixed-size array
        strncpy(node->data.name, name, sizeof(node->data.name) - 1);
        node->data.name[sizeof(node->data.name) - 1] = '\0'; // Ensure null termination
        node->left = NULL;
        node->right = NULL;
        return node;
    }

    int cmp = strcmp(name, root->data.name);
    if (cmp < 0) {
        root->left = insert(name, root->left);
    } else if (cmp > 0) {
        root->right = insert(name, root->right);
    }
    return root;
}

//Free the entire binary search tree
void clear(struct tree_node* root) {
    if (root == NULL) {
        return;
    }

    clear(root->left);
    clear(root->right);
    free(root);
}

// Print the tree structure (pre-order traversal)
void print(struct tree_node* root) {
    static int depth = 0;

    if (root == NULL) {
        return;
    }

    // Print the current node (root)
    if (depth == 0) {
        printf("%s\n", root->data.name);
    }

    // Print the left subtree
    if (root->left) {
        for (int i = 0; i < depth + 1; i++) {
            printf(" ");
        }
        printf("l:%s\n", root->left->data.name);
        depth++;
        print(root->left);
        depth--;
    }

    // Print the right subtree
    if (root->right) {
        for (int i = 0; i < depth + 1; i++) {
            printf(" ");
        }
        printf("r:%s\n", root->right->data.name);
        depth++;
        print(root->right);
        depth--;
    }
}

// Print the tree in alphabetical order (in-order traversal)
void printSorted(struct tree_node* root) {
    if (root == NULL) {
        return;
    }

    printSorted(root->left);
    printf("%s\n", root->data.name);
    printSorted(root->right);
}
