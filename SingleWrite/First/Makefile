OUTFILE=testimage
all: main 
	#gdb --args ./main $(OUTFILE).ppm 0 simpleline 
	#./main $(OUTFILE).ppm 0 simpleline 
	./main $(OUTFILE).ppm 50 mat_func 
	viewnior $(OUTFILE).ppm
	#rm main

main: main.c 
	gcc -g -Wall -pedantic -fno-stack-protector -lm main.c -o main
