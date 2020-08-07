#!/bin/bash
	x0=$((RANDOM%1024))
	y0=$((RANDOM%1024))
	x1=$((RANDOM%1024))
	y1=$((RANDOM%1024))
	x2=$((RANDOM%1024))
	y2=$((RANDOM%1024))
	x3=$((RANDOM%1024))
	y3=$((RANDOM%1024))
	./main $x0 $y0 $x1 $y1 $x2 $y2 $x3 $y3 $x0 $y0 | tee log.txt
	viewnior lines.ppm
