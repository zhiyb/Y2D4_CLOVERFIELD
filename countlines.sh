#! /bin/bash

files=$(while (($# != 0)); do
	find "$1" -type f -name '*.cpp'
	find "$1" -type f -name '*.c'
	find "$1" -type f -name '*.h'
	shift
done | sort)
echo "$files"
cat $files | wc -l 1>&2
