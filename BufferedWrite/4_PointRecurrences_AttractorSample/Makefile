all: pickover lorenz lotka_volterra
	#gdb --args ./attractor1 1 1 1 10 10 28 2.666 | log.txt
	#./main 0.1 0.1 -0.966918 2.879879 0.765145 0.744728 | tee log1.txt
	./pickover 0.1 0.1 -0.966918 2.879879 0.765145 0.744728 
	#./attractor1 50 50 50 0.01 10 28 2.6666 | tee log2.txt
	./lorenz 50 50 50 0.01 10 28 2.6666 
	#./lotka_volterra 50 50 0.01 10 28 2.6666 5 | tee log3.txt
	./lotka_volterra 0.9 0.9 0.1 0.6666 1.3333 1 1 | tee log3.txt
	#viewnior 1attractor.ppm

clean:
	rm pickover
	rm lorenz
	rm log.txt
	rm *.ppm
pickover: pickover.c 
	gcc -g -Wall -pedantic -lm pickover.c -o pickover
lorenz: lorenz.c 
	gcc -g -Wall -pedantic -lm lorenz.c -o lorenz
lotka_volterra: lotka_volterra.c 
	gcc -g -Wall -pedantic -lm lotka_volterra.c -o lotka_volterra

