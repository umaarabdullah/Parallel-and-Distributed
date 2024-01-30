import numpy as np
import matplotlib.pyplot as plt

def read_serial_execution_time(filename):
    with open(filename, 'r') as file:
        line = file.readline()
        return float(line)

def read_parallel_execution_times(filename):
    with open(filename, 'r') as file:
        lines = file.readlines()
        return [(int(line.split()[0]), float(line.split()[1])) for line in lines]

def calculate_speedup(serial_time, parallel_time):
    return serial_time / parallel_time

def plot_speedup_graph(serial_times, parallel_times, parallelspawn_times):
    
    num_procs_parallel = [t[0] for t in parallel_times]
    num_procs_parallelspawn = [t[0] for t in parallelspawn_times]

    parallel_exe_times = [t[1] for t in parallel_times]
    parallelspawn_exe_times = [t[1] for t in parallelspawn_times]

    # Calculate speedups
    parallel_speedups = []
    parallelspawn_speedups = []

    for value1, value2 in zip(parallel_exe_times, parallelspawn_exe_times):
        parallel_speedups.append(calculate_speedup(serial_times, value1))
        parallelspawn_speedups.append(calculate_speedup(serial_times, value2))

    plt.plot(num_procs_parallel, parallel_speedups, label='Parallel')
    plt.plot(num_procs_parallel, parallelspawn_speedups, label='Parallel Spawn')

    plt.xlabel('Number of Workers')
    plt.ylabel('Speedup')
    plt.title('Parallel and Parallel Spawn Speedup')
    plt.legend()
    plt.savefig('plot')
    plt.show()

def main():
    serial_time = read_serial_execution_time('Output/serialTime.txt')
    parallel_times = read_parallel_execution_times('Output/parallelTime.txt')
    parallelspawn_times = read_parallel_execution_times('Output/parallelspawnTime.txt')

    plot_speedup_graph(serial_time, parallel_times, parallelspawn_times)

if __name__ == "__main__":
    main()
