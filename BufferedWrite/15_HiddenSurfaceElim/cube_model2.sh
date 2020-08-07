#!/bin/bash
rm cube_model2.ppm
./shape $1 $2 $3 $4 $5 $6 $7 $8 $9 cube_model2.wire cube_model2.color view1.ppm
viewnior view1.ppm

