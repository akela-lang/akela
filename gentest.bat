@echo off
FOR /F "delims=\0 tokens=*" %%i IN (%1) DO @ECHO %%i
