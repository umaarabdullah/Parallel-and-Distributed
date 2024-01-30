#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char* argv[]) {
    
    int rank, num_procs;
    int npoints = 20000000;        // Master generates a number
    int global_circle_count = 0;
    double global_pi;
    double start_time, end_time;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    MPI_Comm inter_comm;
    MPI_Info spawn_info;

    MPI_Info_create(&spawn_info);
    MPI_Info_set(spawn_info, "host", "localhost");

    // Measure the start time
    start_time = MPI_Wtime();
    
    // Spawn worker processes dynamically
    MPI_Comm_spawn("./worker", MPI_ARGV_NULL, num_procs, spawn_info, 0, MPI_COMM_SELF, &inter_comm, MPI_ERRCODES_IGNORE);

    // Broadcast npoints to all worker processes
    for (int i = 0; i < num_procs; i++) {
        MPI_Send(&npoints, 1, MPI_INT, i, 0, inter_comm);
    }

    // Receive and sum the local counts from all worker processes
    for (int i = 0; i < num_procs - 1; i++) {
        int local_circle_count = 0;
        MPI_Recv(&local_circle_count, 1, MPI_INT, MPI_ANY_SOURCE, 0, inter_comm, MPI_STATUS_IGNORE);
        global_circle_count += local_circle_count;
    }

    // Measure the end time
    end_time = MPI_Wtime();

    // Execute printf only in the master process
    if (rank == 0) {
        global_pi = 4.0 * global_circle_count / (npoints * (num_procs - 1));    // Calculate the final π approximation
        double cpu_time_used = end_time - start_time;
        printf("Approximate value of pi: %lf\n", global_pi);
        printf("Execution Time with %d workers: %lf seconds\n", num_procs - 1, cpu_time_used);

        // write execution time to exe_time.txt file
        FILE *exeTimeFile;
        char exeFilename[] = "Output/parallelspawnTime.txt";
        exeTimeFile = fopen(exeFilename, "a");      // append
        if (exeTimeFile == NULL) {
            fprintf(stderr, "Could not open file %s\n", exeFilename);
            exit(1);
        }
        fprintf(exeTimeFile, "%d ", num_procs - 1);
        fprintf(exeTimeFile, "%lf\n", cpu_time_used);
        fclose(exeTimeFile);
    }

    // Disconnect from the worker processes
    MPI_Comm_disconnect(&inter_comm);

    MPI_Finalize();

    return 0;
}
