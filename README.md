## dsa-projekt1

This is a project for 2st semester class "Data Structures and Algorithms". Projects is about working with dynamic memory allocation.

List of functions that need to be made:

**memory_init() (initialization of memory):**
 - [x] redirect global pointer to array representing memory
 - [x] allocate head and foot of memory
 - [x] allocate pointers to previous and next free blocks of memory
 
 **memory_alloc() (allocation of block of memory):**
 - [x] best fit: find a free block of memory with size closest to size of allocated block
 - [x] allocate head and foot of memory block
 - [x] return pointer to allocated block of memory
 - [ ] error handling
 
 **memory_free() (deallocation of already allocated block of memory):**
 - [ ] validate pointer
 - [ ] free block of memory
 - [ ] add neighboring blocks of free memory together
 - [ ] alter pointers pointing to previous and next blocks of free memory
 - [ ] error handling
 
 **memory_check() (validation of pointer pointing to allocated block of memory):**
 - [x] check validity of pointer pointing to allocated block of memory
 - [ ] testing
 
 ## testing
 - [ ] blocks of same size (8 - 24) in memory with size 50, 100 or 200 bytes
 - [ ] blocks of different size (8 - 24) in memory with size 50, 100 or 200 bytes
 - [ ] blocks of different size (500 - 5 000) in memory with size at least 1 000 bytes
 - [ ] blocks of different size (8 - 50 000) in memory with size at least 1 000 bytes
