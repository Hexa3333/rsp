@echo off

call vcvars64.bat
call cl /c src\rsp.cpp
call cl example.cpp rsp.obj
