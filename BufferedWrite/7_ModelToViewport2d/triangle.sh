#!/bin/bash
	x0=$((RANDOM%2048))
	y0=$((RANDOM%2048))
	x1=$((RANDOM%2048))
	y1=$((RANDOM%2048))
	x2=$((RANDOM%2048))
	y2=$((RANDOM%2048))
	./lines $x0 $y0 $x1 $y1 $x2 $y2 $x0 $y0 | tee log.txt
	viewnior lines.ppm
