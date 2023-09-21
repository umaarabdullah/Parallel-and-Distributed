@echo off

rem Test if how the system recovers when 1st response from client is wrong

start "server" cmd /k cd /D "Client_Server_Parallel" ^& make run_server

timeout /t 5

start "client1_test_2" cmd /k cd /D "Client_Server_Parallel/tests" ^& make run_client1_test_2
start "client2_test_2" cmd /k cd /D "Client_Server_Parallel/tests" ^& make run_client2_test_2
start "client3_test_2" cmd /k cd /D "Client_Server_Parallel/tests" ^& make run_client3_test_2

echo "Test 2 Completed"
