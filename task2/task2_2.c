#include<stdio.h>



int main() {
	double x, coeff, result;
    double result_polynomial = 0.0; 
    double result_derivative = 0.0;

	if(scanf("%lf", &x) != 1) {
		return 1;
    }


	while (scanf("%lf", &coeff) != EOF) {
        result_derivative = result_derivative * x + result_polynomial;   
		result_polynomial = result_polynomial * x + coeff;

	} 

	printf("Многочлен = %.2lf\n", result_polynomial);
    printf("Его производная = %.2lf\n", result_derivative);

	return 0;
}
