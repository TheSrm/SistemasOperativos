p2.out: p2.c entradas.o
	gcc -o p2.out p2.c entradas.o funcionsTemporais.o historial.o autores.o procesos.o ficheiros.o memoria.o help.o
entradas.o: entradas.c entradas.h funcionsTemporais.o historial.o autores.o procesos.o ficheiros.o memoria.o help.o
	gcc -c entradas.o entradas.c
funcionsTemporais.o: funcionsTemporais.c funcionsTemporais.h
	gcc -c funcionsTemporais.o funcionsTemporais.c
historial.o: historial.c historial.h definicionsBase.h
	gcc -c historial.o historial.c
autores.o: autores.c autores.h definicionsBase.h
	gcc -c autores.o autores.c
procesos.o: procesos.c procesos.h definicionsBase.h
	gcc -c procesos.o procesos.c
ficheiros.o: ficheiros.c ficheiros.h
	gcc -c ficheiros.o ficheiros.c
memoria.o: memoria.c memoria.h
	gcc -c memoria.o memoria.c
help.o: help.c help.h
	gcc -c help.o help.c
limpar:
	rm p2.out
	rm *.o
proba: p2.out
	./p2.out
