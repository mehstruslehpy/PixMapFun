#!/bin/bash
#5 points per poly 5 polys per model
[ -e random_model.wire ] && rm random_model.wire
[ -e random_model.color ] && rm random_model.color
lines=$(( (RANDOM%10)+1 ))
echo "lines total:" $lines
for ((i=0;i<lines;i++))
do
	points=$(( (RANDOM%10)+1 ))
	echo "line:" $i
	echo "points total:" $points
	for ((j=0;j<points;j++))
	do
		echo "point:" $j
		echo -n "$(( (RANDOM%500) + 1 )),$(( (RANDOM%500) + 1 )),$(( (RANDOM%500) + 1 )),">>random_model.wire
		echo -n "$(( (RANDOM%500) + 1 )),$(( (RANDOM%500) + 1 )),$(( (RANDOM%500) + 1 )),">>random_model.color
	done
	echo "$(( (RANDOM%500) + 1 )),$(( (RANDOM%500) + 1 )),$(( (RANDOM%500) + 1 ))">>random_model.wire
	echo "$(( (RANDOM%500) + 1 )),$(( (RANDOM%500) + 1 )),$(( (RANDOM%500) + 1 ))">>random_model.color
done
