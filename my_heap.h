#pragma once
#define MAX_CAPACITY 512
#define INT_MAX 2147483647
#define INT_MIN -2147483648
#include <iostream>
#include <string>

using namespace std;

struct memory_block {
	memory_block* right; // node to the right
	memory_block* left; // node to the left
	bool used; //if it is allocated, true // if this memory block is reserved or not
	int size; // the number of bytes reserved in Img_heap
	int starting_address; // the starting address in Img_heap
	// of the memory reserved by this block

};

class My_heap //will keep track the empty and full areas, 
{
	// memory_block [] heap;
   //bool isFul;

public:

	memory_block* firstblock_pointer; //a pointer at the first memory block begin,
	memory_block* heap_blk; //a ptr at the last memoryblock


	memory_block* bump_allocate(int num_bytes);
	void deallocate(memory_block* to_delete);
	memory_block* first_fit_allocate(int num_bytes);
	memory_block* best_fit_allocate(int num_bytes);
	memory_block* first_fit_split_allocate(int num_bytes);
	float get_fragmentation();
	void print_heap();
	My_heap();
	~My_heap();


private:
	int used_bytes; //the total number of allocated bytes in Img_heap.

};

