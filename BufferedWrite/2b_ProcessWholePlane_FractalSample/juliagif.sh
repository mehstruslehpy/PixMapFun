#!/bin/bash
imgnumber=0;
#convert each target file to ppm convert image.jpg -compress none image.ppm
for ((i=-80;i<80;i++))
do
	./julia.sh 1 0 0 $(echo -0.7+$i*0.001|bc -l) $(echo 0.27015-$i*0.001)
	mv 0julia.ppm $((imgnumber)).ppm
	imgnumber=$((imgnumber+1))
done
convert -delay 15 -loop 0 `ls -v *.ppm` julia.gif

