//
// Created by marko on 20.4.22..
//

#include "../h/syscall_cpp.hpp"
//#include "../h/print.hpp"
//#include "../test/printing.hpp"
#include "../h/riscv.hpp"
#include "../h/MemoryAllocator.hpp"
#include "../lib/console.h"
#include "../h/IOBuffer.hpp"

void userMain();

//void workerX(void* arg)
//{
////    Semaphore* sem = (Semaphore*)arg;
////    sem_timedwait(sem, 400);
////    printInteger((uint64)TCB::listTimed.removeFirst());
////    printInteger((uint64)TCB::listTimed.removeFirst());
//
//    for (uint64 i = 0; i < 10; i++)
//    {
//        printString("A: i=");
//        printInteger(i);
//        printString("\n");
//        for (uint64 j = 0; j < 10000; j++)
//        {
//            for (uint64 k = 0; k < 30000; k++)
//            {
//                // busy wait
//            }
////            TCB::yield();
//        }
//        if(i==5){
////            delete sem;
////            sem_close(sem);
////            sem->signal();
////            printInteger(b);
////            thread_dispatch();
//            time_sleep(50);
//        }
//    }
//}

IOBuffer* ioBuffer;
TCB* displayThread;

void display(void*){
    while(1){
        while((*(char*)CONSOLE_STATUS & CONSOLE_TX_STATUS_BIT) && ioBuffer->getCount2()>0){
            *(char*)CONSOLE_TX_DATA = ioBuffer->getc2();
        }
        thread_dispatch();
    }
}

void userMainWrapper(void*){
    userMain();
};


int main()
{
    Riscv::w_stvec((uint64) &Riscv::supervisorTrap);

//    char* prvaMem = (char*)mem_alloc(64);
//    printInteger((uint64)prvaMem);
//    putc('\n');
//    mem_free(prvaMem);

    ioBuffer = (IOBuffer*)MemoryAllocator::getMem((sizeof(IOBuffer)+MEM_BLOCK_SIZE-1)/MEM_BLOCK_SIZE);

    thread_t mainThread;
    thread_create(&mainThread, nullptr, nullptr);
    TCB::running = mainThread;

    thread_create(&displayThread, display, nullptr);

//    Semaphore* sem = new Semaphore(0);
//    thread_t threadX;
//    thread_create(&threadX, workerX, sem);

    thread_t userThread;
    thread_create(&userThread, userMainWrapper, nullptr);
    Riscv::ms_sstatus(Riscv::SSTATUS_SIE);
    while(!userThread->isFinished()){
        thread_dispatch();
    }


//    printString("pred sleepom\n");
//    int a = Thread::sleep(50);
////    int a = sem_trywait(sem);
//    printString("prosao sleep sa");
//    printInteger(a);
//    printString("\n");
//    while(!threadX->isFinished())
//    {
//        thread_dispatch();
//    }
//    printString("pred deletom\n");

    while(ioBuffer->getCount2()>0){
        thread_dispatch();
    }

//    delete threadX;
    delete userThread;
    delete displayThread;
    delete ioBuffer;
    delete mainThread;

//    char* novaMem = (char*)mem_alloc(64);
//    printInteger((uint64)novaMem);
//    mem_free(novaMem);

    return 0;
}