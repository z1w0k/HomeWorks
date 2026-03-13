#include<stdio.h>




int main(void) {
	int i, j, k, n;
	scanf("%d", &n);
	
	for(i = 0; i < n; i++)
		for( j = 0; j < i + 2; j++) {
			for (k = 0; k < 2*n + 1; k++)
			       if ((k < n-j) || (k > n + j)) {
				       printf(" ");

			       }
				else {
					printf("*");
				}
	printf("\n");

	}
	return 0;
}
