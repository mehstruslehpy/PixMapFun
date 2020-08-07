#!/bin/bash
imgnumber=0;
#convert each target file to ppm convert image.jpg -compress none image.ppm
for ((i=-1000;i<1000;i++))
do
	./main.sh 0.1 0.1 $(echo -0.966918+0.001*$i|bc -l) $(echo 2.879879-0.001*$i|bc -l) $(echo 0.765145-0.001*$i|bc -l) $(echo 0.744728-0.001*$i|bc -l)
	mv 0attractor.ppm $((imgnumber)).ppm
	imgnumber=$((imgnumber+1))
done
convert -delay 2 -loop 0 `ls -v *.ppm` attractor.gif

