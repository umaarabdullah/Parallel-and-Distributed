@echo off

start "server" cmd /k cd /D "Client_Server_Parallel" ^& make run_server

start "client1_test_1" cmd /k cd /D "Client_Server_Parallel/tests" ^& make run_client1_test_1
start "client2_test_1" cmd /k cd /D "Client_Server_Parallel/tests" ^& make run_client2_test_1
start "client3_test_1" cmd /k cd /D "Client_Server_Parallel/tests" ^& make run_client3_test_1

echo "Test 1 Completed"