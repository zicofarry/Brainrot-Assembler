#!/bin/bash

# Kompilasi menggunakan gcc
gcc brainrot-interpreter.c lib/logics/lexer/*.c lib/logics/parser/*.c -o bin/a.out

# Eksekusi file yang sudah dikompilasi
./bin/a.out "$1"
