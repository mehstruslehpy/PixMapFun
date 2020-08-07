#!/bin/bash
imgnumber=0;
#convert each target file to ppm convert image.jpg -compress none image.ppm
for ((i=0;i<100;i++))
do
	./cube_noview.sh $(echo $i*\(10\)+1000|bc -l) $(echo $i*\(-10\)+2048|bc -l) 2048 $(echo $i*\(-0.0001\)+0|bc -l) 3 $(echo $i*\(0.0001\)+1|bc -l) 0 0 1
	mv view1.ppm $((imgnumber)).ppm
	imgnumber=$((imgnumber+1))
done
convert -delay 15 -loop 0 `ls -v *.ppm` cube2.gif
rm *.ppm
