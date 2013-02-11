#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <mpi.h>

// make use of two functions from the simplejpeg library
void import_JPEG_file(const char *filename, unsigned char **image_chars, 
						int *image_height, int *image_width,
						int *num_components);

void export_JPEG_file(const char *filename, unsigned char *image_chars,
						int image_height, int image_width,
						int *num_components, int quality);


/****************************************************/
/*													*/
/*           Create an image struct  				*/
/*													*/
/****************************************************/
typedef struct 
{
	float** image_data;	/* a 2D array of floats */
	int m;				/* # pixels in x-direction */
	int n;				/* # pixels in y-direction */
}
image;



/****************************************************/
/*													*/
/*     		Allocate a 2D array inside u 			*/
/*													*/
/****************************************************/
void allocate_image(image *u, int m, int n)
{
	int i;
	u->image_data = (float**)malloc(m*sizeof(float*));
	for (i=0; i<m; i++)
	{
		u->image_data[i] = (float*)malloc(n*sizeof(float));
	}
	u->m = m; 
	u->n = n;
}



/****************************************************/
/*													*/
/*     	Free storage used by a 2D array 			*/
/*													*/
/****************************************************/
void deallocate_image(image *u)
{
	int i;
	for (i=0; i<u->m; i++)
	{
		free(u->image_data[i]);
	}
	free(u->image_data);
	u->m = 0; 
	u->n = 0;
}



/****************************************************/
/*													*/
/*     		Denoise image stored in u 	 			*/
/*													*/
/****************************************************/
void iso_diffusion_denoising(image *u, image *u_bar, float kappa, int iters)
{
	int i,j,n;

	n = 0;
	memmove(u_bar, u, sizeof(u));  /* Make sure the boundary points are correct */

	while (n < iters)  /* Control number of iterations */
	{
		for (i=1; i<u->m-1; i++)
		{
			for (j=1; j<u->n-1; j++)
			{
				u_bar->image_data[i][j] = u->image_data[i][j] + 
							kappa*(u->image_data[i-1][j] + u->image_data[i][j-1] - 
								4*u->image_data[i][j] + u->image_data[i+1][j] + u->image_data[i][j+1]);
			}
		}
		memmove(u, u_bar, sizeof(u_bar));  /* Update u */
		n++;
	}
}



/****************************************************/
/*													*/
/*     				Main program 	 				*/
/*													*/
/****************************************************/
int main(int argc, char *argv[])
{
	int m, n, c, iters;
	int my_m, my_n, my_rank, num_procs;
	float kappa;
	image u, u_bar;
	unsigned char *image_chars;
	char *input_jpeg_filename, *output_jpeg_filename;

	printf("Now in main program\n");

	// MPI_Init (&argc, &argv);
	// MPI_Comm_rank (MPI_COMM_WORLD, &my_rank);
	// MPI_Comm_size (MPI_COMM_WORLD, &num_procs);

	/* read from kommand line: kappa, iters, input_jpeg filename, output_jpeg_filename */
	/* ... */

	
	if (my_rank==0)
		//import_JPEG_file(input_jpeg_filename, &image_chars, &m, &n, &c);
		printf("my_rank==0\n");
	

	// MPI_Bcast (&m, 1, MPI_INT, 0, MPI_COMM_WORLD);
	// MPI_Bcast (&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

	/* Divide the m x n pixels evenly among the MPI processes */
	// my_m = ...;
	// my_n = ...;

	// allocate_image (&u, my_m, my_n);
	// allocate_image (&u_bar, my_m, my_n);

	/* Each process asks process 0 for a partitioned region */
	/* of image_chars and copy the values into u */
	/* ... */

	// iso_diffusion_denoising (&u, &u_bar, kappa, iters);

	/* Each process sends its resulting content of u_bar to process 0 */
	/* Process 0 recieves from each process incoming values and */
	/* copy them into the designated region of image_chars */
	/* ... */

	// if (my_rank==0)
	// 	export_JPEG_file(output_jpeg_filename, image_chars, m, n, c, 75);

	// deallocate_image(&u);
	// deallocate_image(&u_bar);

	// MPI_Finalize ();

	return 0;
}

