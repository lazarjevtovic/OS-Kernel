//
// Created by os on 5/4/24.
//

#include "../h/syscall_c.hpp"

void* mem_alloc (size_t size){
    size_t sz ;
    sz = (size+MEM_BLOCK_SIZE-1)/MEM_BLOCK_SIZE;

    __asm__ volatile ("mv a1, %[par]" : : [par] "r"(sz));
    __asm__ volatile ("li a0, 0x01");

//    uint64 sp;
//    __asm__ volatile ("mv %[par], sp" : [par] "=r"(sp));
//    printInteger(sp);
//    printString("\n");

    __asm__ volatile ("ecall");
    uint64 mem;
    __asm__ volatile ("mv %[par], a0" : [par] "=r"(mem));
    return (void*) mem;
}
int mem_free (void* p){
    __asm__ volatile ("mv a1, %[par]" : : [par] "r"(p));
    __asm__ volatile ("li a0, 0x02");
    __asm__ volatile ("ecall");
    uint64 ret;
    __asm__ volatile ("mv %[par], a0" : [par] "=r"(ret));
    return ret;
}




int thread_create (thread_t* handle, void(*start_routine)(void*), void* arg){
    uchar* s = start_routine != nullptr? (uchar*) mem_alloc(DEFAULT_STACK_SIZE) : nullptr;
    __asm__ volatile ("mv a4, %[par]" : : [par] "r"(s));
    __asm__ volatile ("mv a1, %[par]" : : [par] "r"(handle));
    __asm__ volatile ("mv a2, %[par]" : : [par] "r"(start_routine));
    __asm__ volatile ("mv a3, %[par]" : : [par] "r"(arg));
    __asm__ volatile ("li a0, 0x11");
    __asm__ volatile ("ecall");
    uint64 ret;
    __asm__ volatile ("mv %[par], a0" : [par] "=r"(ret));
    return ret;
}
int thread_exit (){
    __asm__ volatile ("li a0, 0x12");
    __asm__ volatile ("ecall");
    uint64 ret;
    __asm__ volatile ("mv %[par], a0" : [par] "=r"(ret));
    return ret;
}
void thread_dispatch(){
    __asm__ volatile ("li a0, 0x13");
    __asm__ volatile ("ecall");
}



int sem_open(sem_t* handle, unsigned init){
    __asm__ volatile ("mv a2, %[par]" : : [par] "r"(init));
    __asm__ volatile ("mv a1, %[par]" : : [par] "r"(handle));
    __asm__ volatile ("li a0, 0x21");
    __asm__ volatile ("ecall");
    uint64 ret;
    __asm__ volatile ("mv %[par], a0" : [par] "=r"(ret));
    return ret;
}
int sem_close(sem_t handle){
    __asm__ volatile ("mv a1, %[par]" : : [par] "r"(handle));
    __asm__ volatile ("li a0, 0x22");
    __asm__ volatile ("ecall");
    uint64 ret;
    __asm__ volatile ("mv %[par], a0" : [par] "=r"(ret));
    return ret;
}
int sem_wait(sem_t id){
    __asm__ volatile ("mv a1, %[par]" : : [par] "r"(id));
    __asm__ volatile ("li a0, 0x23");
    __asm__ volatile ("ecall");
    uint64 ret;
    __asm__ volatile ("mv %[par], a0" : [par] "=r"(ret));
    return ret;
}
int sem_signal(sem_t id){
    __asm__ volatile ("mv a1, %[par]" : : [par] "r"(id));
    __asm__ volatile ("li a0, 0x24");
    __asm__ volatile ("ecall");
    uint64 ret;
    __asm__ volatile ("mv %[par], a0" : [par] "=r"(ret));
    return ret;
}
int sem_timedwait(sem_t id, time_t timeout){
    __asm__ volatile ("mv a2, %[par]" : : [par] "r"(timeout));
    __asm__ volatile ("mv a1, %[par]" : : [par] "r"(id));
    __asm__ volatile ("li a0, 0x25");
    __asm__ volatile ("ecall");
    uint64 ret;
    __asm__ volatile ("mv %[par], a0" : [par] "=r"(ret));
    return ret;
}
int sem_trywait(sem_t id){
    __asm__ volatile ("mv a1, %[par]" : : [par] "r"(id));
    __asm__ volatile ("li a0, 0x26");
    __asm__ volatile ("ecall");
    uint64 ret;
    __asm__ volatile ("mv %[par], a0" : [par] "=r"(ret));
    return ret;
}

int time_sleep(time_t time){
    __asm__ volatile ("mv a1, %[par]" : : [par] "r"(time));
    __asm__ volatile ("li a0, 0x31");
    __asm__ volatile ("ecall");
    uint64 ret;
    __asm__ volatile ("mv %[par], a0" : [par] "=r"(ret));
    return ret;
}


char getc(){
    __asm__ volatile ("li a0, 0x41");
    __asm__ volatile ("ecall");
    uint64 ret;
    __asm__ volatile ("mv %[par], a0" : [par] "=r"(ret));
    return ret;
}
void putc(char c){
    __asm__ volatile ("mv a1, %[par]" : : [par] "r"(c));
    __asm__ volatile ("li a0, 0x42");
    __asm__ volatile ("ecall");
}
