main: triangles.c
	./triangles 5 view1.ppm
	viewnior view1.ppm
triangles.c: buffer triangles.c 
	gcc -g -Wall -pedantic -lm -c triangles.c -o triangles.o
	gcc -g -Wall -pedantic -lm triangles.o buffer.o matrix.o -o triangles
buffer: matrix buffer.c buffer.h
	gcc -g -Wall -pedantic -lm -c buffer.c -o buffer.o
matrix: matrix.c matrix.h
	gcc -g -Wall -pedantic -lm -c matrix.c -o matrix.o
clean:
	rm triangles
	rm *.o
	rm *.ppm

