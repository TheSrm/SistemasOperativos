shell.out: shell.c entradas.o
	gcc -o shell.out shell.c
entradas.o: temporais.o historial.o
	gcc -c entradas.o entradas.c
temporais.o: funcionsTemporais.c funcionsTemporais.h
	gcc -c funcionsTemporais.o funcionsTemporais.c
historial.o: historial.c historial.h
	gcc -c historial.o historial.c
limpar:
	rm shell.out
proba: shell.out
	./shell.out