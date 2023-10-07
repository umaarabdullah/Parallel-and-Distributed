#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_SPAWNS 3
#define NUM_SAMPLES 200000000 // Number of random samples for the Monte Carlo simulation

int main(int argc, char* argv[]) {
    int np = NUM_SPAWNS;
    int errcodes[NUM_SPAWNS];
    int global_inside_circle_count = 0;
    double start_time, end_time; // Variables to store start and end times
    MPI_Comm parentcomm, intercomm;

    MPI_Init(&argc, &argv);
    MPI_Comm_get_parent(&parentcomm);

    // Record the start time
    start_time = MPI_Wtime();
    
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
        
        // Sum the results related to pi calculation from all spawned processes using MPI_Reduce
        MPI_Reduce(&local_inside_circle_count, &global_inside_circle_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
        
        // Calculate the approximation of pi in the root process
        int rank;
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);

        end_time = MPI_Wtime();

        // Calculate and print the total execution time for this process
        double total_execution_time = end_time - start_time;
        printf("Total execution time by Process %d: %lf seconds\n", rank, total_execution_time);

        // Use MPI_Allreduce to find the maximum execution time among all processes
        double max_execution_time;
        /* The total parallel execution time (T_total) would be the maximum time among all processes because parallel processes run concurrently, and the total execution time is determined by the slowest process. */
        MPI_Allreduce(&total_execution_time, &max_execution_time, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);          
        
        if (rank == 0) {
            double pi_approximation = (4.0 * global_inside_circle_count) / (NUM_SAMPLES * NUM_SPAWNS);
            printf("Master process: Approximate value of pi: %lf\n", pi_approximation);
            printf("Total parallel execution time (Tp): %lf seconds\n", max_execution_time);
        }
    }

    fflush(stdout);
    MPI_Finalize();
    return 0;
}
