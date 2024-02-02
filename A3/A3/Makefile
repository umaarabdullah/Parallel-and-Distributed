CC = gcc
CFLAGS = -Wall -O2
LDFLAGS =

all: serialpiBuild parallelpiBuild workerBuild parallelspawnBuild

serialpiBuild: serialPI.c
	$(CC) $(CFLAGS) -o serialPI serialPI.c $(LDFLAGS)

serialpiRun: serialpiBuild
	./serialPI

parallelpiBuild: parallelPI.c
	mpicc -o parallelPI parallelPI.c

parallelpiRun: parallelpiBuild
	mpirun  --oversubscribe -np 12 ./parallelPI

parallelspawnBuild: parallelspawnPI.c
	mpicc -o parallelspawnPI parallelspawnPI.c

workerBuild: worker.c
	mpicc -o worker worker.c

parallelspawnRun: parallelspawnBuild workerBuild
	mpirun --oversubscribe -np 4 ./parallelspawnPI

clean:
	rm -f *.o


