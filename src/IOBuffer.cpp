//
// Created by os on 5/21/24.
//

#include "../h/IOBuffer.hpp"



char IOBuffer::getc1() {
    itemAvailable1.wait();
    char c = buffer1[head1];
    head1 = (head1 + 1) % B_SIZE;
    count1--;
    return c;
}
void IOBuffer::putc1(char c) {
    if (count1 < B_SIZE) {
        buffer1[tail1] = c;
        tail1 = (tail1 + 1) % B_SIZE;
        count1++;
        itemAvailable1.signal();
    }
}
char IOBuffer::getc2() {
    itemAvailable2.wait();
    char c = buffer2[head2];
    head2 = (head2 + 1) % B_SIZE;
    count2--;
    return c;
}
void IOBuffer::putc2(char c) {
    if (count2 < B_SIZE) {
        buffer2[tail2] = c;
        tail2 = (tail2 + 1) % B_SIZE;
        count2++;
        itemAvailable2.signal();
    }
}



