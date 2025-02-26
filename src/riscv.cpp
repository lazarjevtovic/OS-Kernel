//
// Created by marko on 20.4.22..
//

#include "../h/riscv.hpp"
#include "../h/tcb.hpp"
#include "../lib/console.h"
#include "../h/MemoryAllocator.hpp"
#include "../h/print.hpp"
#include "../h/syscall_c.hpp"
#include "../h/IOBuffer.hpp"

extern IOBuffer* ioBuffer;
extern TCB* displayThread;

void Riscv::popSppSpie()
{

    if(TCB::running!=displayThread){
        mc_sstatus(1<<8);
    }
    __asm__ volatile("csrw sepc, ra");
    __asm__ volatile("sret");
}


void Riscv::handleSupervisorTrap()
{


//    uint64 sp;
//    __asm__ volatile ("mv %[par], sp" : [par] "=r"(sp));
//    printInteger(sp);
//    printString("\n");

    uint64 volatile scause = r_scause();

    // interrupt: no; cause code: environment call from U-mode(8) or S-mode(9)
    if (scause == 0x0000000000000008UL || scause == 0x0000000000000009UL)
    {
        uint64 volatile sepc = r_sepc() + 4;
        uint64 volatile sstatus = r_sstatus();

        uint64 num;
        __asm__ volatile ("mv %[par], a0" : [par] "=r"(num));

        if(num == 0x01){
            __asm__ volatile ("mv %[par], a1" : [par] "=r"(num));
            void* mem = MemoryAllocator::getMem(num);
            __asm__ volatile ("mv a1, %[par]" : : [par] "r"(mem));
            returnResultA1toA0();
        }
        else if(num == 0x02){
            void* mem;
            __asm__ volatile ("mv %[par], a1" : [par] "=r"(mem));
            int ret = MemoryAllocator::deleteMem(mem);
            __asm__ volatile ("mv a1, %[par]" : : [par] "r"(ret));
            returnResultA1toA0();
        }
        else if(num == 0x11){
            thread_t*  handle;
            void(*  start_routine)(void*);
            void*  args;
            uchar*  s;
            uint64  ret;

            __asm__ volatile("ld t0 , 88(fp)");
            __asm__ volatile ("mv %[par], t0" : [par] "=r"(handle));
            __asm__ volatile("ld t0 , 96(fp)");
            __asm__ volatile ("mv %[par], t0" : [par] "=r"(start_routine));
            __asm__ volatile("ld t0 , 104(fp)");
            __asm__ volatile ("mv %[par], t0" : [par] "=r"(args));
            __asm__ volatile("ld t0 , 112(fp)");
            __asm__ volatile ("mv %[par], t0" : [par] "=r"(s));

            (*handle) =  TCB::createThread(start_routine, args, s);

            if((*handle) == nullptr){
                ret = -1;
            }
            else{
                ret = 0;
            }
            __asm__ volatile ("mv a1, %[par]" : : [par] "r"(ret));
            returnResultA1toA0();

        }
        else if(num == 0x12){
            TCB::running->setFinished(true);
            TCB::timeSliceCounter = 0;
            TCB::dispatch();
        }
        else if(num == 0x13){
            TCB::timeSliceCounter = 0;
            TCB::dispatch();
        }
        else if(num == 0x21){
            sem_t*  handle;
            unsigned int init;
            uint64  ret;

            __asm__ volatile("ld t0 , 88(fp)");
            __asm__ volatile ("mv %[par], t0" : [par] "=r"(handle));
            __asm__ volatile("ld t0 , 96(fp)");
            __asm__ volatile ("mv %[par], t0" : [par] "=r"(init));

            (*handle) =  new kSemaphore(init);

            if((*handle) == nullptr){
                ret = -1;
            }
            else{
                ret = 0;
            }
            __asm__ volatile ("mv a1, %[par]" : : [par] "r"(ret));
            returnResultA1toA0();
        }
        else if(num == 0x22){
            sem_t  handle;
            uint64  ret;

            __asm__ volatile("ld t0 , 88(fp)");
            __asm__ volatile ("mv %[par], t0" : [par] "=r"(handle));
            delete handle;
            ret = 0;
            __asm__ volatile ("mv a1, %[par]" : : [par] "r"(ret));
            returnResultA1toA0();
        }
        else if(num == 0x23){
            sem_t  handle;
            uint64  ret;

            __asm__ volatile("ld t0 , 88(fp)");
            __asm__ volatile ("mv %[par], t0" : [par] "=r"(handle));
            ret = handle->wait();
            __asm__ volatile ("mv a1, %[par]" : : [par] "r"(ret));
            returnResultA1toA0();
        }
        else if(num == 0x24){
            sem_t  handle;
            uint64  ret;

            __asm__ volatile("ld t0 , 88(fp)");
            __asm__ volatile ("mv %[par], t0" : [par] "=r"(handle));
            ret = handle->signal();
            __asm__ volatile ("mv a1, %[par]" : : [par] "r"(ret));
            returnResultA1toA0();
        }
        else if(num == 0x25){
            sem_t  id;
            time_t timeout;
            uint64  ret;

            __asm__ volatile("ld t0 , 88(fp)");
            __asm__ volatile ("mv %[par], t0" : [par] "=r"(id));
            __asm__ volatile("ld t0 , 96(fp)");
            __asm__ volatile ("mv %[par], t0" : [par] "=r"(timeout));

            ret = id->timedwait(timeout);

            __asm__ volatile ("mv a1, %[par]" : : [par] "r"(ret));
            returnResultA1toA0();
        }
        else if(num == 0x26){
            sem_t  handle;
            uint64  ret;

            __asm__ volatile("ld t0 , 88(fp)");
            __asm__ volatile ("mv %[par], t0" : [par] "=r"(handle));
            ret = handle->trywait();
            __asm__ volatile ("mv a1, %[par]" : : [par] "r"(ret));
            returnResultA1toA0();
        }
        else if(num == 0x31){
            time_t timeout;
            uint64  ret;

            __asm__ volatile("ld t0 , 88(fp)");
            __asm__ volatile ("mv %[par], t0" : [par] "=r"(timeout));

            ret = TCB::sleep(timeout);

            __asm__ volatile ("mv a1, %[par]" : : [par] "r"(ret));
            returnResultA1toA0();
        }
        else if(num == 0x41){
            uint64  ret;
//            ret = __getc();
            ret = ioBuffer->getc1();
            __asm__ volatile ("mv a1, %[par]" : : [par] "r"(ret));
            returnResultA1toA0();
        }
        else if(num == 0x42){
            char c;
            __asm__ volatile("ld t0 , 88(fp)");
            __asm__ volatile ("mv %[par], t0" : [par] "=r"(c));
//            __putc(c);
            ioBuffer->putc2(c);
        }
        else{
        }

        w_sstatus(sstatus);
        w_sepc(sepc);
    }
    else if (scause == 0x8000000000000001UL)
    {
        // interrupt: yes; cause code: supervisor software interrupt (CLINT; machine timer interrupt)
        mc_sip(SIP_SSIP);
        uint64 volatile sepc = r_sepc();
        uint64 volatile sstatus = r_sstatus();

        TCB::listTimed.dec();
        while(TCB::listTimed.peekFirst() && TCB::listTimed.peekFirst()->getSemTime() == 0){
            TCB* t = TCB::listTimed.removeFirst();
            if(t->isTimed()){
                t->setBlocked(false);
                t->setTimed(false);
                t->getSemaphore()->blocked.removeThis(t);
                t->getSemaphore()->val++;
                Scheduler::put(t);
            }
            else{
                t->setBlocked(false);
                Scheduler::put(t);
            }
        }

        TCB::timeSliceCounter++;
        if (TCB::timeSliceCounter >= TCB::running->getTimeSlice())
        {
            TCB::timeSliceCounter = 0;
            TCB::dispatch();
        }

        w_sstatus(sstatus);
        w_sepc(sepc);
    }
    else if (scause == 0x8000000000000009UL)
    {
        // interrupt: yes; cause code: supervisor external interrupt (PLIC; could be keyboard)
//        console_handler();
        uint64 volatile sepc = r_sepc();
        uint64 volatile sstatus = r_sstatus();
        int br =  plic_claim();
//        if(br == CONSOLE_IRQ){
            while(*(char*)CONSOLE_STATUS & CONSOLE_RX_STATUS_BIT){
                ioBuffer->putc1(*(char*)CONSOLE_RX_DATA);
            }
            plic_complete(br);
//        }
        w_sstatus(sstatus);
        w_sepc(sepc);
    }
    else
    {

//        printString("sepc: ");
//        printInteger(r_sepc());
//        printString("\n scause: ");
//        printInteger(r_scause());
//        printString("\n sstatus: ");
//        printInteger(r_sstatus());
//        printString("\n");
    }
}