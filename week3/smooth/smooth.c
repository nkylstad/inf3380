#include <stdio.h>
#include <stdlib.h>

void smooth (double **v_new, double **v, int nx, int ny, double c){
	int i,j;
	for (i=1; i<nx-1; i++){
		for (j=1; j<ny-1; j++){
			
			/* general scheme for internal points */
			v_new[i][j] = v[i][j] + c*(v[i-1][j] + v[i][j-1] - 4*v[i][j] + v[i][j+1] v[i+1][j]);
			
			/* special scheme for points on boundary */
			v_new[0][0] = v[0][0];
			v_new[0][n-1] = v[0][n-1];
			v_new[n-1][0] = v[n-1][0];
			v_new[n-1][n-1] = v[n-1][n-1];
		}
	}
}


int main(int nargs, char** args){

	

	return 0;
}