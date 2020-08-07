#!/bin/bash
imgnumber=0;
#convert each target file to ppm convert image.jpg -compress none image.ppm
for ((i=-50;i<100;i++))
do
	#./pyramid_noview.sh 700 -700 700 $(echo 200+$i*10|bc -l) $(echo 700-$i*10) 800 500 500 500
	#./cube_noview.sh 1000 -200 -500 -10 -10 20 1000 1000 1000
	./cube_noview.sh 500 -500 -800 $i $(echo $i*\(-1\)|bc -l) $(echo $i+20+25|bc -l) 1000 1000 1000
	mv view1.ppm $((imgnumber)).ppm
	imgnumber=$((imgnumber+1))
done
convert -delay 15 -loop 0 `ls -v *.ppm` cube1.gif

