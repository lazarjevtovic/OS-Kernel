//
// Created by marko on 20.4.22..
//

#include "../h/tcb.hpp"
#include "../h/riscv.hpp"

#include "../h/kSemaphore.hpp"
#include "../h/syscall_c.hpp"


TCB *TCB::running = nullptr;

List TCB::listTimed;

uint64 TCB::timeSliceCounter = 0;

TCB::~TCB(){
    if(!isFinished()){
        Scheduler::readyThreadQueue.removeThis(this);
        listTimed.removeThis(this);
        if(semaphore){
            semaphore->blocked.removeThis(this);
        }
    }
    MemoryAllocator::deleteMem(stack);
}

TCB *TCB::createThread(Body body, void* args, uchar* stack)
{
    return new TCB(body, args, stack, DEFAULT_TIME_SLICE);
}

void TCB::yield()
{
    __asm__ volatile ("li a0, 0x13");
    __asm__ volatile ("ecall");
}

void TCB::dispatch()
{
    TCB *old = running;
    if (!old->isFinished() && !old->isBlocked()) { Scheduler::put(old); }
    running = Scheduler::get();
    TCB::contextSwitch(&old->context, &running->context);
}

void TCB::threadWrapper()
{
    Riscv::popSppSpie();
    running->body(running->args);
    thread_exit();
//    running->setFinished(true);
//    thread_dispatch();
}

int TCB::sleep(time_t timeout)
{
    if(timeout<=0) return -5;

    TCB::running->setSemTime(timeout);
//    TCB::running->setTimed(true);
    TCB::listTimed.addBySemTime(TCB::running);
    TCB::running->setBlocked(true);
    TCB::dispatch();
    //kada se vrati
    if(TCB::running->isBlocked()){
        TCB::running->setBlocked(false);
        return -1;
    }
    return 0;
}


