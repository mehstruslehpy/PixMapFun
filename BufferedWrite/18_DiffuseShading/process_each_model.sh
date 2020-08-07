#!/bin/bash
for i in *.wire
do
	i=$(echo $i|sed 's/\.wire//')
	./moving_light.sh $i
	#rm *.ppm
	#./moving_shape1.sh $i
	#rm *.ppm
	#./moving_shape2.sh $i
	#rm *.ppm
	#./moving_shape3.sh $i
done
