main: matrixmultest matrixbyvec
	./matrixmultest 1 2 3 4     5 6 7 8
	./matrixbyvec 1 2 3 4     7 8
	./matrixmultest 2 4 3 4     1 2 1 3
	./matrixbyvec 2 4 3 4     1 2
matrixmultest: matrix matrixmultest.c 
	gcc -g -Wall -pedantic -lm -c matrixmultest.c -o matrixmultest.o
	gcc -g -Wall -pedantic -lm matrixmultest.o matrix.o -o matrixmultest
matrixbyvec: matrix matrixbyvec.c 
	gcc -g -Wall -pedantic -lm -c matrixbyvec.c -o matrixbyvec.o
	gcc -g -Wall -pedantic -lm matrixbyvec.o matrix.o -o matrixbyvec
matrix: ../matrix.c ../matrix.h
	gcc -g -Wall -pedantic -lm -c ../matrix.c -o matrix.o
clean:
	rm matrixmultest.o
	rm matrix.o
	rm matrixbyvec.o
	rm matrixmultest
	rm matrixbyvec
