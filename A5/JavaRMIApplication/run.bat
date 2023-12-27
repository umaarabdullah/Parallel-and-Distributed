@echo off
REM Compile and execute Makefile for RMIServer
start cmd /k cd /D "RMI_Server" ^& echo Compiling RMIServer... ^& make build ^& echo Running RMIServer... ^& make run

@echo off
timeout /t 5 >nul

@echo off
REM Compile and execute Makefile for RMIClient
start cmd /k cd /D "RMI_Client" ^& echo Compiling RMIClients... ^& make build ^& echo Running RMIClient... ^& make run1

REM Compile and execute Makefile for RMIClient1
start cmd /k cd /D "RMI_Client" ^& echo Running RMIClient1... ^& make run2
