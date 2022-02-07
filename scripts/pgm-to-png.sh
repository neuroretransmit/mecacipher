#!/bin/sh

DIRECTORY="img/"

for f in $(find $DIRECTORY -name *.pgm); do
	convert $f $DIRECTORY/$(basename $f).png
done
