#include<stdio.h>


double str2double( char str[]) {
	int sign = 1;
	int i = 0;
	int now_fraction = 0;	
	int now_exp = 0;
	int exp_sign = 1;
		
	double exp_value = 0.0;
	double fraction = 0.0;
	double multiplier_frac = 10.0;
	double multiplier = 1.0;
	double result = 0.0;

	if (str[i] == '+' || str[i] == ' ') {
		sign = 1;
		i++;
	} else if (str[i] == '-') {
		sign = -1;
		i++;
	}
	
	while (str[i] != '\0') {
		if (str[i] == '.') {
			now_fraction = 1;
			i++;
			continue;
		}

		if (str[i] == 'e' || str[i] == 'E') {
			now_exp = 1;
			i++;

			if (str[i] == '+' || str[i] == ' ') {
				exp_sign = 1;
				i++;
			} else if (str[i] == '-') {
				exp_sign = -1;
				i++;
			}
			continue;
		}
		
		
		if(str[i] >= '0' && str[i] <= '9') {
			int digit = str[i] - '0';
		
			if (now_exp) {
				exp_value = exp_value*10 + digit;
			} else if (now_fraction) {
				fraction = fraction + digit/multiplier_frac;
				multiplier_frac *= 10.0;
			} else {
				result = result * 10 + digit;
			}

		}
		i++;
	}

	result += fraction;

	if (exp_value > 0) {
		for (int j = 0; j < exp_value; j++) {
			if (exp_sign == 1) {
				multiplier *= 10.0;
			} else {
				multiplier /= 10.0;
			}
		}
		result *= multiplier;
	}

	return (sign * result);
}

int main() {
	char str[100];

	while (scanf("%s",str) != EOF) {
		printf("%.10g\n", str2double(str));
	}

	return 0;
}
