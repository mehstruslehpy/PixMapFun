#!/bin/bash
rm cube_model1.ppm
./shape $1 $2 $3 $4 $5 $6 $7 $8 $9 cube_model1.wire cube_model1.color view1.ppm
viewnior view1.ppm

