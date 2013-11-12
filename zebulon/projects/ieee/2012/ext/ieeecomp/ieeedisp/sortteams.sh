#!/bin/bash
COMP=$1
echo "<html>"
echo "<head>"
echo "<meta http-equiv="refresh" content="60">"
echo "<title>IEEE SoutheastCon 2012 Hardware Competition Summary Page</title>"
echo "</head>"
echo "<body>"
#echo "<h1>IEEE SoutheastCon 2012 Hardware Competition Summary Page</h1>"
echo "<table width=100% border=0 style=\"border: 1px solid #000000\">"
echo "<tr bgcolor=#000000>"
echo "<td><font color=#FFFFFF><b>School Name</b></font></td>"
echo "<td><font color=#FFFFFF><b>Team Name</b></font></td>"
echo "<td><font color=#FFFFFF><b>Qualified</b></font></td>"
echo "<td><font color=#FFFFFF><b>Run 1</b></font></td>"
echo "<td><font color=#FFFFFF><b>Run 2</b></font></td>"
echo "<td><font color=#FFFFFF><b>Run 3</b></font></td>"
echo "<td><font color=#FFFFFF><b>Average</b></font></td>"
echo "<td><font color=#FFFFFF><b>Final</b></font></td>"
echo "</tr>"

let bg=0;
cat ../ieeedata/teams.txt | sort | while read I
do
	V=$(echo $I | cut -f 1 -d ",")
	T=$(echo $I | cut -f 2 -d \")
	N=$(echo $I | cut -f 4 -d \")
	C=$(echo $I | cut -f 6 -d \")
	if [ "$C" == "$1" ]
	then
		./extractdata.sh "$V"
	fi
done | while read J
do
	T=$(echo $J | cut -f 1 -d ",")
	R1=$(echo $J | cut -f 2 -d ",")
	R2=$(echo $J | cut -f 3 -d ",")
	R3=$(echo $J | cut -f 4 -d ",")
	FIN=$(echo $J | cut -f 5 -d ",")
	let n=0;
	let SUM=0;
	if [ $R1 != "X" ]
	then
		let n=$n+1;
		let SUM=$SUM+$R1;
	else
		R1=" "
	fi

	if [ $R2 != "X" ]
	then
		let n=$n+1;
		let SUM=$SUM+$R2;
	else
		R2=" "
	fi
	if [ $R3 != "X" ] 
	then
		let n=$n+1;
		let SUM=$SUM+$R3;
	else
		R3=" "
	fi

	if [ $FIN == "X" ] 
	then
		FIN=" "
	fi

	if [ $n -gt 0 ]
	then
		S=$(echo $SUM"/"$n | bc)
	else
		S=0
	fi
	echo $T,$R1,$R2,$R3,$FIN,$S,$n
done | sort -n -k 7 -t "," | sort -r -n -k 6 -t ","| sort -r -n -k 5 -t "," | while read K
#done | sort -r -n -k 6 -t "," | while read K
do
	V=$(echo $K | cut -f 1 -d ",")
	R1=$(echo $K | cut -f 2 -d ",")
	R2=$(echo $K | cut -f 3 -d ",")
	R3=$(echo $K | cut -f 4 -d ",")
	FIN=$(echo $K | cut -f 5 -d ",")
	S=$(echo $K | cut -f 6 -d ",")

	I=$(cat ../ieeedata/$V/team.txt)
	T=$(echo $I | cut -f 2 -d \")
	N=$(echo $I | cut -f 4 -d \")

	Q=$(cat ../ieeedata/teams.txt | grep ^$V"," | cut -f 7 -d \"  | cut -f 2 -d ",")
	

	if [ $(echo $bg\%2 | bc ) -eq 1 ]
	then
		echo "<tr bgcolor=#DDDDDD>"
	else
		echo "<tr bgcolor=#FFFFFF>"
	fi
	let bg=$bg+1;
	echo "<td><a href=\"$V\">"$T"</a></td>"
	echo "<td>"$N"</td>"
	if [ $Q -eq 1 ]
	then
		echo "<td>Yes</td>"
	else
		echo "<td>No</td>"
	fi
	echo "<td>"$R1"</td>"
	echo "<td>"$R2"</td>"
	echo "<td>"$R3"</td>"
	echo "<td>"$S"</td>"
	echo "<td>"$FIN"</td>"
	echo "</tr>"
done

echo "</table>"
echo "<center><a href=\"../index.html\">Back</a></center>"
echo "</body>"
echo "</html>"

