#!/bin/bash
imgnumber=0;
#convert each target file to ppm convert image.jpg -compress none image.ppm
for ((i=-80;i<80;i++))
do
	./multijulia.sh 1 0 0 -0.7 0.27015 $((i))
	mv 0multijulia.ppm $((imgnumber)).ppm
	imgnumber=$((imgnumber+1))
done
convert -delay 15 -loop 0 `ls -v *.ppm` multijulia.gif

