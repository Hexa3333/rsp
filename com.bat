@echo off
SET SRC=src\main.cpp

mkdir build

call vcvars64.bat
call cl %SRC%^
 -Zi^
 -Fe:build\DiscordBot.exe^
 -Fo:build\^
 -Fd:build\ 
