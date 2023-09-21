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

    // Send and receive an array of dynamic size between processes
    if (rank == 0) {
        int* array_send;
        int array_size = 4;
        int dest_rank = 1;

        // Allocate memory for the dynamic array
        array_send = (int*)malloc(array_size * sizeof(int));
        if (array_send == NULL) {
            fprintf(stderr, "Memory allocation failed.\n");
            MPI_Finalize();
            return 1;
        }

        // Initialize the array with values
        for (int i = 0; i < array_size; i++) {
            array_send[i] = i + 1;
        }

        MPI_Send(array_send, array_size, MPI_INT, dest_rank, tag, MPI_COMM_WORLD);

        printf("Process %d sent an array of size %d to process %d.\n", rank, array_size, dest_rank);

        // Free the dynamically allocated memory
        free(array_send);

        int sum_recv;
        int source_rank = 1;
        MPI_Recv(&sum_recv, sizeof(int), MPI_INT, source_rank, tag, MPI_COMM_WORLD, &status);
        printf("Process %d received a sum %d from process %d.\n", rank, sum_recv, source_rank);

    } else if (rank == 1) {
        int* array_recv;
        int source_rank = 0;
        int received_count;

        // Receive the size of the incoming array
        MPI_Probe(source_rank, tag, MPI_COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_INT, &received_count);

        // Allocate memory for the dynamic array based on the received size
        array_recv = (int*)malloc(received_count * sizeof(int));
        if (array_recv == NULL) {
            fprintf(stderr, "Memory allocation failed.\n");
            MPI_Finalize();
            return 1;
        }

        MPI_Recv(array_recv, received_count, MPI_INT, source_rank, tag, MPI_COMM_WORLD, &status);

        printf("Process %d received an array of size %d from process %d.\n", rank, received_count, source_rank);

        int sum = 0;
        for(int i=0; i<received_count; i++){
            sum += array_recv[i];
        }
        printf("Process %d calculated Sum %d\n", rank, sum);

        // Check status for more information about the received message
        printf("Source: %d\n", status.MPI_SOURCE);
        printf("Tag: %d\n", status.MPI_TAG);

        // Free the dynamically allocated memory
        free(array_recv);


    }

    MPI_Finalize();

    return 0;
}
