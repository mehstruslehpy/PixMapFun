#!/bin/bash
for inputfile in ./* ; do
	outputfile="${inputfile%.*}.jpg"
	convert "$inputfile" "$outputfile" &&
		[[ -e "$outputfile" && "$inputfile" != "$outputfile" ]] && rm "$inputfile"
	done
