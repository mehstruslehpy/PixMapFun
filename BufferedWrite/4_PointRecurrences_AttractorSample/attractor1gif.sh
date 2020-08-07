#!/bin/bash
imgnumber=0;
#convert each target file to ppm convert image.jpg -compress none image.ppm
for ((i=0;i<1000;i++))
do
	#./main.sh 0.1 0.1 $(echo -0.966918+0.001*$i|bc -l) $(echo 2.879879-0.001*$i|bc -l) $(echo 0.765145-0.001*$i|bc -l) $(echo 0.744728-0.001*$i|bc -l)
	./attractor1 50 50 50 0.01 10 28 $(echo 0.01*$i|bc -l)
	mv 1attractor.ppm $((imgnumber)).ppm
	imgnumber=$((imgnumber+1))
done
convert -limit memory 8gb -delay 2 -loop 0 `ls -v *.ppm` attractor.gif

