cmake_minimum_required(VERSION 3.26)
project(SistemasOperativosp C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(P2
        autores.c
        autores.h
        definicionsBase.h
        entradas.c
        entradas.h
        ficheiros.c
        ficheiros.h
        funcionsTemporais.c
        funcionsTemporais.h
        help.c
        help.h
        historial.c
        historial.h
        p2.c
        procesos.c
        procesos.h)
