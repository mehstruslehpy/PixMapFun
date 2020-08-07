main: xformtest
	./run.sh
	./xformtest 3 3 3
	./xformtest 30 80 3
xformtest: buffer xformtest.c 
	gcc -g -Wall -pedantic -lm -c xformtest.c -o xformtest.o
	gcc -g -Wall -pedantic -lm xformtest.o buffer.o matrix.o -o xformtest
buffer: matrix ../buffer.c ../buffer.h
	gcc -g -Wall -pedantic -lm -c ../buffer.c -o buffer.o
matrix: ../matrix.c ../matrix.h
	gcc -g -Wall -pedantic -lm -c ../matrix.c -o matrix.o
clean:
	rm *.o
	rm xformtest
