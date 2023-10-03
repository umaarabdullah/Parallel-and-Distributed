@echo off

rem Execute the run_server target in a new terminal window
start cmd /k cd /D "Client_Server_Parallel" ^& make run_server
