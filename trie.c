/* 
 * Copyright (C) 2018-2021 Vikrant Gajria - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license, which unfortunately won't be
 * written for another century.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: gajriavikrant@gmail.com
 */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define ALPHABET_RANGE 26 // 26 Alphabets + 4 extra characters.
#define CHAR_TO_INDEX(c) ((int)c - (int)'a') // Macro to convert character to index.

// Create a new datatype node.
typedef struct TrieNode {
    // Children is an array of pointers, hence we have O(1) access.
    struct TrieNode* children[ALPHABET_RANGE];
    // Mark the end of a word.
    bool isEnd;
} node;

// Returns a new node structure.
node* createNode () {
    node *newNode = (node *)malloc(sizeof (node));

    // Mark as not the end of the word.
    newNode->isEnd = false;
    // Set all children to null.
    for (int i = 0; i < ALPHABET_RANGE; i++)
        newNode->children[i] = NULL;

    return newNode;
}

// Insert a node into the tree.
void insert (node *trie, char *word) {
    // Return for null word.
    if (*word == '\0') return;
    // Traversing pointer.
    node *root = trie;

    do {
        // Check if alphabet was added as a child. If not, add.
        int i = CHAR_TO_INDEX(*word);
        if (root->children [i] == NULL)
            root->children [i] = createNode();
        // Go down the level of the subtrie.
        root = root->children [i];
    } while (*(++word) != '\0'); // Till the word isn't terminated.
    // Finally mark the last letter as end of word.
    root->isEnd = true;
}

// ! NOTE: I am omitting the search function.

// Returns true if the node is a leaf node.
bool isLeaf (node *t) {
    // Check if all the children are null.
    for (int i = 0; i < ALPHABET_RANGE; i++) {
        if (t->children [i] != NULL)
            return false;
    }
    return true;
}

// -----

// Recursive function to print auto-suggestions for given node.
void suggestionsRec(node *root, char* currentPrefix) {
    // Found a string in Trie with the given prefix
    if (root->isEnd) {
        printf("%s", currentPrefix);
    }

    // All children struct node pointers are NULL
    if (isLeaf(root)) return;
 
    for (int i = 0; i < ALPHABET_RANGE; i++) {
        if (root->children[i]) {
            // append current character to current prefix string.
            char *toAppend = 97 + i;
            strcat(currentPrefix, toAppend);
            // recur over the rest
            suggestionsRec(root->children[i], currentPrefix);
        }
    }
}
 
// Print suggestions for given query prefix.
int printAutoSuggestions(node* trie, char* query) {
    node* current = trie;
 
    // Check if prefix is present and find the node (of last level) with last character of given query.
    int n = strlen(query);
    for (int i = 0; i < n; i++) {
        int index = CHAR_TO_INDEX( *(query + i) );
        // No string in the Trie has this prefix
        if (!current->children[index]) return 0;
        // If string in the Trie, go down a level.
        else current = current->children[index];
    }
 
    // If prefix is present as a word.
    bool isWord = current->isEnd;
    // If prefix is last node of tree (has no children).
    bool isLast = isLeaf(current);
 
    // If prefix is present as a word, but there is no subtree below the last matching node.
    if (isWord && isLast) {
        printf("%s", query);
        return -1;
    }
 
    // If there are are nodes below last matching character.
    if (!isLast) {
        suggestionsRec(current, query);
        return 1;
    }
}
