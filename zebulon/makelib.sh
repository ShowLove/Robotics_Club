#/bin/bash

#Run this from the base directory of the lib
#Example:
#run from ./v3/sensors/compass
#../../makelib.sh name

if [ -z "$1" ]
then
	echo Need a libname
else
	mkdir -p $1/src/$1
	mkdir -p $1/include/$1
	mkdir -p $1/build/codelite/$1
	mkdir -p $1/ext
	mkdir -p $1/lib
	mkdir -p $1/bin
	mkdir -p $1/doc
fi
