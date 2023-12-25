CC = gcc
CFLAGS = -Wall -O2
LDFLAGS =

TARGET = serial_pi_calc
SRC = serial_pi_calc.c

all: $(TARGET)

build_serial_pi: $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

run_serial_pi: build_serial_pi
	./serial_pi_calc

build_parallel_pi: parallel_pi_calc.c
	mpicc -o parallel_pi_calc parallel_pi_calc.c

run_parallel_pi: build_parallel_pi
	mpirun  --oversubscribe -np 12 ./parallel_pi_calc

build_parallel_pi_spawn: parallel_pi_calc_master.c
	mpicc parallel_pi_calc_master.c -o master

run_parallel_pi_spawn: build_parallel_pi_spawn
	mpirun --oversubscribe -np 1 ./master

clean:
	rm -f *.o $(TARGET)


