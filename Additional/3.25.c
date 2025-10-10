#include<stdio.h>

int main() {
    int n = 0;
    double a;
    double sum = 0.0;

    
    printf("Введите a: ");
    scanf("%lf\n", &a);

    while(sum <= a) {
        n++;
        sum += 1.0/n;
    }

    printf("Наименьшее число n = %d\n", n);
}
