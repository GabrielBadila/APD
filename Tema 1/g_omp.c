#include <stdio.h> 
#include <stdlib.h>
#include <omp.h>


/* Functie pentru copierea matricei principale intr-o matrice secundara pe
   care o vom folosi pentru parcurgere in functia "evolve". */
void copyMatrix(int rows, int cols, int** matrix, int** copy) {

	int i, j;

	#pragma omp parallel for private(i,j)
	for(i = 0; i < rows; i++) {
		for(j = 0; j < cols; j++) {
			matrix[i][j] = copy[i][j];
		}
	}
}


/* Functie de bordare a unei matrice */
void bordMatrix(int rows, int cols, int** matrix) {

	int i;

	#pragma omp parallel
	{
		#pragma omp for private(i)
		for (i = 1; i < cols; i++) {
			matrix[0][i] = matrix[rows - 2][i];
			matrix[rows - 1][i] = matrix[1][i];
		}

		#pragma omp for private(i)
		for ( i = 1; i < rows; i++) {	
			matrix[i][0] = matrix[i][cols - 2];
			matrix[i][cols - 1] = matrix[i][1];
		}

		#pragma omp master
		{
			matrix[0][0] = matrix[rows - 2][cols - 2];
			matrix[0][cols - 1] = matrix[rows - 2][1];
			matrix[rows - 1][0] = matrix[1][cols - 2];
			matrix[rows - 1][cols - 1] = matrix[1][1];
		}
	}
}


/* Functia principala a intregului program. Aceasta schimba elementele matricei
   principale in funtie de transformarile care au loc in timpul unei iteratii.*/
void evolve(int rows, int cols, int** matrix, int** copy) {

	int i, j, neighbors;

	#pragma omp parallel for collapse(2) private (i, j, neighbors)
	for (i = 1; i < rows - 1; i++) {
		for (j = 1; j < cols - 1; j++) {
			neighbors = 0;
			neighbors = matrix[i - 1][j - 1] + matrix[i - 1][j] + matrix[i - 1][j + 1] + 
						matrix[i + 1][j - 1] + matrix[i + 1][j] + matrix[i + 1][j + 1] + 
						matrix[i][j - 1] + matrix[i][j + 1];

			if (neighbors < 2 || neighbors > 3) {
				copy[i][j] = 0;
			}
			else if (matrix[i][j] == 1 && (neighbors == 2 || neighbors == 3)) {
				copy[i][j] = 1;
			}
			else if (matrix[i][j] == 0 && neighbors == 3) {
				copy[i][j] = 1;
			}
		}
	}
}


int main(int argc, char **argv) {

	FILE *file;
	int rows = 0, cols = 0, i, j;
	char c;

	if(argc < 4) {
		printf("Error: <./file> <input> <iterations> <output>\n");
		return 1;
	}

	file = fopen(argv[1], "r");

	int numIterations = atoi(argv[2]);
	fscanf(file, "%d %d", &rows, &cols);

	rows += 2;
	cols += 2;

	/* Alocarea dinamica a matricelor */
	int** matrix = (int**)malloc(rows * sizeof(int*));
	int** copy = (int**)malloc(rows * sizeof(int*));

	for (i = 0;i < rows; i++){
	    matrix[i] = (int*)malloc(cols * sizeof(int));
	    copy[i] = (int*)malloc(cols * sizeof(int));
	}

	/* Citirea matricei */
	for(i = 1; i < rows - 1; i++ ){
		for(j = 1; j < cols - 1; j++) {
			while(!feof(file)) {
				c = getc(file);
				if (c == '.') {
					matrix[i][j] = 0;
					break;
				}
				else if (c == 'X') {
					matrix[i][j] = 1;
					break;
				}
			}
		}
	}

	fclose (file);  

	/* Parcurgerea iteratiiilot si apelarea functiilor */
    for (i = 0; i < numIterations; i++) {
    	bordMatrix(rows, cols, matrix);
    	evolve(rows, cols, matrix, copy);
    	copyMatrix(rows, cols, matrix, copy);
    }

	file = fopen(argv[3], "w");

	/* Scrierea rezultatului */
	for(i = 1; i < rows - 1; i++ ){
		for(j = 1; j < cols - 1; j++) {
			if (matrix[i][j] == 0) {
				putc('.', file);
			}
			else {
				putc('X', file);
			}
			putc(' ', file);
		}
		putc('\n', file);
	}

	fclose (file);  

	/* Eliberarea memoriei */
	for(i = 0; i < rows; i++) {
    	free(matrix[i]);
    	free(copy[i]);
	}
	free(matrix);
	free(copy);

	return 0;
}