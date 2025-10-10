#include<stdio.h>

int main() {
    int i;
    int count = 0;

    for (i = 111; i <= 999; i++) {
        int digit1 = i / 100;
        int digit2 = (i /10) % 10;
        int digit3 = i % 10;

        if ((digit1 == digit2) || (digit2 == digit3) || (digit3 == digit1)) {
            count++;
        }
    }

    printf("Колличество чисел с 2 одинаковыми цифрами = %d", count);

    return 0;
}
