#include <cstdint>
#include <iostream>
using namespace std;
struct BlockHeader {
    size_t size;
    bool is_free;
    BlockHeader* next;
    BlockHeader* next_free;
};

class Allocator {
    static constexpr size_t POOL_SIZE = 1024;
    static constexpr size_t ALIGNMENT = 8;
    
    alignas(ALIGNMENT) uint8_t buffer[POOL_SIZE];
  
    BlockHeader* freeList;
    size_t align(size_t size) {
        if (size % ALIGNMENT != 0) {
                return size = (size + ALIGNMENT - 1) / ALIGNMENT * ALIGNMENT;
            }
        return size;
    }
    BlockHeader* findFit(size_t size) {
        BlockHeader* current = freeList;
        BlockHeader* prev = nullptr;
        while (current != nullptr) {
            if (current->size >= size) {
                if (current == freeList) {
                    freeList = current->next_free;
                }
                else  {
                    prev->next_free = current->next_free;
                }
                return current;
            }
            else {
                prev = current;
                current = current->next_free;
            }
        }
        return nullptr;
    }
    BlockHeader* getHeader(void* ptr);
    void dump() {
        cout << "Memory Pool Dump" << endl;

        BlockHeader* current = freeList;
        int count = 0; 
        while (current != nullptr) {
            cout << "Block " << count << endl;
            cout << "Address: " << current << endl;
            cout << "Size: " << current->size << endl;
            cout << "Status: " << (current->is_free ? "FREE" : "USED") << endl;
            cout << "Next: " << current->next << endl;
            
            current = current->next;
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
            if (findFit(size) != nullptr) {
                BlockHeader* current = findFit(size);
                current->is_free = false;
            }
        }
        void free(void* ptr);
    
};