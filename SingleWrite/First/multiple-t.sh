#!/bin/bash
for ((i=0;i<100;i++))
do
	echo "process $i.ppm"
	./main $((i)).ppm `calc $i*0.1` mat_func
done
convert -delay 2 -loop 0 `ls *.ppm|sort -h` out.gif
