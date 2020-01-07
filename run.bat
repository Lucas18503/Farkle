@echo off
cd src
gcc -o ../%1.exe %1.c combination.c turn.c utils.c
cd ..
%1.exe
