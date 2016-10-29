#include "trie.h"
#include <assert.h>

int main(int argc, char* args[]) {
	TrieTree *tree = create_trie_tree();
	static char a[] = "hello";
	static char b[] = "world";
	static char c[] = "hellowls";
	static char d[] = "helloworld";
	static char e[] = "fengyu";
	static char f[] = "hello";
	static char g[] = "hellowls";
	static char h[] = "fengxiao";
	static char i[] = "zhuxiao";
	static char j[] = "worldd";

	insert_value(tree, (uint8_t *)a, 5);
	insert_value(tree, (uint8_t *)b, 5);
	insert_value(tree, (uint8_t *)c, 8);
	insert_value(tree, (uint8_t *)d, 10);
	insert_value(tree, (uint8_t *)NULL, 0);
	insert_value(tree, (uint8_t *)e, 6);
	insert_value(tree, (uint8_t *)f, 5);
	insert_value(tree, (uint8_t *)g, 8);
	insert_value(tree, (uint8_t *)h, 8);
	insert_value(tree, (uint8_t *)i, 7);
	insert_value(tree, (uint8_t *)j, 6);

	assert(contains_value(tree, (uint8_t *)a, 5));
	assert(contains_value(tree, (uint8_t *)d, 10));
	assert(contains_value(tree, (uint8_t *)g, 8));
	assert(contains_value(tree, (uint8_t *)h, 8));
	assert(contains_value(tree, (uint8_t *)b, 5));
	assert(contains_value(tree, (uint8_t *)d, 10));
	assert(contains_value(tree, (uint8_t *)b, 5));
	
	static char na[] = "notexist";
	static char nb[] = "fengyux";
	static char nc[] = "hellowor";
	assert(!contains_value(tree, (uint8_t *)na, 8));
	assert(!contains_value(tree, (uint8_t *)nb, 8));
	assert(!contains_value(tree, (uint8_t *)nc, 8));
	
	printDebugInfo(tree);
	printValueInfo(tree);

	free_trie_tree(tree);
	return 0;
}
