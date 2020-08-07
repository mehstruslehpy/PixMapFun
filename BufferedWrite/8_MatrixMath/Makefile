main: matrixmultest patchxform vertexxform
	#vertexxform uses the faster precomputed transformation and
	#faster copying by redrawing lines 
	./vertexxform 0 0 2048 2048 v_view1.ppm
	./vertexxform 400 400 1548 1548 v_view2.ppm
	./vertexxform 0 0 2048 2048 v_view3.ppm
	#patchxform uses the slower per point xform and
	#the slower copying by copying full patches of fbuf
	#./patchxform 0 0 2048 2048 p_view1.ppm
	#./patchxform 400 400 1548 1548 p_view2.ppm
	#./patchxform 0 0 2048 2048 p_view3.ppm
	viewnior v_view1.ppm
patchxform: buffer patchxform.c 
	gcc -g -Wall -pedantic -lm -c patchxform.c -o patchxform.o
	gcc -g -Wall -pedantic -lm patchxform.o buffer.o matrix.o -o patchxform
vertexxform: buffer vertexxform.c 
	gcc -g -Wall -pedantic -lm -c vertexxform.c -o vertexxform.o
	gcc -g -Wall -pedantic -lm vertexxform.o buffer.o matrix.o -o vertexxform
buffer: matrix buffer.c buffer.h
	gcc -g -Wall -pedantic -lm -c buffer.c -o buffer.o
matrix: matrix.c matrix.h
	gcc -g -Wall -pedantic -lm -c matrix.c -o matrix.o
clean:
	rm vertexxform 
	rm patchxform
	rm *.o
	rm *.ppm
	rm *.txt

