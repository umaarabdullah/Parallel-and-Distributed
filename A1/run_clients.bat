@echo off

rem Execute the run_server target in a new terminal window
@REM start cmd /k cd /D "Client_Server_Parallel" ^& make run_server

rem Execute the run_client1 target in a new terminal window
start cmd /k cd /D "Client_Server_Parallel" ^& make run_client1

rem Execute the run_client2 target in a new terminal window
start cmd /k cd /D "Client_Server_Parallel" ^& make run_client2

rem Execute the run_client3 target in a new terminal window
start cmd /k cd /D "Client_Server_Parallel" ^& make run_client3
