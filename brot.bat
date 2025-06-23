@echo off
gcc brainrot-interpreter.c lib/logics/lexer/*.c lib/logics/parser/*.c -o bin/a.exe && .\\bin\\a.exe %1
