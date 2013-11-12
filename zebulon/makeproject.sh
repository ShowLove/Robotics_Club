#/bin/bash

#Run this from the base directory of the project
#Example:
#run from ./v3/projects/
#../../makeproject.sh name year

if [ -z "$1" ] || [ -z "$2" ]
then
	echo Need a project and year
else
	mkdir -p $1/$2/src/$1
	mkdir -p $1/$2/include/$1
	mkdir -p $1/$2/build/codelite/$1
	mkdir -p $1/$2/ext
	mkdir -p $1/$2/lib
	mkdir -p $1/$2/bin
	mkdir -p $1/$2/doc
	mkdir -p $1/$2/pcb
	mkdir -p $1/$2/tmp
	mkdir -p $1/$2/cad
	mkdir -p $1/$2/data
fi
