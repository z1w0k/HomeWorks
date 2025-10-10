#include<stdio.h>



int main() {
	double x, coeff, result;


	if(scanf("%lf", &x) != 1) {
		return 1;
	}


	if(scanf("%lf", &result) != 1) {
		return 1;
	}

	while (scanf("%lf", &coeff) != EOF) {
		result = result*x + coeff;
	} 

	printf("%lf\n", result);

	return 0;
}
