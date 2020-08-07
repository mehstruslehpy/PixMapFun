main: shape 
	./shape 1200 1200 2048    0 3 1    0 0 1    squares_model.txt squares_model.ppm
	#./process_each_model.sh 1200 1200 2048    0 3 1    0 0 1
	viewnior simple_model.ppm
shape: matrix vector polygon buffer wireframe file shape.c 
	gcc -g -Wall -pedantic -lm -c shape.c -o shape.o
	gcc -g -Wall -pedantic -lm shape.o buffer.o matrix.o vector.o polygon.o wireframe.o file.o -o shape
buffer: buffer.c buffer.h
	gcc -g -Wall -pedantic -lm -c buffer.c -o buffer.o
matrix: matrix.c matrix.h
	gcc -g -Wall -pedantic -lm -c matrix.c -o matrix.o
vector: vector.c vector.h
	gcc -g -Wall -pedantic -lm -c vector.c -o vector.o
polygon: polygon.c polygon.h
	gcc -g -Wall -pedantic -lm -c polygon.c -o polygon.o
wireframe: wireframe.c wireframe.h
	gcc -g -Wall -pedantic -lm -c wireframe.c -o wireframe.o
file: file.c file.h
	gcc -g -Wall -pedantic -lm -c file.c -o file.o
clean:
	rm *.o
	rm *.ppm
	rm *.gif
	rm shape
