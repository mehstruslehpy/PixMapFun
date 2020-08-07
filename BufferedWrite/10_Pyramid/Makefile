main: pyramid.c cube.c
	#gdb --args ./pyramid 5 view1.ppm
	./pyramid 1000 -1000 1000 100 200 500 500 500 500 view_pyramid1.ppm
	#./cube 1000 -1000 1000 1 1 10 1000 1000 1000 view_cube1.ppm
	./cube 1000 -200 -500 -10 -10 20 1000 1000 1000 view_cube1.ppm
	#viewnior view_cube1.ppm
pyramid.c: buffer pyramid.c 
	gcc -g -Wall -pedantic -lm -c pyramid.c -o pyramid.o
	gcc -g -Wall -pedantic -lm pyramid.o buffer.o matrix.o -o pyramid
cube.c: buffer cube.c 
	gcc -g -Wall -pedantic -lm -c cube.c -o cube.o
	gcc -g -Wall -pedantic -lm cube.o buffer.o matrix.o -o cube
buffer: matrix buffer.c buffer.h
	gcc -g -Wall -pedantic -lm -c buffer.c -o buffer.o
matrix: matrix.c matrix.h
	gcc -g -Wall -pedantic -lm -c matrix.c -o matrix.o
clean:
	rm pyramid
	rm *.o
	rm *.ppm

