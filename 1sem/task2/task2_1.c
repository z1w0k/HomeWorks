#include <stdio.h>


double root(double x, double epsilon) {
	double x_initial = 1.0;
	double x_next = 0.5 * (x_initial + x / x_initial);

	while ((x_initial - x_next >= epsilon) || (x_next - x_initial >= epsilon)) {
		x_initial = x_next;
		x_next = 0.5 * (x_initial + x / x_initial);
	
	}
	return x_next;
}


int main() {
    double epsilon;
    scanf("%lf", &epsilon);
    
    double x;
    while (scanf("%lf", &x) != EOF) {
        double result = root(x, epsilon);
        printf("%.10g\n", result);
    }
    
    return 0;
}


