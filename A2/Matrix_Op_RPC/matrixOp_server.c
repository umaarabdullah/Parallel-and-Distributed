/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "matrixOp.h"
#include <math.h>

/** Function Prototypes **/
int perform_gaussian_elimination(Matrix *, Matrix *);
double determinant(int, double []);

Matrix *
matrix_add_1_svc(MatrixPair *argp, struct svc_req *rqstp)
{
	static Matrix result;

	if (argp == NULL) {
        fprintf(stderr, "matrix_add_1_svc: Invalid input\n");
        return NULL;
    }

	int rows, cols;
	
	rows = argp->matrix1.rows;
    cols = argp->matrix1.cols;
	
	rows = argp->matrix2.rows;
    cols = argp->matrix2.cols;

	result.rows = rows;
	result.cols = cols;

    /* Check if the matrices have the same dimensions */
    if (argp->matrix1.rows != argp->matrix2.rows || argp->matrix1.cols != argp->matrix2.cols) {
        fprintf(stderr, "matrix_add_1_svc: Matrices must have the same dimensions for addition\n");
        return NULL;
    }

    /* Perform matrix addition */
	int i, j;
    for (i = 0; i < argp->matrix1.rows; i++) {
        for (j = 0; j < argp->matrix1.cols; j++) {
            result.data[i*result.cols + j] = argp->matrix1.data[i*argp->matrix1.cols + j] +
                                             argp->matrix2.data[i*argp->matrix2.cols + j];
        }
    }

	return &result;
}

Matrix *
matrix_mul_1_svc(MatrixPair *argp, struct svc_req *rqstp)
{
	static Matrix result;

	// Check matrix dimensions for multiplication
    if (argp->matrix1.cols != argp->matrix2.rows) {
        fprintf(stderr, "Invalid matrix dimensions for multiplication\n");
        return NULL;
    }

	result.rows = argp->matrix1.rows;
    result.cols = argp->matrix2.cols;

    for (int i = 0; i < argp->matrix1.rows; i++) {
        for (int j = 0; j < argp->matrix2.cols; j++) {
            result.data[i*result.cols + j] = 0;
            for (int k = 0; k < argp->matrix1.cols; k++) {
                result.data[i*result.cols + j] +=
                    argp->matrix1.data[i*argp->matrix1.cols + k] *
                    argp->matrix2.data[k*argp->matrix2.cols + j];
            }
        }
    }

	return &result;
}

Matrix *
matrix_inverse_1_svc(Matrix *argp, struct svc_req *rqstp)
{
	static Matrix result;

	// Check if the matrix is square
    if (argp->rows != argp->cols){
        fprintf(stderr, "Matrix is not square, cannot be inverted\n");
        return NULL;
    }

	// check if matrix is singular or not
	double det = determinant(argp->cols, argp->data);
	printf("Determinant: %lf\n", det);
	if(det == 0){
		fprintf(stderr, "Matrix is singular, cannot be inverted\n");
        return NULL;
	}

	result.rows = argp->rows;
    result.cols = argp->cols;

	// Perform Gaussian elimination to find the inverse
    if (perform_gaussian_elimination(argp, &result) == -1) {
        fprintf(stderr, "Matrix inversion failed\n");
        return NULL;
    }

	return &result;
}

Matrix *
matrix_transpose_1_svc(Matrix *argp, struct svc_req *rqstp)
{
	static Matrix result;

    // Check if argp is NULL
    if (argp == NULL) {
        fprintf(stderr, "matrix_transpose_1_svc: Invalid input\n");
        return NULL;
    }

    // Set dimensions for the result matrix (transpose)
    result.rows = argp->cols;
    result.cols = argp->rows;

    // Compute the transpose of the matrix
    for (int i = 0; i < argp->rows; i++) {
        for (int j = 0; j < argp->cols; j++) {
            result.data[j * result.cols + i] = argp->data[i * argp->cols + j];
        }
    }

	return &result;
}

// Take input matrix "mat" and return inverse matrix "inv"
int perform_gaussian_elimination(Matrix *mat, Matrix *inv){
	
	int n = mat->rows;

    // Create an augmented matrix [mat | I] for inverse calculation
    Matrix augmented;
    augmented.rows = n;
    augmented.cols = 2 * n;

	// Initialize augmented matrix with the input matrix and an identity matrix
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            augmented.data[i * augmented.cols + j] = mat->data[i * n + j];
            augmented.data[i * augmented.cols + (j + n)] = (i == j) ? 1 : 0;
        }
    }

	// Gaussian elimination
    for (int i = 0; i < n; i++) {

        // Pivoting: Find the row with the largest pivot element
        int max_row = i;
        for (int k = i + 1; k < n; k++) {
            if (fabs(augmented.data[k * augmented.cols + i]) > fabs(augmented.data[max_row * augmented.cols + i])) {
                max_row = k;
            }
        }

        // Swap rows if necessary
        if (max_row != i) {
            for (int j = 0; j < augmented.cols; j++) {
                double temp = augmented.data[i * augmented.cols + j];
                augmented.data[i * augmented.cols + j] = augmented.data[max_row * augmented.cols + j];
                augmented.data[max_row * augmented.cols + j] = temp;
            }
        }

        // Perform elimination to make the pivot element 1
        double pivot = augmented.data[i * augmented.cols + i];
        if (pivot == 0) {
            fprintf(stderr, "Matrix is not invertible\n");
            return -1;
        }

        for (int j = 0; j < augmented.cols; j++) {
            augmented.data[i * augmented.cols + j] /= pivot;
        }

        // Apply the same row operations to the other rows to make the column elements 0
        for (int k = 0; k < n; k++) {
            if (k != i) {
                double factor = augmented.data[k * augmented.cols + i];
                for (int j = 0; j < augmented.cols; j++) {
                    augmented.data[k * augmented.cols + j] -= factor * augmented.data[i * augmented.cols + j];
                }
            }
        }

    }

    // Copy the inverse part from the augmented matrix to 'inv'
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            inv->data[i * n + j] = augmented.data[i * augmented.cols + (j + n)];
        }
    }

	return 0;
}

// Function to calculate the determinant of a square matrix in 1D array format
double determinant(int size, double matrix[]) {
    if (size == 1) {
        return matrix[0]; // For a 1x1 matrix, determinant is its only element
    }
    
    double det = 0;
    
    // Loop over the first row to expand the determinant using cofactor expansion
    for (int i = 0; i < size; i++) {
        double minor[size - 1][size - 1];
        
        // Create the minor matrix by skipping the current row and column
        for (int row = 1; row < size; row++) {
            for (int col = 0, j = 0; col < size; col++) {
                if (col != i) {
                    minor[row - 1][j++] = matrix[row * size + col];
                }
            }
        }
        
        // Calculate the determinant recursively for the minor matrix
        double minorDet = determinant(size - 1, (double *)minor);
        det += (i % 2 == 0 ? 1 : -1) * matrix[i] * minorDet;
    }
    
    return det;
}
