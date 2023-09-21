@echo off

rem Test if how the system recovers when 2nd response from client is wrong

start "server" cmd /k cd /D "Client_Server_Parallel" ^& make run_server

timeout /t 5

start "client1_test_3" cmd /k cd /D "Client_Server_Parallel/tests" ^& make run_client1_test_3
start "client2_test_3" cmd /k cd /D "Client_Server_Parallel/tests" ^& make run_client2_test_3
start "client3_test_3" cmd /k cd /D "Client_Server_Parallel/tests" ^& make run_client3_test_3

echo "Test 3 Completed"