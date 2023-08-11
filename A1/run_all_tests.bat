@echo off

@REM @REM rem Execute the run_server target in a new terminal window
start "server" cmd /k cd /D "Client_Server_Parallel" ^& make run_server

@REM @REM rem Add a delay to ensure the server has started before running clients (adjust as needed)
timeout /t 5

start "client1_test_1" cmd /k cd /D "Client_Server_Parallel/tests" ^& make run_client1_test_1
start "client2_test_1" cmd /k cd /D "Client_Server_Parallel/tests" ^& make run_client2_test_1
start "client3_test_1" cmd /k cd /D "Client_Server_Parallel/tests" ^& make run_client3_test_1

timeout /t 5

echo "Test 1 Completed"

rem Execute the run_server target in a new terminal window
start "server" cmd /k cd /D "Client_Server_Parallel" ^& make run_server

timeout /t 5

start "client1_test_2" cmd /k cd /D "Client_Server_Parallel/tests" ^& make run_client1_test_2
start "client2_test_2" cmd /k cd /D "Client_Server_Parallel/tests" ^& make run_client2_test_2
start "client3_test_2" cmd /k cd /D "Client_Server_Parallel/tests" ^& make run_client3_test_2

echo "Test 2 Completed"

timeout /t 5

rem Execute the run_server target in a new terminal window
start "server" cmd /k cd /D "Client_Server_Parallel" ^& make run_server

timeout /t 5

start "client1_test_3" cmd /k cd /D "Client_Server_Parallel/tests" ^& make run_client1_test_3
start "client2_test_3" cmd /k cd /D "Client_Server_Parallel/tests" ^& make run_client2_test_3
start "client3_test_3" cmd /k cd /D "Client_Server_Parallel/tests" ^& make run_client3_test_3

