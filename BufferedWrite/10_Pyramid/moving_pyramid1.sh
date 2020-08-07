#!/bin/bash
imgnumber=0;
#convert each target file to ppm convert image.jpg -compress none image.ppm
for ((i=-50;i<50;i++))
do
	#./cube_noview.sh 500 -500 -800 $i $(echo $i*\(-1\)|bc -l) $(echo $i+20+25|bc -l) 1000 1000 1000
	./pyramid_noview.sh 700 -1400 700 $i $(echo $i*\(-1\)|bc -l) $(echo $i+45|bc -l) 500 500 500
	mv view1.ppm $((imgnumber)).ppm
	imgnumber=$((imgnumber+1))
done
convert -delay 15 -loop 0 `ls -v *.ppm` pyramid1.gif

