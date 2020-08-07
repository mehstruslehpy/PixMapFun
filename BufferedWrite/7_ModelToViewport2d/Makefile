main: patchxform vertexxform
	#./patchxform 200 200 800 800| tee log.txt
	./vertexxform 0 0 2048 2048 view1.ppm
	./vertexxform 400 400 1548 1548 view2.ppm
	./vertexxform 0 0 2048 2048 view3.ppm
	viewnior view1.ppm
patchxform: buffer patchxform.c 
	gcc -g -Wall -pedantic -lm -c patchxform.c -o patchxform.o
	gcc -g -Wall -pedantic -lm patchxform.o buffer.o -o patchxform
vertexxform: buffer vertexxform.c 
	gcc -g -Wall -pedantic -lm -c vertexxform.c -o vertexxform.o
	gcc -g -Wall -pedantic -lm vertexxform.o buffer.o -o vertexxform
buffer: buffer.c buffer.h
	gcc -g -Wall -pedantic -lm -c buffer.c -o buffer.o
clean:
	rm patchxform.o
	rm patchxform
	rm buffer.o
	rm *.ppm
	rm log.txt
