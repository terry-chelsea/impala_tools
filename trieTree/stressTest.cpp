#include "trie.h"
#include <cstdlib>
#include <set>
#include <string>
#include <cstring>
#include <assert.h>
#include <iostream>

using namespace std;

static uint8_t alphas[] = "abcdefghijklmnopqrstuvwxyz1234567890";
static const int LENGTH = sizeof(alphas) - 1;
static uint8_t buffer[1024];

int randomString() {
	memset(buffer, 0, sizeof(buffer));
	int len = rand() % sizeof(buffer);
	for(int i = 0 ; i < len ; ++ i) {
		buffer[i] = alphas[rand() % LENGTH];
	}

	return len;
}


void test(int count) {
	set<string> values;
	TrieTree* tree = create_trie_tree();
	for(int i = 0; i < count; ++ i) {
		int len = randomString();
	
		const char* tmp = (const char*) buffer;
		string str(tmp, len);
		insert_value(tree, buffer, len);
		values.insert(str);
		assert(contains_value(tree, buffer, len));
	}
//	printValueInfo(tree);
//	printDebugInfo(tree);

        set<string>::iterator ita = values.begin();
	while(ita != values.end()) {
//		cerr << (*ita) << endl;
		++ ita;
	}

	cout << "distinct values : " << getCardinality(tree) << endl;
	
	assert(values.size() == getCardinality(tree));
	set<string>::iterator it = values.begin();
	int i = 0;
	while(it != values.end()) {
		string value = *it;
		int size = value.length();
		char* ptr = new char[size];
		memcpy(ptr, value.c_str(), size);
		bool exist = contains_value(tree, (uint8_t *)ptr, size);
		if(! exist) {
			cout << "not exist value " << value<< endl;
		}
		assert(exist);
		delete[] ptr;
		++ it;
++ i;
	}
	
	free_trie_tree(tree);	
	cout << "Test finish with success !" << endl;
}

int main(int args, char* argv[]) {
	srand(time(NULL));
	int count = 10;
	if(args > 1){
		count = atoi(argv[1]);
	} 
	cout << "Test with round " << count << endl;

	test(count);
	return 0;
}
