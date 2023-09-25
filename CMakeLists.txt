cmake_minimum_required(VERSION 3.26)
project(SHELL C)

set(CMAKE_C_STANDARD 99)

add_executable(SHELL main.c
        funcionsTemporais.c
        funcionsTemporais.h
        entradas.c
        entradas.h
        historial.c
        historial.h
        autores.c
        autores.h
        procesos.c
        procesos.h
        definicionsBase.h
        ficheiros.c
        ficheiros.h
)
