//
// Created by os on 5/4/24.
//

#ifndef PROJEKAT_SYSCALL_C_HPP
#define PROJEKAT_SYSCALL_C_HPP

#include "../lib/hw.h"
#include "../h/tcb.hpp"
#include "../h/kSemaphore.hpp"

void* mem_alloc (size_t size);
int mem_free (void*);

typedef TCB _thread;
typedef _thread* thread_t;

int thread_create (thread_t* handle, void(*start_routine)(void*), void* arg);
int thread_exit ();
void thread_dispatch();

typedef kSemaphore _sem;
typedef _sem* sem_t;

int sem_open(sem_t* handle, unsigned init);
int sem_close(sem_t handle);
int sem_wait(sem_t id);
int sem_signal(sem_t id);
int sem_timedwait(sem_t id, time_t timeout);
int sem_trywait(sem_t id);

int time_sleep(time_t);

const int EOF = -1;
char getc();
void putc(char);


#endif //PROJEKAT_SYSCALL_C_HPP
