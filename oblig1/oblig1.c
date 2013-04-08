#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

// make use of two functions from the simplejpeg library
void import_JPEG_file(const char *filename, unsigned char **image_chars, 
						int *image_height, int *image_width,
						int *num_components);

void export_JPEG_file(const char *filename, unsigned char *image_chars,
						int image_height, int image_width,
						int num_components, int quality);


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
	printf("...\n");
	printf("Allocating space for image.\n");
	u->image_data = (float**)malloc(m*sizeof(float*));
	for (i=0; i<m; i++)
	{
		u->image_data[i] = (float*)malloc(n*sizeof(float));
	}
	u->m = m; 
	u->n = n;
	printf("Successfully allocated image!\n");
	printf("...\n");
}



/****************************************************/
/*													*/
/*     	Free storage used by a 2D array 			*/
/*													*/
/****************************************************/
void deallocate_image(image *u)
{
	int i;
	printf("...\n");
	printf("Freeing space taken up by image.\n");
	for (i=0; i<u->m; i++)
	{
		free(u->image_data[i]);
	}
	free(u->image_data);
	u->m = 0; 
	u->n = 0;
	printf("Successfully deallocated image!\n");
	printf("...\n");
}



/****************************************************/
/*													*/
/*     		Denoise image stored in u 	 			*/
/*													*/
/****************************************************/
void iso_diffusion_denoising(image *u, image *u_bar, float kappa, int iters)
{
	int i,j,n;
	image *tmp;
	printf("...\n");
	printf("Cleaning up image:\n");
	n = 0;
	

	/* Make sure the boundary is correct */
	for (i=0; i<u->m; i++)
	{
		u_bar->image_data[0][i] = u->image_data[0][i];
		u_bar->image_data[u->m-1][i] = u->image_data[u->m-1][i];
	}

	for (i=0; i<u->n; i++)
	{
		u_bar->image_data[i][0] = u->image_data[0][i];
		u_bar->image_data[i][u->n-1] = u->image_data[i][u->n-1];
	}

	

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
		tmp = u;  /* Update u */
		u = u_bar;
		u_bar = tmp;
		n++;
	}
	printf("Finished cleaning image!\n");
	printf("...\n");
}




/****************************************************/
/*													*/
/*     				Main program 	 				*/
/*													*/
/****************************************************/
int main(int argc, char *argv[])
{
	int m, n, c, iters, i, j;
	int my_m, my_n, my_rank, num_procs, size;
	float kappa;
	image u, u_bar;
	unsigned char *image_chars;
	char *input_jpeg_filename, *output_jpeg_filename;
	int* sendcounts, displs, recvcounts;

	sendcounts = (int*)malloc(num_procs*sizeof(int));
	displs = (int*)malloc(num_procs*sizeof(int));
	recvcounts = (int*)malloc(num_procs*sizeof(int));

	printf("Now in main program\n");

	MPI_Init (&argc, &argv);
	MPI_Comm_rank (MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size (MPI_COMM_WORLD, &num_procs);

	/* read from kommand line: kappa, iters, input_jpeg filename, output_jpeg_filename */
	kappa = atof(argv[1]);
	iters = atoi(argv[2]);
	input_jpeg_filename = argv[3];
	output_jpeg_filename = argv[4];
	/* Test that parameters are read correctly from command line: 
	printf("kappa: %f\n", kappa);
	printf("iters: %d\n", iters);
	printf("input_jpeg_filename: %s\n", input_jpeg_filename);
	printf("output_jpeg_filename: %s\n", output_jpeg_filename);
	*/

	
	if (my_rank==0)
		import_JPEG_file(input_jpeg_filename, &image_chars, &m, &n, &c);
		printf("Successfully imported JPEG image.\n");
	

	MPI_Bcast (&m, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast (&n, 1, MPI_INT, 0, MPI_COMM_WORLD);


	/* Divide the m x n pixels evenly among the MPI processes */
	my_m = m/num_procs;
	my_n = n;


	/* If the pixels cannot be evenly divided, the last process picks up 	*/
	/* the remainder.  														*/
	/* Each process needs the rows above and below it. 						*/
	/* The first and last process only need 1 additional row. 				*/
	if (my_rank == num_procs - 1){
		my_m += m % num_procs;
		allocate_image(&u, my_m+1, my_n);
		allocate_image(&u_bar, my_m+1, my_n);
	} else if (my_rank == 0){
		allocate_image(&u_bar, my_m+1, my_n);
	} else {
		allocate_image (&u, my_m+2, my_n);
		allocate_image (&u_bar, my_m+2, my_n);
	}

	/* Each process asks process 0 for a partitioned region */
	/* of image_chars and copy the values into u */

	if (my_rank==0){
		size = (my_m + 1)*my_n;
		sendcounts[my_rank] = size;
		displs[my_rank] = my_rank;
		displs[my_rank+1] = my_n*(my_rank*my_m - 1);
	} else if (my_rank==num_procs-1){
		size = (my_m + 1)*my_n;
		sendcounts[my_rank] = size;
	} else {
		size = (my_m + 2)*my_n;
		sendcounts[my_rank] = size;
		displs[my_rank+1] = my_n*(my_rank*my_m - 1);
	}


	
	MPI_Scatterv(&image_chars, &sendcounts, &displs, MPI_UNSIGNED_CHAR, &u.image_data, size, MPI_UNSIGNED_CHAR,
		0, MPI_COMM_WORLD);

	/* Convert data type from unsigned char to float: */
	for (i=0; i<my_m; i++)
	{
		for (j=0; j<my_n; j++)
		{
			u.image_data[i][j] = (float)u.image_data[i][j];
		}
	}

	iso_diffusion_denoising (&u, &u_bar, kappa, iters);

	/* Each process must convert the data type in u back */
	/* to unsigned char. */
	for (i=0; i<my_m; i++)
	{
		for (j=0; j<my_n; j++)
		{
			u.image_data[i][j] = (unsigned char)u.image_data[i][j];
		}
	}

	/* Each process sends its resulting content of u to process 0 */
	/* Process 0 recieves from each process incoming values and */
	/* copy them into the designated region of image_chars */
	/* ... */


	if (my_rank==0){
		displs[my_rank] = 0;
	
	displs[my_rank+1] = my_rank*my_m*my_n;
	size = my_m*my_n




	if (my_rank==0)
		c = 1;
		export_JPEG_file(output_jpeg_filename, image_chars, m, n, c, 75);
		printf("Successfully exported JPEG image! \n");

	deallocate_image(&u);
	deallocate_image(&u_bar);

	MPI_Finalize ();

	printf("Finished the program!\n");

	return 0;
}

