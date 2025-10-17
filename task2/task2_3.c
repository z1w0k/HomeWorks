#include<stdio.h>
#include<time.h>

unsigned long long fibonacci_iterative(int i) {
	if (i == 0) 
		return 0;
	

	if (i == 1) 
		return 1;


	long long prev2 = 0;
	long long prev1 = 1;
	long long current;

	for (int n = 2; n <= i; n++) {
	current = prev2 + prev1;
	prev2 = prev1;
	prev1 = current;
	}

	return prev1;
}


unsigned long long fibonacci_recursive(int i) {
	if (i == 0) return 0;
	if (i == 1) return 1;
	
	return fibonacci_recursive(i - 2) + fibonacci_recursive(i - 1);
}

int main() {
	int i;
	clock_t start,end;
	double iterative_counter, recursive_counter;

	while (scanf("%d", &i) != EOF) {
		start = clock();
		long long result_iterative = fibonacci_iterative(i);
		end = clock();

		iterative_counter = ((double)(end - start)) / CLOCKS_PER_SEC;


		start = clock();
		long long result_recursive = fibonacci_recursive(i);
		end = clock();
		
		recursive_counter = ((double)(end - start)) / CLOCKS_PER_SEC;

		printf("Итеративно = %lld\t", result_iterative);
		printf("Время = %.2f\n", iterative_counter);

		printf("Рекурсивно = %lld\t", result_recursive);
		printf("Время = %.2f\n", recursive_counter);
			
	}

	return 0;
}
