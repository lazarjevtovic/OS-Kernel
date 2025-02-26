//
// Created by os on 5/16/24.
//

#include "../h/syscall_cpp.hpp"

void* operator new(size_t size){
    return mem_alloc(size);
}
void* operator new[](size_t size){
    return mem_alloc(size);
}
void operator delete(void* p){
    mem_free(p);
}
void operator delete[](void* p){
    mem_free(p);
}

Thread::Thread(void (*body)(void *), void *arg)
{
    this->body = body;
    this->arg = arg;
}

Thread::~Thread()
{
    delete myHandle;
}

int Thread::start()
{

    return thread_create(&myHandle, wrapper, this);

}

void Thread::dispatch()
{
    thread_dispatch();
}

int Thread::sleep(time_t t)
{
    return time_sleep(t);
}

Thread::Thread()
{
    body = nullptr;
    arg = nullptr;
}

void Thread::wrapper(void *p)
{
    Thread* t = (Thread*) p;
    if(t->body){
        t->body(t->arg);
    }
    else{
        t->run();
    }
}

Semaphore::Semaphore(unsigned int init)
{
    sem_open(&myHandle, init);
}

Semaphore::~Semaphore()
{
    sem_close(myHandle);
}

int Semaphore::wait()
{
    return sem_wait(myHandle);
}

int Semaphore::signal()
{
    return sem_signal(myHandle);
}

int Semaphore::timedWait(time_t t)
{
    return sem_timedwait(myHandle, t);
}

int Semaphore::tryWait()
{
    return sem_trywait(myHandle);
}

char Console::getc()
{
    return ::getc();
}

void Console::putc(char c)
{
    ::putc(c);
}

void PeriodicThread::terminate()
{
    period = 0;
}

PeriodicThread::PeriodicThread(time_t period)
{
    this->period = period;
}

void PeriodicThread::run() {

    while(true){
        if(period == 0){
            break;
        }
        periodicActivation();
        time_sleep(period);
    }

}

