#!/bin/bash
T=$1;
N=$2;
C=$3;
x=$4;

echo "<html>"
echo "<head>"
echo "<title>IEEE SoutheastCon 2012 Hardware Competition Team Page</title>"
echo "</head>"
echo "<body>"
#Make the Title prettier
echo "<center><h1>"$T"</h1></center>"
echo "<center><h2>"$N"</h2></center>"
echo "<center><h2>"$C"</h2></center>"
#Table of run information
./formatruns.sh "$x"
if [ "$C" == "Official University Team" ]
then
	echo "<center><a href=\"../university.html\">Back</a></center>"
else
	echo "<center><a href=\"../open.html\">Back</a></center>"
fi
echo "</body>"
echo "</html>"
