//
// Created by os on 5/10/24.
//

#include "../h/kSemaphore.hpp"

void kSemaphore::block()
{
    blocked.addLast(TCB::running);
    TCB::running->setBlocked(true);
    TCB::running->setSemaphore(this);
    TCB::dispatch();
}

void kSemaphore::unblock()
{
    TCB* t = blocked.removeFirst();
    t->setBlocked(false);
    t->setSemaphore(0);
    if(t->isTimed()){
        t->setSemTime(0);
        TCB::listTimed.removeThis(t);
    }
    Scheduler::put(t);
}

int kSemaphore::wait()
{
    if(--val<0) block();
    //nakon dolaska
    if(TCB::running->isBlocked()){
        //ako je doslo zbog oslobadjanja semafora
        TCB::running->setBlocked(false);
        TCB::running->setSemaphore(0);
        return -1;
    }
    else{
        return 0;
    }
}

int kSemaphore::signal()
{
    if(++val<=0) unblock();
    return 0;
}

kSemaphore::~kSemaphore()
{
    TCB* t=blocked.removeFirst();
    while(t){
        Scheduler::put(t);
        t->setSemaphore(0);
//        t->setBlocked(false);
        if(t->isTimed()){
            TCB::listTimed.removeThis(t);
            t->setTimed(false);
            TCB::running->setSemTime(0);
        }
        t=blocked.removeFirst();
    }
}

int kSemaphore::timedwait(time_t timeout)
{
    if(timeout<=0) return -5;

    if(--val<0){
        TCB::running->setSemTime(timeout);
        TCB::running->setTimed(true);
        TCB::listTimed.addBySemTime(TCB::running);
        block();
        //kada se vrati
        if(TCB::running->isBlocked()){
            //nakon delete
            TCB::running->setBlocked(false);
//            TCB::running->setSemaphore(nullptr);
            return -1;
        }
        else if(TCB::running->isTimed()){
            //nakon signal
            TCB::running->setTimed(false);
            return 0;
        }
        else{
            //nakon timera
            return -2;
        }
    }
    return 0;
}

int kSemaphore::trywait()
{
    //TODO kad provalim sta treba da radi, menja se i signal!!
    if(val<=0){
        return 0;
    }
    if(wait()==-1){
        return -1;
    }
    return 1;
}

