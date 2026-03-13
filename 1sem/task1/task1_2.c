#include <stdio.h>

int main() {
	int first, second;

    printf("Положительные числа\n");
    printf("7 %% 3 = %d\n", 7 % 3);
    printf("10 %% 4 = %d\n", 10 % 4);
    printf("15 %% 6 = %d\n", 15 % 6);
    printf("5 %% 5 = %d\n\n", 5 % 5);

    printf("Отрицательный делимое\n");
    printf("7 %% -3 = %d\n", 7 % -3);
    printf("10 %% -4 = %d\n", 10 % -4);
    printf("15 %% -6 = %d\n", 15 % -6);
    printf("5 %% -5 = %d\n\n", 5 % -5);

    printf("Отрицательное делитель\n");
    printf("-7 %% 3 = %d\n", -7 % 3);
    printf("-10 %% 4 = %d\n", -10 % 4);
    printf("-15 %% 6 = %d\n", -15 % 6);
	printf("-5 %% 5 = %d\n\n", -5 % 5);
    
    printf("Оба операнда отрицательные\n");
    printf("-7 %% -3 = %d\n", -7 % -3);
    printf("-10 %% -4 = %d\n", -10 % -4);
    printf("-15 %% -6 = %d\n", -15 % -6);
	printf("-5 %% -5 = %d\n\n", -5 % -5);

	printf("Проверьте все что хотите:\n");
	printf("Введите первое число:\n");
	scanf("%d", &first);
	printf("Ведите второе число:\n");
	scanf("%d",&second);	
	printf("%d %% %d = %d",first, second, first % second);
	    
}
