#!/bin/bash
imgnumber=0;
#convert each target file to ppm convert image.jpg -compress none image.ppm
#2048 2048 2048    0 1 0    0 0 1
for ((i=0;i<100;i++))
do
	echo $1.txt
	./shape_noview.sh $(echo s\($i*0.025\)*2048|bc -l) $(echo c\($i*0.05\)+2048|bc -l) $(echo 2048|bc -l)\
					0 $(echo $i*0.05+1|bc -l) 0\
					0 0 1\
					$1
	mv view1.ppm $((imgnumber)).ppm
	imgnumber=$((imgnumber+1))
done
convert -delay 15 -loop 0 `ls -v *.ppm` $1.gif
rm *.ppm
