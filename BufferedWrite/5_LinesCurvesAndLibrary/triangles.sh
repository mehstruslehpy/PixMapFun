#!/bin/bash
imgnumber=0;
#convert each target file to ppm convert image.jpg -compress none image.ppm
for ((i=0;i<200;i++))
do
	#./main.sh 0.1 0.1 $(echo -0.966918+0.001*$i|bc -l) $(echo 2.879879-0.001*$i|bc -l) $(echo 0.765145-0.001*$i|bc -l) $(echo 0.744728-0.001*$i|bc -l)
	./triangle.sh
	mv lines.ppm $((imgnumber)).ppm
	imgnumber=$((imgnumber+1))
done
convert -delay 20 -loop 0 `ls -v *.ppm` triangles.gif

