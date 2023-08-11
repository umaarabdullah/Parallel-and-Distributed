/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "matrixOp.h"


void
matrix_prog_1(char *host)
{
	CLIENT *clnt;
	Matrix  *result_1;
	MatrixPair  matrix_add_1_arg;
	Matrix  *result_2;
	MatrixPair  matrix_mul_1_arg;
	Matrix  *result_3;
	Matrix  matrix_inverse_1_arg;
	Matrix  *result_4;
	Matrix  matrix_transpose_1_arg;

#ifndef	DEBUG
	clnt = clnt_create (host, MATRIX_PROG, MATRIX_VERS, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */

	result_1 = matrix_add_1(&matrix_add_1_arg, clnt);
	if (result_1 == (Matrix *) NULL) {
		clnt_perror (clnt, "call failed");
	}
	result_2 = matrix_mul_1(&matrix_mul_1_arg, clnt);
	if (result_2 == (Matrix *) NULL) {
		clnt_perror (clnt, "call failed");
	}
	result_3 = matrix_inverse_1(&matrix_inverse_1_arg, clnt);
	if (result_3 == (Matrix *) NULL) {
		clnt_perror (clnt, "call failed");
	}
	result_4 = matrix_transpose_1(&matrix_transpose_1_arg, clnt);
	if (result_4 == (Matrix *) NULL) {
		clnt_perror (clnt, "call failed");
	}
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
	matrix_prog_1 (host);
exit (0);
}
