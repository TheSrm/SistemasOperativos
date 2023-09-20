cmake_minimum_required(VERSION 3.26)
project(SHELL C)

set(CMAKE_C_STANDARD 99)

add_executable(SHELL main.c
        funcionsTemporais.c
        funcionsTemporais.h
        entradas.c
        entradas.h)
