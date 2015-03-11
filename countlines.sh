#! /bin/bash

files=$(while (($# != 0)); do
	find "$1" -type f -name '*.cpp'
	find "$1" -type f -name '*.c'
	find "$1" -type f -name '*.h'
	shift
done)
echo "$files" 1>&2
cat $files | wc -l
