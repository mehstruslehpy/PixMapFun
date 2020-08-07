#!/bin/bash
	./main $1 $2 $3 $4 | tee log.txt
	viewnior lines.ppm
