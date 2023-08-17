CC = gcc
CFLAGS = -Wall
LDFLAGS = -lws2_32

all: run_server run_client1 run_client2 run_client3

server: server.c
	$(CC) $(CFLAGS) -o server server.c $(LDFLAGS) -L"C:\Program Files (x86)\sqlite" -lsqlite3

run_server: server
	./server

client1: client.c
	$(CC) $(CFLAGS) -o client1 client.c $(LDFLAGS)

run_client1: client1
	./client1

client2: client.c
	$(CC) $(CFLAGS) -o client2 client.c $(LDFLAGS)

run_client2: client2
	./client2

client3: client.c
	$(CC) $(CFLAGS) -o client3 client.c $(LDFLAGS)

run_client3: client3
	./client3

clean:
	del server.exe client1.exe client2.exe client3.exe
