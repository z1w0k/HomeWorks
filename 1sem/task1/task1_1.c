#include<stdio.h>

int main() {
	char c = 23;
	printf("Размер short = %d", sizeof(short));
	printf(" \n");

	printf("Размер int = %d", sizeof(int));
	printf("\n");
	
	printf("Размер long = %d", sizeof(long));
	printf("\n");

	printf("Размер float = %d", sizeof(float));
	printf("\n");

	printf("Размер double = %d", sizeof(double));
	printf("\n");

	printf("Размер long double = %d", sizeof(long double));
	printf("\n");
	
	printf("Размер char c = %d", sizeof(c));
	return 0;
}
