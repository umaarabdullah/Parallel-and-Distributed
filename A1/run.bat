@echo off

rem The batch file must comment or uncomment a few line on the server and client program.


rem Execute the run_server target in a new terminal window
@REM start cmd /k cd /D "Client_Server_Parallel" ^& make run_server

rem Add a delay to ensure the server has started before running clients (adjust as needed)
@REM timeout /t 5

rem Execute the run_client1 target in a new terminal window
start cmd /k cd /D "Client_Server_Parallel" ^& make run_client1

rem Execute the run_client2 target in a new terminal window
start cmd /k cd /D "Client_Server_Parallel" ^& make run_client2

rem Execute the run_client3 target in a new terminal window
start cmd /k cd /D "Client_Server_Parallel" ^& make run_client3
