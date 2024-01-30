#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

int main(int argc, char* argv[]) {

    int rank, num_procs;
    int npoints = 20000000;
    int local_circle_count = 0;
    int global_circle_count;
    double x, y;
    double local_pi, global_pi;
    double start_time, end_time;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    // Seed the random number generator with a unique seed for each process
    srand(time(NULL) + rank);

    // Calculate the number of points for each process
    int points_per_process = npoints / num_procs;

    // Measure the start time
    start_time = MPI_Wtime();

    // Generate and count random points
    for (int j = 0; j < points_per_process; j++) {
        x = (double)rand() / RAND_MAX;
        y = (double)rand() / RAND_MAX;

        if (x * x + y * y <= 1.0) {
            local_circle_count++;
        }
    }

    // Sum the results from all processes using MPI_Allreduce
    MPI_Allreduce(&local_circle_count, &global_circle_count, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    // Measure the end time
    end_time = MPI_Wtime();

    // Process 0 calculates the final π approximation
    if (rank == 0) {
        global_pi = 4.0 * global_circle_count / npoints;    // Calculate the final π approximation
        double cpu_time_used = end_time - start_time;
        printf("Approximate value of pi: %lf\n", global_pi);
        printf("Execution Time with %d workers: %lf seconds\n", num_procs - 1, cpu_time_used);

        // write execution time to exe_time.txt file
        FILE *exeTimeFile;
        char exeFilename[] = "Output/parallelTime.txt";
        exeTimeFile = fopen(exeFilename, "a");      // append
        if (exeTimeFile == NULL) {
            fprintf(stderr, "Could not open file %s\n", exeFilename);
            exit(1);
        }
        fprintf(exeTimeFile, "%d ", num_procs - 1);
        fprintf(exeTimeFile, "%lf\n", cpu_time_used);
        fclose(exeTimeFile);
    }

    MPI_Finalize();

    return 0;
}
