P_OPTION=modd2
INFILE1=GrumpyCat
INFILE2=NiceDog
all: main 
main: conv.c 
	gcc -g -Wall -pedantic -fno-stack-protector -lm conv.c -o main
	#gdb --args ./main $(INFILE1).ppm $(INFILE2).ppm $(P_OPTION).ppm $(P_OPTION)
	./main $(INFILE1).ppm $(INFILE2).ppm $(P_OPTION).ppm $(P_OPTION)
	viewnior $(P_OPTION).ppm
	#rm main
