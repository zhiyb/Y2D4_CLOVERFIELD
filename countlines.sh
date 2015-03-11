#! /bin/bash

while (($# != 0)); do
	#echo $1 1>&2
	find "$1" -type f -name '*.cpp' -exec cat {} \;
	find "$1" -type f -name '*.c' -exec cat {} \;
	find "$1" -type f -name '*.h' -exec cat {} \;
	shift
done | wc -l
