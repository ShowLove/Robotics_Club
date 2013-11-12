#!/bin/bash

#while [ 1 ]
#do
	date
	mkdir www
	echo "<html>
<head>
<title>IEEE SoutheastCon 2012 Hardware Competition</title>
</head>
<body>
<a href=\"university.html\">University</a><br>
<a href=\"open.html\">Open</a><br>
<a href=\"http://www.southeastcon2012.org/StudentProgram.html\">Rules</a>
</body>
</html>" > www/index.html
	svn up ../ieeedata
	./makesummary.sh
	./maketeams.sh
	rsync -vvr --delete www/ /var/www/
	#rsync -vvr --delete www/ robotics@pegasus.cc.ucf.edu:public_html/ieeecomp
#	sleep 60			
#done
