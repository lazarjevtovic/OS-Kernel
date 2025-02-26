//
// Created by marko on 20.4.22..
//

#ifndef OS1_VEZBE07_RISCV_CONTEXT_SWITCH_2_INTERRUPT_TCB_HPP
#define OS1_VEZBE07_RISCV_CONTEXT_SWITCH_2_INTERRUPT_TCB_HPP

#include "../lib/hw.h"
#include "../h/MemoryAllocator.hpp"
//NE INKLUDOVATI KAD NE MORA DA SE NE BI NAPRAVILA PETLJA
void thread_dispatch();
class kSemaphore;
//extern void idle(void*);
#include "scheduler.hpp"

// Thread Control Block
class TCB
{
public:
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

    ~TCB();

//    ~TCB(){
//        MemoryAllocator::deleteMem(stack);
//    }

    bool isFinished() const { return finished; }
    bool isBlocked() const { return blocked; }
    bool isTimed() const {return timed;}
    time_t getSemTime() const {return semTime;}
    kSemaphore* getSemaphore() const{return semaphore;}

    void setFinished(bool value) { finished = value; }
    void setBlocked(bool value) {blocked = value;}
    void setTimed(bool value) {timed = value;}
    void setSemTime(time_t value) {semTime = value;}
    void setSemaphore(kSemaphore* value) {semaphore = value;}


    static List listTimed;
    static int sleep(time_t timeout);

    uint64 getTimeSlice() const { return timeSlice; }

    using Body = void (*)(void *);

    static TCB *createThread(Body body, void* args, uchar* stack);

    static void yield();

    static TCB *running;

private:
    TCB(Body body, void* args, uchar* stack, uint64 timeSlice) :
            body(body),
            args(args),
            stack(stack),
            context({(uint64) &threadWrapper,
                     stack != nullptr ? (uint64) &stack[DEFAULT_STACK_SIZE] : 0
                    }),
            timeSlice(timeSlice),
            finished(false),
            blocked(false),
            timed(false),
            semTime(0),
            semaphore(0)
    {
        if (body != nullptr) { Scheduler::put(this); }
    }

    struct Context
    {
        uint64 ra;
        uint64 sp;
    };

    Body body;
    void* args;
    uchar *stack;
    Context context;
    uint64 timeSlice;
    bool finished;
    bool blocked;
    bool timed;
    time_t semTime;
    kSemaphore* semaphore;

    friend class Riscv;
    friend class kSemaphore;
//    friend class List;

    static void threadWrapper();

    static void contextSwitch(Context *oldContext, Context *runningContext);

    static void dispatch();

    static uint64 timeSliceCounter;

};

#endif //OS1_VEZBE07_RISCV_CONTEXT_SWITCH_2_INTERRUPT_TCB_HPP
