/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "matrixOp.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/** Macros **/
#define ADD_OPERATION "add"
#define MUL_OPERATION "mul"
#define INVERSE_OPERATION "inverse"
#define TRANSPOSE_OPERATION "transpose"

/** Global Variables **/
Matrix matrix1, matrix2;

/** Function Prototypes **/
void read_input_file(const int);
void write_output_file(const Matrix *);

void
matrix_prog_1(char *host, const char *op)
{
	CLIENT *clnt;

#ifndef	DEBUG
	clnt = clnt_create (host, MATRIX_PROG, MATRIX_VERS, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */

	if(strcasecmp(op, ADD_OPERATION) == 0){
		
		Matrix *result_1;
		MatrixPair matrix_add_1_arg;
		matrix_add_1_arg.matrix1 = matrix1;
		matrix_add_1_arg.matrix2 = matrix2;

		result_1 = matrix_add_1(&matrix_add_1_arg, clnt);
		if (result_1 == (Matrix *) NULL) {
			clnt_perror (clnt, "call failed");
		}

		if(result_1 != NULL)
			write_output_file(result_1);
	}
	else if(strcasecmp(op, MUL_OPERATION) == 0){

		Matrix *result_2;
		MatrixPair matrix_mul_1_arg;
		matrix_mul_1_arg.matrix1 = matrix1;
		matrix_mul_1_arg.matrix2 = matrix2;

		result_2 = matrix_mul_1(&matrix_mul_1_arg, clnt);
		if (result_2 == (Matrix *) NULL) {
			clnt_perror (clnt, "call failed");
		}

		if(result_2 != NULL)
			write_output_file(result_2);
		
	}
	else if(strcasecmp(op, INVERSE_OPERATION) == 0){
		
		Matrix *result_3;
		Matrix matrix_inverse_1_arg;
		matrix_inverse_1_arg = matrix1;

		result_3 = matrix_inverse_1(&matrix_inverse_1_arg, clnt);
		if (result_3 == (Matrix *) NULL) {
			clnt_perror (clnt, "call failed");
			fprintf(stderr, "Matrix is not invertible !\n");
		}

		if(result_3 != NULL)
			write_output_file(result_3);
		
	}
	else if(strcasecmp(op, TRANSPOSE_OPERATION) == 0){
		
		Matrix *result_4;
		Matrix matrix_transpose_1_arg;
		matrix_transpose_1_arg = matrix1;

		result_4 = matrix_transpose_1(&matrix_transpose_1_arg, clnt);
		if (result_4 == (Matrix *) NULL) {
			clnt_perror (clnt, "call failed");
		}

		if(result_4 != NULL)
			write_output_file(result_4);
	}

	
	
#ifndef	DEBUG
	clnt_destroy (clnt);
#endif	 /* DEBUG */
}


int
main (int argc, char *argv[])
{
	char *host;
	char *op;		/** add, mul, inverse, transpose **/

	if (argc < 2) {
		printf ("usage: %s server_host\n", argv[0]);
		exit (1);
	}
	
	host = argv[1];
	op = argv[2];

	if(strcasecmp(op, ADD_OPERATION) == 0){	// second argument must be matrix operation name
		read_input_file(2);
	}
	else if(strcasecmp(op, MUL_OPERATION) == 0){
		read_input_file(2);
	}
	else if(strcasecmp(op, INVERSE_OPERATION) == 0){
		read_input_file(1);
	}
	else if(strcasecmp(op, TRANSPOSE_OPERATION) == 0){
		read_input_file(1);
	}

	matrix_prog_1 (host, op);
exit (0);
}

void write_output_file(const Matrix *matrix){
    
	FILE *outputFile = fopen("IO/output.txt", "w");
    if (outputFile == NULL) {
        perror("Error opening file");
        return;
    }

    // Write dimensions
    fprintf(outputFile, "%d %d\n", matrix->rows, matrix->cols);

    // Write matrix data
    for (int i = 0; i < matrix->rows; i++) {
        for (int j = 0; j < matrix->cols; j++) {
            fprintf(outputFile, "%d ", matrix->data[i * matrix->cols + j]);
        }
        fprintf(outputFile, "\n");
    }

    fclose(outputFile);
}

void read_input_file(const int numOfInputMatrices){

	// Open the input file
	FILE *inputFile = fopen("IO/input.txt", "r");
	if (inputFile == NULL) {
		perror("Error opening file");
		return;
	}

	if(numOfInputMatrices == 2){
		int rows, cols, totalElements;
		fscanf(inputFile, "%d %d", &rows, &cols);
		totalElements = rows * cols;

		matrix1.rows = rows;
		matrix1.cols = cols;
		for (int i = 0; i < totalElements && i < sizeof(matrix1.data); i++) {		// 1024 being the size of allocated data array in matrix structure
			fscanf(inputFile, "%d", &matrix1.data[i]);
		}

		fscanf(inputFile, "%d %d", &rows, &cols);
		totalElements = rows * cols;

		matrix2.rows = rows;
		matrix2.cols = cols;

		for (int i = 0; i < totalElements && i < sizeof(matrix1.data); i++) {
			fscanf(inputFile, "%d", &matrix2.data[i]);
		}
	}
	else if(numOfInputMatrices == 1){
		int rows, cols, totalElements;
		fscanf(inputFile, "%d %d", &rows, &cols);
		totalElements = rows * cols;

		matrix1.rows = rows;
		matrix1.cols = cols;

		for (int i = 0; i < totalElements && i < sizeof(matrix1.data); i++) {
			fscanf(inputFile, "%d", &matrix1.data[i]);
		}
	}

	fclose(inputFile);	// Close the file
	
}

