#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <mpi.h>

#define V 5 // Number of vertices in the graph

void dijkstra(int graph[V][V], int src, int num_processes, int my_rank) {
    int dist[V]; // Shortest distance from src to each vertex
    int visited[V]; // True if vertex is visited

    // Initialize distance and visited arrays
    for (int i = 0; i < V; i++) {
        dist[i] = INT_MAX;
        visited[i] = 0;
    }

    dist[src] = 0; // Distance from source to itself is 0

    for (int count = 0; count < V - 1; count++) {
        int min_dist = INT_MAX;
        int u;

        // Find the vertex with the minimum distance that is not visited
        for (int v = 0; v < V; v++) {
            if (!visited[v] && dist[v] <= min_dist) {
                min_dist = dist[v];
                u = v;
            }
        }

        // Mark the selected vertex as visited
        visited[u] = 1;

        // Update dist[] for adjacent vertices in parallel
        for (int v = 0; v < V; v++) {
            if (!visited[v] && graph[u][v] && dist[u] != INT_MAX &&
                dist[u] + graph[u][v] < dist[v]) {
                dist[v] = dist[u] + graph[u][v];
            }
        }
    }

    // Print the result from each process (rank)
    for (int i = 0; i < num_processes; i++) {
        if (my_rank == i) {
            printf("Process %d: Shortest Path from Source %d:\n", my_rank, src);
            for (int j = 0; j < V; j++) {
                printf("Vertex %d: Distance %d\n", j, dist[j]);
            }
            printf("\n");
        }
        MPI_Barrier(MPI_COMM_WORLD); // Synchronize processes
    }
}

int main(int argc, char* argv[]) {
    int graph[V][V] = {
        {0, 10, 0, 0, 5},
        {0, 0, 1, 0, 2},
        {0, 0, 0, 4, 0},
        {7, 0, 6, 0, 0},
        {0, 0, 0, 0, 0}
    };

    int num_processes, my_rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    // Call Dijkstra's algorithm with each process
    for (int i = 0; i < V; i++) {
        dijkstra(graph, i, num_processes, my_rank);
    }

    MPI_Finalize();

    return 0;
}
