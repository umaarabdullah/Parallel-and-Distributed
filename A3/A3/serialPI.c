#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    int npoints = 20000000;
    int circle_count = 0;
    double x, y;

    clock_t start, end;
    double cpu_time_used;

    start = clock();

    // Seed the random number generator with the current time
    srand(time(NULL));

    for (int j = 0; j < npoints; j++) {
        // Generate random numbers between 0 and 1
        x = (double)rand() / RAND_MAX;
        y = (double)rand() / RAND_MAX;

        // Check if the point (x, y) is inside the unit circle
        if (x * x + y * y <= 1.0) {
            circle_count++;
        }
    }

    // Calculate the approximate value of pi
    double pi = 4.0 * circle_count / npoints;

    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;

    // Print the result
    printf("Approximate value of pi: %lf\n", pi);

    printf("CPU Time Used: %lf seconds\n", cpu_time_used);

    // write execution time in exe_time.txt file
    FILE *exeTimeFile;
    char exeFilename[] = "Output/serialTime.txt";
    exeTimeFile = fopen(exeFilename, "w");
    if (exeTimeFile == NULL) {
        fprintf(stderr, "Could not open file %s\n", exeFilename);
        exit(1);
    }
    fprintf(exeTimeFile, "%lf\n", cpu_time_used);
    fclose(exeTimeFile);

    return 0;
}
