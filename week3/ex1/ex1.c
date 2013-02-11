#include <stdio.h>

double limit(int n) {
	double sum, add;
	int i;
	sum = 0.0;
	add = 1.0;

	for (i=0; i < n; i++){
		sum = sum + add;
		printf("sum:%f\n",sum);
		add = add*(1.0/4);
		sum = sum - add;
		printf("sum:%f\n",sum);
		add = add*(1.0/4);
	}
	printf("final sum:%f\n",sum);
	return sum;

}


int main(int nargs, char** args)
{
	int n;
	double exact, s;
	n = atoi(args[1]);
	s = limit(n);
	exact = 4.0/5;
	printf("exact: %f, sum: %f\n", exact, s);
	return 0;
}