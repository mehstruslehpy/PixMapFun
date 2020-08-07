#!/bin/bash
#wave up and down vertically
imgnumber=0;
#convert each target file to ppm convert image.jpg -compress none image.ppm
#2048 2048 2048    0 1 0    0 0 1
for ((i=0;i<100;i++))
do
	echo $1.txt
	./shape_noview.sh 2048 $(echo -1*$i*25+2048|bc -l) 2048 $(echo 0.01*$i|bc -l) 1 0 0 $(echo 0.05*$i|bc -l) 1 $1
	#./shape_noview.sh 2048 2048 2048 $(echo 0.1*$i+0|bc -l) 1 0 0 0 1 $1
	mv view1.ppm $((imgnumber)).ppm
	imgnumber=$((imgnumber+1))
done
convert -delay 15 -loop 0 `ls -v *.ppm` $1_2.gif
rm *.ppm
