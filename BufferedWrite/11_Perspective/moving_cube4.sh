#!/bin/bash
imgnumber=0;
#convert each target file to ppm convert image.jpg -compress none image.ppm
for ((i=0;i<100;i++))
do
	./cube_noview.sh $(echo $i*\(10\)+1000|bc -l) $(echo $i*\(-10\)+2048|bc -l) $(echo -30*$i+2048|bc -l)\
					$(echo $i*\(0.0001\)+0|bc -l) $(echo 0.001*$i+3|bc -l) $(echo $i*\(-0.01\)+1|bc -l)\
					$(echo $i*\(0.01\)+0|bc -l) $(echo 0.001*$i+0|bc -l) $(echo $i*\(-0.01\)+1|bc -l)
	mv view1.ppm $((imgnumber)).ppm
	imgnumber=$((imgnumber+1))
done
convert -delay 15 -loop 0 `ls -v *.ppm` cube4.gif
rm *.ppm
