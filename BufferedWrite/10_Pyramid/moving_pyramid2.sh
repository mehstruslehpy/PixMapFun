#!/bin/bash
imgnumber=0;
#convert each target file to ppm convert image.jpg -compress none image.ppm
for ((i=0;i<200;i++))
do
	./pyramid_noview.sh $(echo 200+$i*20|bc -l) $(echo -1200-$i*5|bc -l) $(echo 700-$i*50|bc -l) $(echo 450+$i*2.5|bc -l) 200 800 500 500 500 view1.ppm
	mv view1.ppm $((imgnumber)).ppm
	imgnumber=$((imgnumber+1))
done
convert -delay 15 -loop 0 `ls -v *.ppm` pyramid2.gif

