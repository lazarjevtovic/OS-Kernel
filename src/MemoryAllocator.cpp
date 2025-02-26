//
// Created by os on 5/3/24.
//

#include "../h/MemoryAllocator.hpp"
#include "../h/print.hpp"

int MemoryAllocator::initialized = 0;
MemoryAllocator::FreeMem* MemoryAllocator::freeMem = nullptr;

void MemoryAllocator::initialize()
{
    //poravnanje
    if((uint64)HEAP_START_ADDR % MEM_BLOCK_SIZE == 0){
        MemoryAllocator::freeMem = (FreeMem*) HEAP_START_ADDR;
    }
    else{
        MemoryAllocator::freeMem = (FreeMem*) ((char*)HEAP_START_ADDR + MEM_BLOCK_SIZE - (uint64)HEAP_START_ADDR % MEM_BLOCK_SIZE);
    }
//    freeMem->size = ((char*)HEAP_END_ADDR - (char*)HEAP_START_ADDR - MEM_BLOCK_SIZE)-
//            ((char*)HEAP_END_ADDR - (char*)HEAP_START_ADDR)%MEM_BLOCK_SIZE;


    freeMem->size = ((char*)HEAP_END_ADDR - (char*)HEAP_START_ADDR - MEM_BLOCK_SIZE)-
            ((char*)HEAP_END_ADDR - (char*)freeMem)%MEM_BLOCK_SIZE;
    freeMem->next = nullptr;
    freeMem->prev = nullptr;
    initialized = 1;
}

//kod preuzet iz zadatka 1, avgust 2021
char *MemoryAllocator::getMem(size_t size)
{
    if(initialized == 0){
        initialize();
    }
    //size je u blokovima pa prebacuje u vel
    size*=MEM_BLOCK_SIZE;

    FreeMem *blk = freeMem, *prev = nullptr;
    for(; blk!=nullptr; prev = blk, blk=blk->next)
        if(blk->size>=size) break;
    if(blk == nullptr){
        //pozvati compact
        return nullptr;
    }
    size_t remainingSize = blk->size - size;
    if(remainingSize >= MEM_BLOCK_SIZE*2){
        blk->size = size;
        size_t offset = size + MEM_BLOCK_SIZE;
        FreeMem* newBlk = (FreeMem*)((char*)blk + offset);
        if(prev) prev->next = newBlk;
        else freeMem = newBlk;
        newBlk->next = blk->next;
        newBlk->size = remainingSize - MEM_BLOCK_SIZE;
    }
    else{
        if(prev) prev->next = blk->next;
        else freeMem = blk->next;
    }
    blk->next = nullptr;
    return (char*)blk + MEM_BLOCK_SIZE;
}

//kod preuzet iz zadatka 2, septembar 2015
int MemoryAllocator::tryToJoin(FreeMem* cur){
    if(!cur) return 0;
    if(cur->next && (char*)cur + cur->size + MEM_BLOCK_SIZE == (char*)cur->next){
        cur->size = cur->size + cur->next->size + MEM_BLOCK_SIZE;
        cur->next = cur->next->next;
        if(cur->next) cur->next->prev = cur;
        return 1;
    }
    else{
        return 0;
    }
}

//kod preuzet iz zadatka 2, septembar 2015
int MemoryAllocator::deleteMem(void *location)
{
//    printInteger((uint64)location);
    if(initialized == 0){
        initialize();
    }
    if(location == nullptr){
        return -1;
    }
    char* addr = (char*) location;
    FreeMem* cur = nullptr;
    if(!freeMem || addr<(char*)freeMem){
        cur = nullptr;
    }
    else{
        for(cur = freeMem; cur->next!= nullptr && addr>(char*)(cur->next); cur = cur->next);
    }

    FreeMem* newSeg = (FreeMem*)(addr - MEM_BLOCK_SIZE);
    if(cur && cur->next && addr+newSeg->size>(char*)cur->next){
        return -2;
    }
    newSeg->prev = cur;
    if(cur) newSeg->next = cur->next;
    else newSeg->next = freeMem;
    if(newSeg->next) newSeg->next->prev = newSeg;
    if(cur) cur->next = newSeg;
    else freeMem = newSeg;

    tryToJoin(newSeg);
    tryToJoin(cur);

    return 0;
}

void MemoryAllocator::ispisi()
{
//    printString("----Pocetak----\n");
//    if(initialized == 0){
//        initialize();
//    }
//    FreeMem* t= freeMem;
//    while(t){
//        printInteger((uint64) t);
//        printString(" ");
//        printInteger(t->size);
//        printString("\n");
//        t=t->next;
//    }
//    printString("-----\n");
}
