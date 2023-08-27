/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "matrixOp.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h> 

/** Macros **/
#define ADD_OPERATION "Matrix Addition\n"
#define MUL_OPERATION "Matrix Multiplication\n"
#define INVERSE_OPERATION "Matrix Inverse\n"
#define TRANSPOSE_OPERATION "Matrix Transpose\n"

/** Global Variables **/
Matrix matrix1, matrix2;

/** Function Prototypes **/
void read_input_file(void);
void write_output_file(int, ...);
void print_matrix(Matrix);

void
matrix_prog_1(char *host)
{
	CLIENT *clnt;

#ifndef	DEBUG
	clnt = clnt_create (host, MATRIX_PROG, MATRIX_VERS, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */
		
	Matrix *result_1 = NULL;
	MatrixPair matrix_add_1_arg;
	matrix_add_1_arg.matrix1 = matrix1;
	matrix_add_1_arg.matrix2 = matrix2;

	result_1 = matrix_add_1(&matrix_add_1_arg, clnt);
	if (result_1 == (Matrix *) NULL) {
		clnt_perror (clnt, "call failed");
	}


	Matrix *result_2 = NULL;
	MatrixPair matrix_mul_1_arg;
	matrix_mul_1_arg.matrix1 = matrix1;
	matrix_mul_1_arg.matrix2 = matrix2;

	result_2 = matrix_mul_1(&matrix_mul_1_arg, clnt);
	if (result_2 == (Matrix *) NULL) {
		clnt_perror (clnt, "call failed");
	}

		
	Matrix *result_3 = NULL;
	Matrix matrix_inverse_1_arg;
	matrix_inverse_1_arg = matrix1;

	result_3 = matrix_inverse_1(&matrix_inverse_1_arg, clnt);
	if (result_3 == (Matrix *) NULL) {
		clnt_perror (clnt, "call failed");
		fprintf(stderr, "Matrix is not invertible !\n");
	}

	
	Matrix *result_4 = NULL;
	Matrix matrix_transpose_1_arg;
	matrix_transpose_1_arg = matrix1;

	result_4 = matrix_transpose_1(&matrix_transpose_1_arg, clnt);
	if (result_4 == (Matrix *) NULL) {
		clnt_perror (clnt, "call failed");
	}

	if(result_1 != NULL && result_2 != NULL && result_3 != NULL && result_4 != NULL)
		write_output_file(4, result_1, result_2, result_3, result_4);

	
#ifndef	DEBUG
	clnt_destroy (clnt);
#endif	 /* DEBUG */
}


int
main (int argc, char *argv[])
{
	char *host;

	if (argc < 2) {
		printf ("usage: %s server_host\n", argv[0]);
		exit (1);
	}
	
	host = argv[1];

	read_input_file();		// read input matrices from file and into global variables matrix1 and matrix2

	matrix_prog_1 (host);
exit (0);
}

void write_output_file(int numMatrices, ...){

    FILE *outputFile = fopen("IO/output.txt", "w");
    if (outputFile == NULL) {
        perror("Error opening file");
        return;
    }

    va_list matrices;
    va_start(matrices, numMatrices);

    fclose(fopen("IO/output.txt", "w"));	// Clear the file

    for (int i = 0; i < numMatrices; i++) {
		fprintf(outputFile, "%s", (i == 0) ? ADD_OPERATION : (i == 1) ? MUL_OPERATION : (i == 2) ? INVERSE_OPERATION : (i == 3) ? TRANSPOSE_OPERATION : "");

			
        Matrix *matrix = va_arg(matrices, Matrix *);
        if (matrix != NULL) {
			// Write dimensions
            fprintf(outputFile, "%d %d\n", matrix->rows, matrix->cols);

            // Write matrix data
            for (int j = 0; j < matrix->rows; j++) {
                for (int k = 0; k < matrix->cols; k++) {
                    fprintf(outputFile, "%.3f ", matrix->data[j * matrix->cols + k]);
                }
                fprintf(outputFile, "\n");
            }
			fprintf(outputFile, "\n\n");
        }
    }

    va_end(matrices);
    fclose(outputFile);
}

void read_input_file(void){

	// Open the input file
	FILE *inputFile = fopen("IO/input.txt", "r");
	if (inputFile == NULL) {
		perror("Error opening file");
		return;
	}

	int rows, cols, totalElements;
	fscanf(inputFile, "%d %d", &rows, &cols);
	totalElements = rows * cols;

	matrix1.rows = rows;
	matrix1.cols = cols;
	for (int i = 0; i < totalElements && i < sizeof(matrix1.data); i++) {		// 1024 being the size of allocated data array in matrix structure
		fscanf(inputFile, "%lf", &matrix1.data[i]);
	}

	fscanf(inputFile, "%d %d", &rows, &cols);
	totalElements = rows * cols;

	matrix2.rows = rows;
	matrix2.cols = cols;

	for (int i = 0; i < totalElements && i < sizeof(matrix1.data); i++) {
		fscanf(inputFile, "%lf", &matrix2.data[i]);
	}

	fclose(inputFile);	// Close the file
	
}

void print_matrix(struct Matrix matrix){
    for (int i = 0; i < matrix.rows; i++) {
        for (int j = 0; j < matrix.cols; j++) {
            printf("%.2f ", matrix.data[i * matrix.cols + j]);
        }
        printf("\n");
    }
}
