#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char* argv[]) {
    int rank, num_procs;
    MPI_Status status;
    const int tag = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    if (num_procs < 2) {
        fprintf(stderr, "This program requires at least 2 processes.\n");
        MPI_Finalize();
        return 1;
    }

    // Send and receive 1-byte data between processes
    if (rank == 0) {
        // char data_send = 'A';
        int array_send[4] = {1,2,5,6};
        int dest_rank = 1;

        MPI_Send(array_send, 4, MPI_INT, dest_rank, tag, MPI_COMM_WORLD);

        // printf("Process %d sent '%c' to process %d.\n", rank, data_send, dest_rank);
        printf("Process %d sent array of size %d to process %d.\n", rank, sizeof(array_send)/sizeof(int), dest_rank);
    } else if (rank == 1) {
        // char data_recv;
        int array_recv[4];
        int source_rank = 0;

        MPI_Recv(array_recv, 4, MPI_INT, source_rank, tag, MPI_COMM_WORLD, &status);

        // printf("Process %d received '%c' from process %d.\n", rank, data_recv, source_rank);
        printf("Process %d received array of size %d from process %d.\n", rank, sizeof(array_recv)/sizeof(int), source_rank);

        // Check status for more information about the received message
        int received_count;
        MPI_Get_count(&status, MPI_CHAR, &received_count);
        printf("Received %d bytes.\n", received_count);
        printf("Source: %d\n", status.MPI_SOURCE);
        printf("Tag: %d\n", status.MPI_TAG);
    }

    MPI_Finalize();

    return 0;
}