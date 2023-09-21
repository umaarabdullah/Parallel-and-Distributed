/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "matrixOp.h"

bool_t
xdr_Matrix (XDR *xdrs, Matrix *objp)
{
	register int32_t *buf;

	int i;
	 if (!xdr_int (xdrs, &objp->rows))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->cols))
		 return FALSE;
	 if (!xdr_vector (xdrs, (char *)objp->data, 30,
		sizeof (double), (xdrproc_t) xdr_double))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_MatrixPair (XDR *xdrs, MatrixPair *objp)
{
	register int32_t *buf;

	 if (!xdr_pointer (xdrs, (char **)&objp->matrix1, sizeof (Matrix), (xdrproc_t) xdr_Matrix))
		 return FALSE;
	 if (!xdr_pointer (xdrs, (char **)&objp->matrix2, sizeof (Matrix), (xdrproc_t) xdr_Matrix))
		 return FALSE;
	return TRUE;
}
