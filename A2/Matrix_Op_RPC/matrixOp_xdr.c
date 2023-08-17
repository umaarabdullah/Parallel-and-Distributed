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

	if (xdrs->x_op == XDR_ENCODE) {
		buf = XDR_INLINE (xdrs, (2 +  1024 )* BYTES_PER_XDR_UNIT);
		if (buf == NULL) {
			 if (!xdr_int (xdrs, &objp->rows))
				 return FALSE;
			 if (!xdr_int (xdrs, &objp->cols))
				 return FALSE;
			 if (!xdr_vector (xdrs, (char *)objp->data, 1024,
				sizeof (int), (xdrproc_t) xdr_int))
				 return FALSE;
		} else {
			IXDR_PUT_LONG(buf, objp->rows);
			IXDR_PUT_LONG(buf, objp->cols);
			{
				register int *genp;

				for (i = 0, genp = objp->data;
					i < 1024; ++i) {
					IXDR_PUT_LONG(buf, *genp++);
				}
			}
		}
		return TRUE;
	} else if (xdrs->x_op == XDR_DECODE) {
		buf = XDR_INLINE (xdrs, (2 +  1024 )* BYTES_PER_XDR_UNIT);
		if (buf == NULL) {
			 if (!xdr_int (xdrs, &objp->rows))
				 return FALSE;
			 if (!xdr_int (xdrs, &objp->cols))
				 return FALSE;
			 if (!xdr_vector (xdrs, (char *)objp->data, 1024,
				sizeof (int), (xdrproc_t) xdr_int))
				 return FALSE;
		} else {
			objp->rows = IXDR_GET_LONG(buf);
			objp->cols = IXDR_GET_LONG(buf);
			{
				register int *genp;

				for (i = 0, genp = objp->data;
					i < 1024; ++i) {
					*genp++ = IXDR_GET_LONG(buf);
				}
			}
		}
	 return TRUE;
	}

	 if (!xdr_int (xdrs, &objp->rows))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->cols))
		 return FALSE;
	 if (!xdr_vector (xdrs, (char *)objp->data, 1024,
		sizeof (int), (xdrproc_t) xdr_int))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_MatrixPair (XDR *xdrs, MatrixPair *objp)
{
	register int32_t *buf;

	 if (!xdr_Matrix (xdrs, &objp->matrix1))
		 return FALSE;
	 if (!xdr_Matrix (xdrs, &objp->matrix2))
		 return FALSE;
	return TRUE;
}