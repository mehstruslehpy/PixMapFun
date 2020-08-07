#!/bin/bash
imgnumber=0;
#convert each target file to ppm convert image.jpg -compress none image.ppm
for ((i=0;i<200;i++))
do
	./vertexxform 400 400 $(echo 2048-10*$i|bc) $(echo 2048-10*$i|bc) $((imgnumber)).ppm
	#mv lines.ppm $((imgnumber)).ppm
	imgnumber=$((imgnumber+1))
done
convert -delay 20 -loop 0 `ls -v *.ppm` triangles.gif

