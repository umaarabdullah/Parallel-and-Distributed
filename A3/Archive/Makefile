all : build run

build : dijkstra_parallel.c
	mpicc -o dijkstra_parallel dijkstra_parallel.c
run : build
	mpirun -np 2 ./dijkstra_parallel

build_hello: hello_mpi.c
	mpicc -o hello_mpi hello_mpi.c
run_hello: build_hello
	mpirun --oversubscribe -np 10 ./hello_mpi

build_send_recv: send_recv_mpi.c
	mpicc -o send_recv_mpi send_recv_mpi.c
run_send_recv: build_send_recv
	mpirun -np 2 ./send_recv_mpi

build_send_recv_dynamic: send_recv_mpi_dynamic_arr.c
	mpicc -o send_recv_mpi_dynamic_arr send_recv_mpi_dynamic_arr.c
run_send_recv_dynamic: build_send_recv_dynamic
	mpirun -np 2 ./send_recv_mpi_dynamic_arr
