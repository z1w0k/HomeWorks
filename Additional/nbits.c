#include<stdio.h>


int nbits() {
    unsigned char value = 1;
    int bits = 0;
    
    while (value != 0) {
        value = value << 1;
        bits++;
    }

    return bits;
}

int main() {
    int bits = nbits();

    printf("Коллмчество бит в char = %d\n", bits);

    return 0;
}
