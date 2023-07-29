@echo off

rem Execute the run_server target in a new terminal window
start "server" cmd /k cd /D "Client_Server_Parallel" ^& make run_server

rem Add a delay to ensure the server has started before running clients (adjust as needed)
timeout /t 5

rem Execute the run_test_client1 target in a new terminal window
start "client_test_1" cmd /k cd /D "Client_Server_Parallel" ^& make run_client1_test_1

rem Execute the run_client2 target in a new terminal window
start "client_test_2" cmd /k cd /D "Client_Server_Parallel" ^& make run_client2_test_1

rem Execute the run_client3 target in a new terminal window
start "client_test_3" cmd /k cd /D "Client_Server_Parallel" ^& make run_client3_test_1

