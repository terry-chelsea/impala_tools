#ifndef _H_TRIE_TREE_H_
#define _H_TRIE_TREE_H_

#include <boost/cstdint.hpp>
#include <vector>

using namespace std;

typedef struct TrieNode {
        vector<struct TrieNode *> children;
        bool isEndOfValue;
        bool needFree;
        short len;
        uint8_t *part;
} TrieNode;

typedef struct TrieTree {
        TrieNode *root;
        int nValues;
        int maxLength;
	int inValues;
	long sumLength;
} TrieTree;

typedef void (*valueFunc)(void *obj, uint8_t* ptr, int len);
typedef void (*nodeFunc)(void *obj, TrieNode *node, int level);

#define MAX_PART_LENGTH 64

TrieTree *create_trie_tree();

void insert_value(TrieTree* tree, uint8_t *ptr, int len);

bool contains_value(TrieTree* tree, uint8_t* ptr, int len);

int getCardinality(TrieTree* tree);

void printDebugInfo(TrieTree* tree, bool withNodes = false);

void printValueInfo(TrieTree* tree);

void free_trie_tree(TrieTree* tree);

uint8_t* serialize_trie_tree(TrieTree* tree, int& length);

TrieTree* deserialize_trie_tree(uint8_t* buffer, int length);

void merge_trie_trees(const TrieTree* src, const TrieTree* dest);

#endif
