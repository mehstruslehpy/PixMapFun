#!/bin/bash
#wave up and down vertically
imgnumber=0;
#convert each target file to ppm convert image.jpg -compress none image.ppm
#2048 2048 2048    0 1 0    0 0 1
for ((i=-100;i<=100;i+=5))
do
	echo $i
	./shape_noview.sh 2048 2048 2048    0 1 0    0 0 1 500 500 $(echo 10*\($i\)|bc -l)    1 1 0.1 $1
	mv view1.ppm $((imgnumber)).ppm
	imgnumber=$((imgnumber+1))
done
convert -delay 15 -loop 0 `ls -v *.ppm` $1_1.gif
rm *.ppm
