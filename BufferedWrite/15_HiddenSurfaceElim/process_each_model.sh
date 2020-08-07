#!/bin/bash
for i in *.txt
do
	i=$(echo $i|sed 's/\.txt//')
	./shape $1 $2 $3    $4 $5 $6    $7 $8 $8    $i.txt $i.ppm
	#echo $i
done
