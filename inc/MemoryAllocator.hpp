//
// Created by os on 5/3/24.
//

#ifndef PROJEKAT_MEMORYALLOCATOR_HPP
#define PROJEKAT_MEMORYALLOCATOR_HPP

#include "../lib/hw.h"

class MemoryAllocator
{
public:
    MemoryAllocator() = delete;
    static char* getMem (size_t blocks);
    static int deleteMem (void* location);
    static int getBroj(){
        return freeMem->size;
    }
    static void ispisi();
    static MemoryAllocator &allocator ;
private:
    static void initialize();
    static int initialized;
    struct FreeMem{
        FreeMem* next;
        FreeMem* prev;
        size_t size;
    };
    static FreeMem* freeMem;
    static int tryToJoin(FreeMem* cur);
};

#endif //PROJEKAT_MEMORYALLOCATOR_HPP
