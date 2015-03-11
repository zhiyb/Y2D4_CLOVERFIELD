#! /bin/bash

files=($(while (($# != 0)); do
	find "$1" -type f -name '*.cpp'
	find "$1" -type f -name '*.c'
	find "$1" -type f -name '*.h'
	shift
done | sort))

for ((i = 0; i < ${#files[@]}; i++)); do
	echo "${files[i]}"
	cat "${files[i]}"
	echo
done
