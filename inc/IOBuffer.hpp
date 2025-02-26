//
// Created by os on 5/21/24.
//

#ifndef PROJEKAT_IOBUFFER_HPP
#define PROJEKAT_IOBUFFER_HPP

#include "../lib/hw.h"
#include "../h/kSemaphore.hpp"


//koriscen zadatak 1, jun 2022
class IOBuffer {
public:
   IOBuffer() : head1(0), tail1(0), count1(0), head2(0), tail2(0), count2(0), itemAvailable1(0), itemAvailable2(0) {}
    void putc1(char c);
    char getc1();
    void putc2(char c);
    char getc2();
    size_t getCount2(){return count2;}
private:
    static const size_t B_SIZE = 256;
    //ulazni, za konzolu
    char buffer1[B_SIZE];
    //izlazni, za ekran
    char buffer2[B_SIZE];

    size_t head1, tail1, count1;
    size_t head2, tail2, count2;
    kSemaphore itemAvailable1, itemAvailable2;
};






#endif //PROJEKAT_IOBUFFER_HPP
