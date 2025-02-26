//
// Created by marko on 20.4.22..
//

#include "../h/scheduler.hpp"

extern TCB* idleThread;

List Scheduler::readyThreadQueue;

TCB *Scheduler::get()
{
    TCB* t = readyThreadQueue.removeFirst();
    return t;
}

void Scheduler::put(TCB *ccb)
{
    readyThreadQueue.addLast(ccb);
}