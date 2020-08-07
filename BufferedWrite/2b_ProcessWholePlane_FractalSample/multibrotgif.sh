#!/bin/bash
imgnumber=0;
#convert each target file to ppm convert image.jpg -compress none image.ppm
for ((i=-80;i<80;i++))
do
	./multibrot.sh 0 1024 0 1024 $((i))
	mv 0multibrot.ppm $((imgnumber)).ppm
	imgnumber=$((imgnumber+1))
done
convert -delay 15 -loop 0 `ls -v *.ppm` multibrot.gif

