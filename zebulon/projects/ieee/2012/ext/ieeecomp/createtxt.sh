#!/bin/bash

let x=0
cat ieeeteams.csv | sort | while read I
do
	F=$(echo "ieeedata/$x/team.txt")
	echo "$I" > $F
	echo $x,$I 
	let x=$x+1;
done > ieeedata/teams.txt
