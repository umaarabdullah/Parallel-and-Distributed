#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

int main(int argc, char* argv[]) {
    
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    MPI_Comm parent;
    MPI_Comm_get_parent(&parent);

    // Check if the process was spawned dynamically
    if (parent == MPI_COMM_NULL) {
        fprintf(stderr, "Error: This program should be spawned by the master process.\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    int npoints;
    int local_circle_count = 0;
    double x, y;

    // Measure the start time
    double start_time = MPI_Wtime();

    // Receive the broadcasted number from the master
    MPI_Recv(&npoints, 1, MPI_INT, 0, 0, parent, MPI_STATUS_IGNORE);

    // Seed the random number generator with a unique seed for each process
    srand(time(NULL) + rank);

    // Do slave operation
    // Generate and count random points
    for (int j = 0; j < npoints; j++) {
        x = (double)rand() / RAND_MAX;
        y = (double)rand() / RAND_MAX;

        if (x * x + y * y <= 1.0) {
            local_circle_count++;
        }
    }

    // Send the rank and received number back to the master
    MPI_Send(&local_circle_count, 1, MPI_INT, 0, 0, parent);

    // Measure the end time
    double end_time = MPI_Wtime();

    double cpu_time_used = end_time - start_time;
    printf("Execution Time worker %d: %lf seconds\n", rank, cpu_time_used);

    // printf("Worker %d received number from master: %d\n", rank, npoints);

    // Disconnect from the parent communicator
    MPI_Comm_disconnect(&parent);

    MPI_Finalize();

    return 0;
}
