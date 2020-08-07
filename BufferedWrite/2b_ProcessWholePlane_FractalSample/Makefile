all: multijulia multibrot julia mandelbrot 
	#gdb --args ./main 
	#./mandelbrot 0 1024 0 1024 255 | tee mandelbrotlog.txt
	#./julia 1 0 0 -0.7 0.27015 | tee julialog.txt
	./multijulia 1 0 0 -0.7 0.27015 2 | tee julialog.txt
	#./multibrot 0 1024 0 1024 2 255 | tee multibrotlog.txt
	viewnior 0multijulia.ppm
	#rm main
clean:
	rm *.ppm
	rm *.txt
	rm julia
	rm mandelbrot
	rm multibrot
julia: julia.c 
	gcc -g -Wall -pedantic -lm julia.c -o julia 
mandelbrot: mandelbrot.c 
	gcc -g -Wall -pedantic -lm mandelbrot.c -o mandelbrot
multibrot: multibrot.c 
	gcc -g -Wall -pedantic -lm multibrot.c -o multibrot
multijulia: multijulia.c 
	gcc -g -Wall -pedantic -lm multijulia.c -o multijulia 
