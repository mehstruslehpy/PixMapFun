main: cube pyramid
	#gdb --args ./pyramid 5 view1.ppm
	#./cube 1000 -200 -500 -10 -10 20 1000 1000 1000 view_cube1.ppm 
	./cube 1200 1200 2048    0 3 1           0 0 1 view_cube1.ppm
	./cube 1000 2048 2048    0 3 1           0 0 1 view_cube2.ppm
	./cube 1000 2048 2048   -0.01 2.2 0.40   0 0 1 view_cube3.ppm
	./cube 1000 2048 2048    0 -1 -3         0 -1 0 view_cube4.ppm
	./cube 1000 900 900      0 0 1           1 0 0 view_cube5.ppm
	./pyramid 1200 1200 2048    0 3 1           0 0 1 view_pyramid1.ppm
	./pyramid 1000 2048 2048    0 3 1           0 0 1 view_pyramid2.ppm
	./pyramid 1000 2048 2048   -0.01 2.2 0.40   0 0 1 view_pyramid3.ppm
	./pyramid 1000 2048 2048    0 -1 -3         0 -1 0 view_pyramid4.ppm
	./pyramid 1000 900 900      0 0 1           1 0 0 view_pyramid5.ppm
	viewnior view_cube3.ppm
cube: matrix vector buffer cube.c 
	gcc -g -Wall -pedantic -lm -c cube.c -o cube.o
	gcc -g -Wall -pedantic -lm cube.o buffer.o matrix.o vector.o -o cube
pyramid: matrix vector buffer pyramid.c 
	gcc -g -Wall -pedantic -lm -c pyramid.c -o pyramid.o
	gcc -g -Wall -pedantic -lm pyramid.o buffer.o matrix.o vector.o -o pyramid
buffer: buffer.c buffer.h
	gcc -g -Wall -pedantic -lm -c buffer.c -o buffer.o
matrix: matrix.c matrix.h
	gcc -g -Wall -pedantic -lm -c matrix.c -o matrix.o
vector: vector.c vector.h
	gcc -g -Wall -pedantic -lm -c vector.c -o vector.o
clean:
	rm *.o
	rm *.ppm
	rm cube
	rm pyramid
