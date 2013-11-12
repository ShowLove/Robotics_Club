#!/bin/bash

cat ../ieeedata/teams.txt | while read I
do
	V=$(echo $I | cut -f 1 -d ",")
	mkdir -p www/$V
	F=$(echo "www/$V/index.html")
	L=$(cat ../ieeedata/$V/team.txt)
	T=$(echo $L | cut -f 2 -d \")
	N=$(echo $L | cut -f 4 -d \")
	C=$(echo $L | cut -f 6 -d \")
	./teamdata.sh "$T" "$N" "$C" "$V" > $F
done
