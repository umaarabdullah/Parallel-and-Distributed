# Parallel-and-Distributed

## Required Packages

### make for windows and add path to environment variable

### if sqlite.dll causes problems then change rule in Makefile

    server: server.c
    $(CC) $(CFLAGS) -o server server.c sqlite/sqlite3.c $(LDFLAGS)
    
    or
    
    $(CC) $(CFLAGS) -o server server.c $(LDFLAGS) -L"C:\Program Files (x86)\sqlite" -lsqlite3

### sqlite windows precomplied binary & sqlite tools & sqlite amalgamation source files from the sqlite website.

### copy sqlite from the requirements folder and paste it to C:\Program Files (x86)\sqlite

## Binding Ip-Address
###  Can find server device's Ip Address by using ipconfig command

