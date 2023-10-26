cmake_minimum_required(VERSION 3.26)
project(SistemasOperativos1 C)

set(CMAKE_C_STANDARD 11)

include_directories(P1)

add_executable(SistemasOperativos1
        P1/autores.c
        P1/autores.h
        P1/definicionsBase.h
        P1/entradas.c
        P1/entradas.h
        P1/ficheiros.c
        P1/ficheiros.h
        P1/funcionsTemporais.c
        P1/funcionsTemporais.h
        P1/help.c
        P1/help.h
        P1/historial.c
        P1/historial.h
        P1/p1.c
        P1/procesos.c
        P1/procesos.h)
