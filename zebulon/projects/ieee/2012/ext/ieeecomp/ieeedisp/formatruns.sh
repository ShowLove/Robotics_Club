#!/bin/bash
x=$1;
echo "<table border=0>"

R=$(ls -1 ../ieeedata/$x/data*.csv | wc -l)
echo "<tr>"
#echo $R
if [ $R -gt 0 ]
then
	for P in $(seq 0 $(echo $R-1 | bc)) 
	do 
		echo "<td>"
		echo "<table width=100% border=0 style=\"border: 1px solid #000000\">"
		echo "<tr bgcolor=#000000><td colspan=2 align=center><font color=#FFFFFF><b>Run " $(echo $P+1 | bc)"</b></font></td></tr>"
		LINE=$(cat ../ieeedata/$x/data$P.csv | grep -v \# | tail -n 1)

		S=$(echo $LINE | cut -f 1 -d ",")
		C=$(echo $LINE | cut -f 2 -d ",")
		I=$(echo $LINE | cut -f 3 -d ",")
		L=$(echo $LINE | cut -f 4 -d ",")
		T=$(echo "scale=3;"$(echo $LINE | cut -f 5 -d ",")"/1000.0"  | bc -l)
		#score,correct,incorrect,laps,time
		echo "<tr><td>Score</td><td align=right>"$S"</td></tr>"
		echo "<tr><td>Correct</td><td align=right>"$C"</td></tr>"
		echo "<tr><td>Incorrect</td><td align=right>"$I"</td></tr>"
		echo "<tr><td>Laps</td><td align=right>"$L"</td></tr>"
		echo "<tr><td>Time</td><td align=right>"$T"</td></tr>"
		echo "</table>"
		echo "<table width=100% border=0 style=\"border: 1px solid #000000\">"
		echo "<tr bgcolor=#000000>"
		echo "<td><font color=#FFFFFF><b>Turn</b></font></td><td><font color=#FFFFFF><b>True</b></font></td><td><font color=#FFFFFF><b>Measured</b></font></td>"
		echo "</tr>"
		let t=1;
		cat ../ieeedata/$x/raw$P.csv | grep -v \# | while read D
		do
			T=$(echo $D | cut -f 1 -d ",")
			M=$(echo $D | cut -f 2 -d ",")
			if [ $(echo $t\%2 | bc ) -eq 1 ]
			then
				echo "<tr bgcolor=#DDDDDD>"
			else
				echo "<tr bgcolor=#FFFFFF>"
			fi
			TL=$(echo $T%2 | bc)
			ML=$(echo $M%2 | bc)
			echo "<td align=center>"
			echo $t
			echo "</td>"
			echo "<td align=center>"
			if [ $TL -eq 0 ]
			then
				echo Left
			else
				echo Right
			fi
			echo "</td>"
			echo "<td align=center>"
			if [ $T -eq $M ]
			then
				echo "<b>"
			fi
			if [ $M -eq "-1" ]
			then
				echo Skip
			else
				if [ $ML -eq 0 ]
				then
					echo Left
				else
					echo Right
				fi
			fi
			if [ $T -eq $M ]
			then
				echo "</b>"
			fi
			echo "</td>"
			echo "</tr>"
			let t=$t+1;
		done
		echo "</table>"
		echo "</td>"
	done

fi
echo "</tr>"

echo "</table>"

