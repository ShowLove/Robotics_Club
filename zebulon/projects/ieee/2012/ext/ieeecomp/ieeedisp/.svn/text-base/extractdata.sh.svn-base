#!/bin/bash
x=$1;
echo -n $x
for P in $(seq 0 3) 
do
	if [ -e ../ieeedata/$x/data$P.csv ]
	then
		LINE=$(cat ../ieeedata/$x/data$P.csv | grep -v \#)
		S=$(echo $LINE | cut -f 1 -d ",")
		echo -n ","$S
	else
		echo -n ",X"
	fi
done
echo
