#include <iostream>
#include <queue>
#include <utility>
#include <cstring>
#include "trie.h"

void prints(uint8_t *ptr, int len, string msg) {
	uint8_t arr[len + 1];
	memcpy(arr, ptr, len);
	arr[len] = '\0';
	cout << msg << "  "<<arr << endl;
}

TrieNode* new_trie_node(uint8_t *ptr, int index, int len, bool isValue, vector<TrieNode *> children) {
	TrieNode* node = new TrieNode();
	node->children = children;
	node->isEndOfValue = isValue;
	if(ptr == NULL) {
		node->part= NULL;
		node->len = 0;
	} else {
		node->part= ptr + index;
		node->len= len;
	}
	node->needFree = false;
	return node;
}

TrieNode* new_trie_node(uint8_t *ptr, int len, bool isValue, vector<TrieNode *> children) {
	TrieNode* node = new TrieNode();
	node->children = children;
	node->isEndOfValue = isValue;
	node->part = ptr;
	node->len = len;
	node->needFree = true;
}

TrieTree *create_trie_tree() {
	TrieTree *tree = new TrieTree();
	
	tree->root = new_trie_node(NULL, 0, false, vector<TrieNode *>());
	tree->nValues = 0;
	tree->maxLength = 0;
	tree->inValues = 0;
	tree->sumLength = 0;
	
	return tree;
}

uint8_t* copyStr(uint8_t* ptr, int index, int len) {
	uint8_t * newPart = new uint8_t[len];
	memcpy(newPart, ptr + index, len);
	return newPart;
}

void printFirst(TrieNode * node, string msg) {
	vector<TrieNode *> childs = node->children;
	char buf[1024] = {0};
	vector<TrieNode *>::iterator it = childs.begin();
	int i = 0;
	while(it != childs.end()) {
		buf[i ++] = (*it)->part[0];
		it ++;
	}
	buf[i] = '\0';
	cout << msg << buf << endl;
}

bool insert_value_recursion(TrieNode* node, uint8_t* ptr, int len, int start, bool insert = true) {
	int i = 0, j = start;
 	int nn = len;
	int n = node->len;
	int comp = 0;
 	for (; i < n && j < nn; i++, j++) {
 		comp = node->part[i] - ptr[j];
		if (comp != 0)
 			break;
   	}
    	if(j == nn) {
		bool isNewValue = false;
    		if (i == n) {
			isNewValue = !(node->isEndOfValue);
			node->isEndOfValue = true;
		} else if(! insert){
			isNewValue = true;
		} else {
			TrieNode* newNode = new_trie_node(node->part, i, (n - i), node->isEndOfValue, node->children);
			node->len = i;
			node->isEndOfValue = true;
			node->children = vector<TrieNode*>();
			node->children.push_back(newNode);
			isNewValue = true;
		}
		return isNewValue;
    	}
    	if (i < n) {
		if(! insert)
			return true;
		
    		TrieNode* n1 = new_trie_node(node->part, i, (n - i), node->isEndOfValue, node->children);
    		node->len = i;
    		node->isEndOfValue = false;
		node->children = vector<TrieNode*>();
    	
    		int newLen = nn - j;
    		uint8_t* newPtr = copyStr(ptr, j, newLen);
    		TrieNode* n2 = new_trie_node(newPtr, newLen, true, vector<TrieNode *>());
    	
		if (comp < 0) {
  			node->children.push_back(n1);
            		node->children.push_back(n2);
     		} else {
            		node->children.push_back(n2);
            		node->children.push_back(n1);
      		}
        	return true;
    	}
   
   	uint8_t lookfor = ptr[j];
    	int lo = 0;
    	int hi = node->children.size() - 1;
    	int mid = 0;
    	bool found = false;
    	comp = 0;
  	
    	while(!found && lo <= hi) {
   		mid = lo + (hi - lo) / 2;
		
     		comp = lookfor - (node->children)[mid]->part[0];
        	if (comp < 0)
        		hi = mid - 1;
        	else if (comp > 0)
        		lo = mid + 1;
        	else
        		found = true;
	}
	bool isNewValue = false;
        if (found) {
      		isNewValue = insert_value_recursion((node->children)[mid], ptr, len, j, insert);
      	} else if(! insert){
		isNewValue = true;
	} else {
            	TrieNode* n = new_trie_node(copyStr(ptr, j, nn -j), nn - j, true, vector<TrieNode *>());
      		vector<TrieNode *>::iterator it =  node->children.begin() + (comp <= 0 ? mid : mid + 1);
       		node->children.insert(it, n);
		isNewValue = true;
       	}
	return isNewValue;
}

void insert_value(TrieTree* tree, uint8_t *ptr, int len) {
	TrieNode *root = tree->root;
	bool isNewValue = insert_value_recursion(root, ptr, len, 0);
	if(isNewValue) {
		tree->nValues ++;
	}
	if(tree->maxLength < len) {
		tree->maxLength = len;
	}
	tree->inValues ++;
	tree->maxLength += len;
}

bool contains_value(TrieTree* tree, uint8_t* ptr, int len) {
	return !insert_value_recursion(tree->root, ptr, len, 0, false);
}

void traverse_trie_tree_recursion(TrieNode* node, valueFunc f, void *param, uint8_t* value, int index) {
	vector<TrieNode*> children = node->children;
	vector<TrieNode*>::iterator it = children.begin();
	while(it != children.end()) {
		memcpy(value + index, (*it)->part, (*it)->len);
		int nextIndex = index + (*it)->len;
		if((*it)->isEndOfValue) {
			f(param, value, nextIndex);
		}
		traverse_trie_tree_recursion(*it, f, param, value, nextIndex);
		it ++;
	}
}

void traverse_trie_tree_values(TrieTree *tree, valueFunc f, void *param) {
	if(tree == NULL || tree->root == NULL)
		return;
	uint8_t* value = new uint8_t[tree->maxLength + 1];
	int len = 0;
	if(tree->root->isEndOfValue) {
		f(param, NULL, 0);
	}
	traverse_trie_tree_recursion(tree->root, f, param, value, 0);
	delete[] value;
}

void traverse_trie_tree_nodes(TrieTree *tree, nodeFunc f, void *param) {
	if(tree == NULL || tree->root == NULL)
		return;
	TrieNode* root = tree->root;
	queue<pair<TrieNode*, int> > queue;
	queue.push(make_pair(root, 1));
	while(!queue.empty()) {
		pair<TrieNode*, int> cur = queue.front();
		TrieNode* node = cur.first;
		int level = cur.second;
		vector<TrieNode*> children = node->children;
		vector<TrieNode*> ::iterator it = children.begin();
		while(it != children.end()){
			queue.push(make_pair(*it, level + 1));
			++ it;
		}
		queue.pop();
		f(param, node, level);
	}
}

void printValue(void *obj, uint8_t* ptr, int len) {
	if(ptr == NULL || len == 0) {
		cout << "" << endl;
		return ;
	}
	ptr[len] = '\0';
	cout << ptr << endl;
}

void printNode(void *obj, TrieNode* node, int level) {
	cout << "Level : " << level << ", need free :" << node->needFree << ", is value : " << node->isEndOfValue << endl;
	prints(node->part, node->len, "Node value :");
}

void printDebugInfo(TrieTree* tree, bool withNodes) {
	cout << "================Debug Infos=====================" << endl;
	cout << "Tree values number : " << tree->nValues << ", max value length : " << tree->maxLength 
		<< ", input values : " << tree->inValues << ", sum length : " << tree->maxLength << endl;
	if(withNodes)
		traverse_trie_tree_nodes(tree, printNode, NULL);
	cout << "================================================" << endl;
}

void printValueInfo(TrieTree* tree) {
	traverse_trie_tree_values(tree, printValue, NULL);
}

void freeNode(void *obj, TrieNode* node, int level) {
	if(node->needFree) 
		delete[] node->part;
	delete node;
}
void free_trie_tree(TrieTree* tree) {
	traverse_trie_tree_nodes(tree, freeNode, NULL);
}

int getCardinality(TrieTree* tree) {
	if(tree == NULL)
		return 0;
	else 
		return tree->nValues;
}

uint8_t* serialize_trie_tree(TrieTree* tree, int& length) {
	return NULL;	
}

TrieTree* deserialize_trie_tree(uint8_t* buffer, int length) {
	return NULL;
}

void merge_trie_trees(const TrieTree* src, const TrieTree* dest) {
}

