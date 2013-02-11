/************************************************/
/*												*/
/* Function to calculate the product 			*/
/* of two values.								*/
/*												*/
/************************************************/

#include <stdio.h>
#include <stdlib.h>

#define SCREAM "aaaargh"

int findProduct(int a, int b) {
	return a*b;
}

int main(int nargs, char** args){

	int prod, a, b;
	a = 3; b = 5;
	prod = findProduct(a,b);
	printf("Product of %d and %d is %d\n", a, b, prod);
	printf(SCREAM"\n");
	return 0;
}