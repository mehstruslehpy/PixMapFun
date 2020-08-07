#!/bin/bash
rm squares_model.ppm
./shape $1 $2 $3 $4 $5 $6 $7 $8 $9 squares_model.wire squares_model.color view1.ppm
viewnior view1.ppm

