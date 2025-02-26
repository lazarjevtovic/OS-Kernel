//
// Created by marko on 20.4.22..
//

#ifndef OS1_VEZBE07_RISCV_CONTEXT_SWITCH_2_INTERRUPT_LIST_HPP
#define OS1_VEZBE07_RISCV_CONTEXT_SWITCH_2_INTERRUPT_LIST_HPP

#include "../h/MemoryAllocator.hpp"
class TCB;

class List
{
protected:
    class Elem
    {
    public:
        Elem(TCB *data, Elem *next) : data(data), next(next) {}

        TCB *data;
        Elem *next;

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
    };

    Elem *head, *tail;

public:
    List() : head(0), tail(0) {}

    List(const List &) = delete;

    List &operator=(const List &) = delete;

    void addFirst(TCB *data)
    {
        Elem *elem = new Elem(data, head);
        head = elem;
        if (!tail) { tail = head; }
    }

    void addLast(TCB *data)
    {
        Elem *elem = new Elem(data, 0);
        if (tail)
        {
            tail->next = elem;
            tail = elem;
        } else
        {
            head = tail = elem;
        }
    }

    TCB *removeFirst()
    {
        if (!head) { return 0; }

        Elem *elem = head;
        head = head->next;
        if (!head) { tail = 0; }

        TCB *ret = elem->data;
        delete elem;
        return ret;
    }

    TCB *peekFirst()
    {
        if (!head) { return 0; }
        return head->data;
    }

    TCB *removeLast()
    {
        if (!head) { return 0; }

        Elem *prev = 0;
        for (Elem *curr = head; curr && curr != tail; curr = curr->next)
        {
            prev = curr;
        }

        Elem *elem = tail;
        if (prev) { prev->next = 0; }
        else { head = 0; }
        tail = prev;

        TCB *ret = elem->data;
        delete elem;
        return ret;
    }

    TCB *peekLast()
    {
        if (!tail) { return 0; }
        return tail->data;
    }

    void addBySemTime(TCB *data);
    void dec();
    int removeThis(TCB* data);
};



#endif //OS1_VEZBE07_RISCV_CONTEXT_SWITCH_2_INTERRUPT_LIST_HPP
