
struct Matrix {
    int rows;
    int cols;
    int *data; /* Pointer to dynamically allocated data array */
};

struct MatrixPair {
    Matrix matrix1;
    Matrix matrix2;
};

program MATRIX_PROG {
    version MATRIX_VERS {
        /* Define a remote procedure named matrix_add that takes a MatrixPair argument */
        Matrix matrix_add(MatrixPair) = 1;
        /* Define a remote procedure named matrix_mul that takes a MatrixPair argument */
        Matrix matrix_mul(MatrixPair) = 2;
        /* Define a remote procedure named matrix_inverse that takes a Matrix argument */
        Matrix matrix_inverse(Matrix) = 3;
        /* Define a remote procedure named matrix_transpose that takes a Matrix argument */
        Matrix matrix_transpose(Matrix) = 4;
    } = 1;
} = 0x31230000;
