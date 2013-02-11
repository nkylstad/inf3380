#include <stdio.h>
#include <stdlib.h>


double *createArray(int n) {

	double *A = (double *)malloc(n*sizeof(double));
	int i;
	for (i=0; i < n; i++){
		A[i] = rand();
		printf("A[%d] = %f\n", i, A[i]);
	}
	return A;
}


double findMin(double *A, int n){
	double t;
	t = A[0];
	int i;
	for (i=1; i<n; i++){
		 if (A[i] < t){
		 	t = A[i];
		 }
	}
	return t;
}


double findMax(double *A, int n){
	double t;
	t = A[0];
	int i;
	for (i=1; i<n; i++){
		if (A[i] > t){
			t = A[i];
		 }		 	
	}
	return t;
}


int main(int nargs, char** args){

	int n, i;
	if (nargs > 1)
	{
		n = atoi(args[1]);
	}else{
		n = 10;
		printf("Using default value for array size, n=%d\n", n);
	}

	double *p = createArray(n);
	double min = findMin(p, n);
	double max = findMax(p, n);
	printf("min:%f, max:%f\n", min, max);
	/*free(p);*/
	return 0;

}