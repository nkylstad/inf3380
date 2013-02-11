#include <stdio.h>
#include <stdlib.h>
//#include <mpi.h>

typedef struct 
{
	float** image_data;
	int m;
	int n;
}image;


void allocate_image(image *u, int m, int n)
{
	int i,j;

}


int main(int argc, char const *argv[])
{
	image u;
	int my_m, my_n, i,j;

	my_m=2; my_n=2;

	printf("Finished declaring stuff.\n");
	

	unsigned char *image_chars = (unsigned char *)malloc(my_m*my_n*sizeof(unsigned char));
	for (i=0; i<(my_m*my_n); i++)
	{
		image_chars[i] = i;
	}

	printf("Created array.\n");


	for (i=0; i<(my_m*my_n); i++)
	{
		printf("image_chars[%d] = %u\n", i, image_chars[i]);
	}

	printf("Allocating to 2D array\n");

	u.image_data = (float**)malloc(my_m*sizeof(float*));
	for (i=0; i<my_m; i++)
	{
		u.image_data[i] = (float*)malloc(my_n*sizeof(float));
	}


	for (i=0; i<my_m; i++)
	{
		for (j=0; j<my_n; j++)
		{
			u.image_data[i][j] = (float)image_chars[my_n*i + j];
		}
	}

	for (i=0; i<my_m; i++)
	{
		for (j=0; j<my_n; j++)
		{
			printf("image_data[%d][%d]= %f\n", i,j,u.image_data[i][j]);
		}
	}

	int bleh;

	bleh = sizeof(u.image_data)/sizeof(float);

	printf("array size: %d\n", bleh);

	printf("Finished\n");

	return 0;
}
