//
// Created by os on 5/10/24.
//

#ifndef PROJEKAT_KSEMAPHORE_HPP
#define PROJEKAT_KSEMAPHORE_HPP


#include "../lib/hw.h"
#include "../h/tcb.hpp"
#include "../h/print.hpp"

//preuzeto iz knjige
class kSemaphore
{
public:
    kSemaphore(int init = 1): val(init){}
    int wait();
    int signal();
    int value() const {return val;}
    int trywait();
    int timedwait(time_t timeout);
    static void* operator new(size_t size){
        return MemoryAllocator::getMem((size+MEM_BLOCK_SIZE-1)/MEM_BLOCK_SIZE);
    }
    static void* operator new[](size_t size){
        return MemoryAllocator::getMem((size+MEM_BLOCK_SIZE-1)/MEM_BLOCK_SIZE);
    }
    static void operator delete(void* p){
        MemoryAllocator::deleteMem(p);
    }
    static void operator delete[](void* p){
        MemoryAllocator::deleteMem(p);
    }
    ~kSemaphore();
protected:
    void block();
    void unblock();
private:
    int val;
    List blocked;

    friend class Riscv;
    friend class TCB;
};


#endif //PROJEKAT_KSEMAPHORE_HPP
