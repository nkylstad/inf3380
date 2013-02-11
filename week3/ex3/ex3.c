#include <stdio.h>
#include <stdlib.h>

int*** make_array3D(int nx, int ny, int nz){
	int *allElements = (int*)malloc(nx * ny * nz * sizeof(int));
	int ***array3D = (int***)malloc(nx * sizeof(int **));
	int i, j;
	for(i = 0; i < nx; i++){
	    array3D[i] = malloc(ny * sizeof(int *));
	    for(j = 0; j < ny; j++){
	        array3D[i][j] = allElements + (i * ny * nz) + (j * nz);
	    }
	}
	return array3D;
}

void print_3Darray(int*** A, int nx, int ny, int nz){
	int i, j, k;
	for (i=0; i<nx; i++){
		for (j=0; j<ny; j++){
			for (k=0; k<nz; k++){
				A[i][j][k] = i + j + k;
				//printf("%d\n", array3D[i][j][k]);
				printf("%d\n", A[i][j][k]);
			}
		}
	}
}


int main(int nargs, char** args){
	int nx, ny, nz, i, j, k;
	int t;
	nx=2; ny=2; nz=2;
	t = nx + ny + nz;
	int ***arr3D = make_array3D(nx, ny, nz);
	printf("Doing something...\n");
	print_3Darray(arr3D, nx, ny, nz);
	/*for (i=0; i<nx; i++){
		for (j=0; j<ny; j++){
			for (k=0; k<nz; k++){
				t = i + j + k;
				printf("%d\n", t);
			}
		}
	}*/
	//printf("%d\n", t);
	//printf("%d\n", arr3D[1][1][1]);
	return 0;
}




