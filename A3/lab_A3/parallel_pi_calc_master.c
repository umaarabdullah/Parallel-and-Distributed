#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_SPAWNS 2
#define NUM_SAMPLES 10000 // Number of random samples for the Monte Carlo simulation

int main(int argc, char* argv[]) {
    int np = NUM_SPAWNS;
    int errcodes[NUM_SPAWNS];
    MPI_Comm parentcomm, intercomm;

    MPI_Init(&argc, &argv);
    MPI_Comm_get_parent(&parentcomm);
    
    if (parentcomm == MPI_COMM_NULL) {
        /* Create n more processes - this executable must be called master.exe for this to work. */
        MPI_Comm_spawn("master", MPI_ARGV_NULL, np, MPI_INFO_NULL, 0, MPI_COMM_WORLD, &intercomm, errcodes);
        printf("I'm the parent.\n");
    } else {
        printf("I'm the spawned.\n");
        // Initialize random number generator with a unique seed for each process
        srand(time(NULL) + MPI_Wtime());

        int local_inside_circle_count = 0;
        
        // Perform the Monte Carlo simulation within each spawned process
        for (int i = 0; i < NUM_SAMPLES; i++) {
            double x = (double)rand() / RAND_MAX;
            double y = (double)rand() / RAND_MAX;

            if (x * x + y * y <= 1.0) {
                local_inside_circle_count++;
            }
        }

        // Sum the results from all spawned processes using MPI_Reduce
        int global_inside_circle_count = 0;
        MPI_Reduce(&local_inside_circle_count, &global_inside_circle_count, 1, MPI_INT, MPI_SUM, 0, intercomm);

        // Calculate the approximation of pi
        int rank;
        MPI_Comm_rank(intercomm, &rank); // Get the rank of the current process

        if (rank != 0) { // Check if the current process is not the root process (rank 0)
            double pi_approximation = (4.0 * global_inside_circle_count) / (NUM_SAMPLES * NUM_SPAWNS);
            printf("Process %d: Approximate value of pi: %f\n", rank, pi_approximation);
        }
        
    }

    fflush(stdout);
    MPI_Finalize();
    return 0;
}
