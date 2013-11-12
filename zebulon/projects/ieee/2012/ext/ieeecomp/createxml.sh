#!/bin/bash

let x=0
cat ieeeteams.csv | sort | while read I
do
	F=$(echo "teams/$x.xml")
	T=$(echo $I | cut -f 2 -d \")
	N=$(echo $I | cut -f 4 -d \")
	C=$(echo $I | cut -f 6 -d \")
	
	echo "<TeamInfo>" > $F

	echo "    <Names University=\"$T\" Name=\"$N\" />" >> $F

	echo "    <Competition Type=\"$C\" />" >> $F

	echo "    <Colors TextColorRed=\"217\" TextColorGreen=\"217\" TextColorBlue=\"25\"" >> $F
	echo "        BGColorRed=\"0\" BGColorGreen=\"1\" BGColorBlue=\"2\" />" >> $F

	echo "</TeamInfo>" >> $F
	let x=$x+1;
done
