main: shape 
	./shape 2048 2048 2048    0 1 0    0 0 1    squares_model.wire squares_model.color squares_model.ppm
	./shape 2048 2048 2048    0 1 0    0 0 1    cube_model2.wire cube_model2.color cube_model2.ppm
	./shape 2048 2048 2048    0 1 0    0 0 1    cube_model1.wire cube_model1.color cube_model1.ppm
	./shape 2048 2048 2048    0 1 0    0 0 1    pyramid_model1.wire pyramid_model1.color pyramid_model1.ppm
	./shape 2048 2048 2048    0 1 0    0 0 1    pyramid_model2.wire pyramid_model2.color pyramid_model2.ppm
	./random_model.sh
	./shape 2048 2048 2048    0 1 0    0 0 1    random_model.wire random_model.color random_model.ppm
	viewnior cube_model1.ppm
shape: xform draw matrix vector polygon buffer wireframe file bsptree shape.c 
	gcc -g -Wall -pedantic -lm -c shape.c -o shape.o
	gcc -g -Wall -pedantic -lm shape.o buffer.o xform.o draw.o matrix.o vector.o polygon.o wireframe.o file.o bsptree.o -o shape
buffer: buffer.c buffer.h
	gcc -g -Wall -pedantic -lm -c buffer.c -o buffer.o
xform: xform.c xform.h
	gcc -g -Wall -pedantic -lm -c xform.c -o xform.o
draw: draw.c draw.h
	gcc -g -Wall -pedantic -lm -c draw.c -o draw.o
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
bsptree: bsptree.c bsptree.h
	gcc -g -Wall -pedantic -lm -c bsptree.c -o bsptree.o
clean:
	rm *.o
	rm *.ppm
	rm *.gif
	rm shape
