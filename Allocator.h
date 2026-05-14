#include <cstdint>
#include <iostream>
using namespace std;
struct BlockHeader {
    size_t size;
    bool is_free;
    BlockHeader* next;
    BlockHeader* next_free;
};

/* 
    TO DO:
    Start and potentially complete free() method
    Test primitive allocator
    Ask claude what's next for a more complex allocator
*/

class Allocator {
    static constexpr size_t POOL_SIZE = 1024;
    static constexpr size_t ALIGNMENT = 8;
    
    alignas(ALIGNMENT) uint8_t buffer[POOL_SIZE]; // pool with blocks of ALIGNMENT size (8)
  
    BlockHeader* freeList;
    size_t align(size_t size) {
        size = (size + ALIGNMENT - 1) / ALIGNMENT * ALIGNMENT; // this rounds size to nearest multiple of ALIGNMENT
        return size;


    }
    BlockHeader* findFit(size_t size, BlockHeader** prevOut) {
        BlockHeader* current = freeList; // start current ptr at head of list
        BlockHeader* prev = nullptr;
        while (current != nullptr) {
            if (current->size >= size) { // if size is suitable then use block
                *prevOut = prev; // prevOut is the ptr parameter passed in, and is now assigned the prev pointer to the block we're using from the pool
                return current;
            }
            prev = current;
            current = current->next_free; // advance current and prev ptr's
        return nullptr; // return null ptr only if list is empty or no free blocks available
    }
}
    BlockHeader* getHeader(void* ptr);
    void dump() { // function to print state of each block 
        cout << "Memory Pool Dump" << endl;

        BlockHeader* current = freeList;
        int count = 0; 
        while (current != nullptr) {
            cout << "Block " << count << endl;
            cout << "Address: " << current << endl;
            cout << "Size: " << current->size << endl;
            cout << "Status: " << (current->is_free ? "FREE" : "USED") << endl;
            cout << "Next: " << current->next << endl;
            
            current = current->next; // advances to next block
            count++;
        }
        cout << "End of Memory Pool Dump" << endl;
    }
    public:
        Allocator() {
            freeList = reinterpret_cast<BlockHeader*>(buffer);
            freeList->size = POOL_SIZE - sizeof(BlockHeader);
            freeList->is_free = true;
            freeList->next_free = nullptr;
            freeList->next = nullptr;
            dump();

        }
        void* alloc(size_t size) {
            size = align(size);
            BlockHeader* prev = nullptr;
            BlockHeader* current = findFit(size, &prev); // pass a prev double ptr to use when returned via double pointers
            if (current == nullptr) { // list is empty or all blocks are used
                return nullptr; 
            }
            if (prev == nullptr) {
                freeList = current->next_free; // case where block is at the head of the list
            }
            else {
                prev->next_free = current->next_free; // case where the block is within the middle of the list
            }
            current->next_free = nullptr; // we remove the the ptr on the block we're removing 
            current->is_free = false; // block is no longer a free block
            void *ptr = current + 1; // ptr moves to next available block returning the block that "was" free
            return ptr; 
        }
        void free(void* ptr);
    
};