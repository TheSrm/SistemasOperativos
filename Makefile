p0.out: p0.c entradas.o
	gcc -o p0.out p0.c entradas.o funcionsTemporais.o historial.o autores.o procesos.o ficheiros.o help.o
entradas.o: entradas.c entradas.h funcionsTemporais.o historial.o autores.o procesos.o ficheiros.o help.o
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
help.o: help.c help.h
	gcc -c help.o help.c
limpar:
	rm p0.out
	rm *.o
proba: p0.out
	./p0.out
