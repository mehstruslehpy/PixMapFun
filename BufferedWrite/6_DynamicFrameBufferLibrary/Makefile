main: lines 
	#gdb --args ./lines 0 0 800 800
	#./lines 200 200 800 800| tee log.txt
	#valgrind ./lines 200 200 800 800
	./lines 200 200 800 800
	viewnior lines.ppm
lines: buffer lines.c 
	gcc -g -Wall -pedantic -lm -c lines.c -o lines.o
	gcc -g -Wall -pedantic -lm lines.o buffer.o -o lines
buffer: buffer.c buffer.h
	gcc -g -Wall -pedantic -lm -c buffer.c -o buffer.o
clean:
	rm lines.o
	rm lines
	rm buffer.o
	rm *.ppm
	rm log.txt
