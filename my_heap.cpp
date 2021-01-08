#include "my_heap.h"


My_heap::My_heap()
{
	used_bytes = 0;
	firstblock_pointer = nullptr;
	heap_blk = nullptr;

	//empty img_heap

}

My_heap::~My_heap()
{
	int leak = 0;

	memory_block* temp;
	while (firstblock_pointer != NULL)
	{
		if (firstblock_pointer->used == true)
		{
			leak += firstblock_pointer->size;
		}

		temp = firstblock_pointer->right;
		delete firstblock_pointer;
		firstblock_pointer = temp;

		//cout << "At destruction, the heap had a memory leak of X bytes.";
	}
	cout << "At destruction, the heap had a memory leak of " << leak << "bytes.";
}

//memory_block* bump_allocate(int num_bytes)
//if (size is fine) ()
//allocate new region.a new mem_block is added at the end of my_heap
//else
//{
//  return nullptr;
//}
//
//
//if (*blk == *firstblock_pointer// list is empty!
//
//
//heap structure

memory_block* My_heap::bump_allocate(int num_bytes)
{
	//heap* helper;

	//FOR NON-IMAGINARY HEAP

	if ((heap_blk != NULL) && (heap_blk->starting_address + heap_blk->size + num_bytes > MAX_CAPACITY))//512 - used_bytes >= num_bytes) //you can add
	{
		return nullptr;
	}

		if (firstblock_pointer == nullptr) //beginning
		{

			//heap linked list construction for non-imaginary heap
			//starting address of the created memory region in Img_heap(0x0)
			memory_block* mynew = new memory_block;
			mynew->starting_address = 0, mynew->size = (num_bytes), mynew->left = nullptr, mynew->right = nullptr, mynew->used = true;
			firstblock_pointer = mynew;
			heap_blk = mynew;

			used_bytes = used_bytes + mynew->size;

			//return pointer of the allocation;
			return mynew;


		}
		else if (firstblock_pointer != nullptr) //if it is not beginning 
		{
		
			memory_block* mynew = new memory_block;
			//fillout the information
			mynew->starting_address = used_bytes; mynew->size = num_bytes; mynew->used = true; mynew->right = nullptr;

			//bind the memory_block node as a last node of the list

			mynew->left = heap_blk;
			mynew = heap_blk->right;
			heap_blk = mynew;



			//mynew.left.right = mynew;
			used_bytes = used_bytes + mynew->size;

			return mynew; //adress of the newly created memory block 

		}

	}


void My_heap::deallocate(memory_block* to_delete) //right,left,used,size,starting adress, ook for dangling pointers
{
	//int newsize = used_bytes - to_delete->size;
	//left
	//heap* helper;
	if (to_delete->left != nullptr && to_delete->left->used == false)
	{
		memory_block* other_free = to_delete->left;

		if (other_free->left == nullptr)
		{
			to_delete->left = nullptr;
			firstblock_pointer = to_delete;

		} //if other_free is at the beginning
		else {
			to_delete->left = other_free->left;
			other_free->left->right = to_delete;
		}
		//used_bytes = used_bytes - to_delete->size;
		to_delete->size = to_delete->size + other_free->size;
		to_delete->used = false;
		to_delete->starting_address -= other_free->size;
		//deletion as a result of merging
		delete other_free;
	}


	else if (to_delete->right != nullptr && to_delete->right->used == false) //merge
	{
		//eventual deletion of one node from the linked list

		memory_block* other_free = to_delete->right;
		if (other_free->right == nullptr) { to_delete->right = nullptr; heap_blk = to_delete; } //if other_free is at the end
		else
		{
			to_delete->right = other_free->right;
			other_free->right->left = to_delete;

		}

		//used_bytes = used_bytes - to_delete->size;
		to_delete->size = to_delete->size + other_free->size;
		to_delete->used = false;


		delete other_free;

	}

	else
	{
		to_delete->used = false;
		//used_bytes = used_bytes - to_delete->size;


	}

	//used_bytes = newsize;
}




memory_block* My_heap::first_fit_allocate(int num_bytes) //check this
{

	if (used_bytes + num_bytes <= MAX_CAPACITY)// continue
	{

		memory_block* ptr;
		ptr = firstblock_pointer;
		while (ptr != nullptr)
		{
			if (ptr->used == false)
			{
				if (ptr->size >= num_bytes) //if right spot is found
				{
					ptr->used = true;
					//used_bytes += num_bytes;
					return ptr;
				}
			}

			ptr = ptr->right;
		}

		//if right spot has not found

		ptr = bump_allocate(num_bytes);
		return ptr;
	}
	else
	{
		return nullptr;
	}
}

memory_block* My_heap::best_fit_allocate(int num_bytes) //20
{
	int small = INT_MAX;
	bool check = false;
	

	if (used_bytes + num_bytes <= MAX_CAPACITY)
	{
		memory_block* ptr2 = firstblock_pointer;
		while (ptr2 != nullptr)
		{
			if (ptr2->used == false)
			{
				if (ptr2->size < small && small >= num_bytes) //5 < 2000 && 2000 >= 4, 4 < 5 && 5 >= 4, 3 < 4 && 4 >= 5	
				{
					small = ptr2->size;  //finds smallest size possible in the list
					check = true;
				}
			}
			ptr2 = ptr2->right;
		}

		memory_block* ptr3 = heap_blk;

		if (check == true)
			while (ptr3 != nullptr)
			{
				if (ptr3->size == small) //ptr3 traces to find smallest sized node
				{
					ptr3->used = true;
					//used_bytes += num_bytes;
					return ptr3;
				}

				ptr3 = ptr3->left;
			}
		else {
			ptr3 = bump_allocate(num_bytes);
			return ptr3;
		}
	}

	else {
		return nullptr;
	}
}


memory_block* My_heap::first_fit_split_allocate(int num_bytes)
{
	//this algorithm begins a search from heap_begin and searches for a
	//free block with size >= num_bytes.
	bool check1 = false;

	if (used_bytes + num_bytes <= MAX_CAPACITY)
	{
		memory_block* ptr4 = firstblock_pointer;

		while (ptr4->right != nullptr)
		{
			if (ptr4->used == false)
			{
				if (ptr4->size >= num_bytes) //if right spot is found
				{
					if (ptr4->size == num_bytes)
					{
						ptr4->used = true;
						//used_bytes += num_bytes;
						return ptr4;
					}

					else //if size > num_bytes
					{
						//two blocks

						//a new free block
						memory_block* mynew = new memory_block;
						mynew->size = ptr4->size - num_bytes, mynew->used = false, mynew->starting_address = ptr4->starting_address + ptr4->size;
						mynew->left = nullptr, mynew->right = nullptr;

						//old block
						ptr4->size = num_bytes, ptr4->used = true;

						//arrange new free block's place
						mynew->right = ptr4->right;
						mynew->left = ptr4;
						ptr4->right->left = mynew;
						ptr4->right = mynew;
						check1 = true;
						//used_bytes += num_bytes;

						return ptr4;
					}
				}
			}
			ptr4 = ptr4->right;
		}
	if(check1 == false)
	{
		ptr4 = bump_allocate(num_bytes);
		return ptr4;
	}
	}

	else
	{
		return nullptr;
	}

}

float My_heap::get_fragmentation()
{

	int biggestfreeblock = INT_MIN;
	int free_memory = 0;
	int non_allocated = 512 - used_bytes; //biggest free block that has not allocated
	memory_block* temp = firstblock_pointer;

	if (temp == nullptr)
	{
		return 0;
	}

	while (temp != nullptr)
	{
		if (temp->used == false)
		{
			free_memory += temp->size;
		}
		temp = temp->right;
	}

	free_memory = non_allocated + free_memory;
	//biggest_free_block
	memory_block* temp2 = firstblock_pointer;

	while (temp2 != nullptr) //biggest free block that has allocation and used==false.
	{
		if (temp2->used == false)
		{
			if (temp2->size > biggestfreeblock)
			{
				biggestfreeblock = temp2->size;
			}
		}

		temp2 = temp2->right;
	}

	if (non_allocated > biggestfreeblock)
	{
		biggestfreeblock = non_allocated;
	}

	//fragmentation
	float fragmentation = (float)(free_memory - biggestfreeblock) / free_memory * 100;

	return fragmentation;
}

void My_heap::print_heap()
{
	int freeblock = 0;
	int count = 0; //count is wrong
	memory_block* temp = firstblock_pointer;

	if (temp != nullptr)
	{
		while (temp != nullptr)
		{
			count++; //counting the number of memory blocks for print func

			if (temp->used == false)
			{
				freeblock++;
			}
			temp = temp->right;
		}

		cout << "Maximum capacity of heap: 512B" << endl;
		cout << "Currently used memory (B):" << std::dec << used_bytes << endl;
		cout << "Total memory blocks: " << count << endl;
		cout << "Total used memory blocks: " << count - freeblock << endl;
		cout << "Total free memory blocks: " << freeblock << endl;
		cout << "Fragmentation: " << get_fragmentation() << "%" << endl;


		//Block O6\t\tUsed: O7\tSize (B): O8\tStarting Address: 0xO9\n
		temp = firstblock_pointer;
		int i = 0;
		string a;


		while (temp != nullptr)
		{
			if (temp->used == true) { a = "True"; }
			else { a = "False"; }

			cout << "Block " << std::dec << i++ << "\t\tUsed: " << a << "\tSize (B): " << std::dec << temp->size << "\tStarting Address: 0x" << std::hex << temp->starting_address << std::dec << "\n";

			temp = temp->right;
		}

		cout << "------------------------------" << endl;
		cout << "------------------------------" << endl;
	}

	else
	{
		if (temp == nullptr)
		{
			count = 0;
			freeblock = 0;

			cout << "Maximum capacity of heap: 512B" << endl;
			cout << "Currently used memory (B):" << used_bytes << endl;
			cout << "Total memory blocks: " << count << endl;
			cout << "Total used memory blocks: " << count - freeblock << endl;
			cout << "Total free memory blocks: " << freeblock << endl;
			cout << "Fragmentation: " << get_fragmentation() << "%" << endl;


			cout << "------------------------------" << endl;
			cout << "------------------------------" << endl;

		}

		//if temp == nullptr
	}
}


