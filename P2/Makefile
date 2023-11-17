p2: p2.c entradas.o
	gcc -g -o p2 p2.c entradas.o funcionsTemporais.o historial.o autores.o procesos.o ficheiros.o help.o memoria.o
entradas.o: entradas.c entradas.h funcionsTemporais.o historial.o autores.o procesos.o ficheiros.o help.o memoria.o
	gcc -g -c entradas.o entradas.c
funcionsTemporais.o: funcionsTemporais.c funcionsTemporais.h
	gcc -g -c funcionsTemporais.o funcionsTemporais.c
historial.o: historial.c historial.h definicionsBase.h
	gcc -g -c historial.o historial.c
autores.o: autores.c autores.h definicionsBase.h
	gcc -g -c autores.o autores.c
procesos.o: procesos.c procesos.h definicionsBase.h
	gcc -g -c procesos.o procesos.c
ficheiros.o: ficheiros.c ficheiros.h
	gcc -g -c ficheiros.o ficheiros.c
memoria.o: memoria.c memoria.h
	gcc -g -c memoria.o memoria.c
help.o: help.c help.h
	gcc -g -c help.o help.c
valgrind:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose ./p2