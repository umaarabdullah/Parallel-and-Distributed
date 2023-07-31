@echo off

start "client1_test_2" cmd /k cd /D "Client_Server_Parallel/tests" ^& make run_client1_test_2
@REM start "client2_test_2" cmd /k cd /D "Client_Server_Parallel/tests" ^& make run_client2_test_2
@REM start "client3_test_2" cmd /k cd /D "Client_Server_Parallel/tests" ^& make run_client3_test_2

echo "Test 2 Completed"
