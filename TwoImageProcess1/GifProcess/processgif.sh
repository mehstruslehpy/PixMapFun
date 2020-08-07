#!/bin/bash
#usage: ./processgif.sh option outputfile

echo processing with option $1 to output $2

convert CatMp4.mp4 target.jpg
jpegcount=$(ls *.jpg |wc -w)
echo "$jpegcount files found"

#convert each target file to ppm convert image.jpg -compress none image.ppm
for ((i=0;i<jpegcount;i++))
do
	echo "convert target-$i.jpg to ppm:"
	convert target-$((i - 1)).jpg -compress none image-$((i - 1)).ppm
done

#Combine first image with self
./main image-0.ppm image-0.ppm out-0.ppm $1

#For all following images combine the nth image with the n-1th image
for ((i=1;i<jpegcount-1;i++))
do
	echo "process target-$i.ppm with target-$((i - 1)).jpg:"
	./main image-$i.ppm image-$((i - 1)).ppm out-$i.ppm $1
done

#recombine all files into gif
convert -delay 2 -loop 0 `ls -v out*` $2
rm *.ppm
rm *.jpg
