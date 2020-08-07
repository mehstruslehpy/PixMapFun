main: shape 
	#valgrind ./shape 0     500 500 500     0 0 0     0 500 0 
	./shape 500 500 500     120 50 800     0 500 0    999 666 222     888 666 524
	viewnior polygon.ppm
shape: matrix vector polygon buffer shape.c 
	gcc -g -Wall -pedantic -lm -c shape.c -o shape.o
	gcc -g -Wall -pedantic -lm shape.o buffer.o matrix.o vector.o polygon.o -o shape
buffer: buffer.c buffer.h
	gcc -g -Wall -pedantic -lm -c buffer.c -o buffer.o
matrix: matrix.c matrix.h
	gcc -g -Wall -pedantic -lm -c matrix.c -o matrix.o
vector: vector.c vector.h
	gcc -g -Wall -pedantic -lm -c vector.c -o vector.o
polygon: polygon.c polygon.h
	gcc -g -Wall -pedantic -lm -c polygon.c -o polygon.o
clean:
	rm *.o
	rm *.ppm
	rm shape
