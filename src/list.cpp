//
// Created by os on 5/12/24.
//
#include "../h/list.hpp"
#include "../h/tcb.hpp"

void List::addBySemTime(TCB *data)
{
    Elem *elem = new Elem(data, 0);
    Elem *t = head, *p = 0;
    while(t && ((elem->data)->getSemTime() >= (t->data)->getSemTime())){
        p=t;
        t=t->next;
    }
    if(p){
        p->next = elem;
    }
    else{
        head = elem;
    }
    elem->next = t;
    if(!t){
        tail = elem;
    }
}

void List::dec(){
    Elem* t = head;
    while(t){
        t->data->setSemTime(t->data->getSemTime()-1);
        t=t->next;
    }
}

int List::removeThis(TCB* data){
    Elem* t = head, *p = 0;
    while(t){
        if(t->data == data){
            if(p){
                p->next = t->next;
            }
            else{
                head = t->next;
            }
            if(!t->next){
                tail = p;
            }
            delete t;
            return 0;
        }
        else{
            p = t;
            t = t->next;
        }
    }
    return -1;
}