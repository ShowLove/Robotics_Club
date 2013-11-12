How to run the software:
- You need Java 6 installed
- Download the 'jar' from: http://felixpageau.com/asv/2011/2011_roboboat_airstation.jar
- type in "java -jar 2011_roboboat_airstation.jar"
- Follow the help instructions displayed in your console to see how to use the air station software

Note: The software is currently set up with a 5m/15ft acceptance radius for the GPS point validation (computed as the Ellopsoidal distance between the expected GPS coordinate and the one included in the REPORT request using WGS84 as the ellipsoid reference).

Option "-active" is required
 -active VAL : Specify the active target. Value must be in (AIR|EARTH|FIRE|WATER
               )
 -air VAL    : Sets the air target gps coordinate (ex: '40.688888,-74.145111')
 -earth VAL  : Sets the earth target gps coordinate (ex: '36.802327,-74.145111')
 -fire VAL   : Sets the fire target gps coordinate (ex: '36.802327,-76.191379')
 -port N     : Sets the port number. Default 8888
 -water VAL  : Sets the water target gps coordinate (ex: '40.688888,-74.045111')
Example:
 java -jar 2011_roboboat_airstation.jar -active WATER -air 40.688888,-74.145111 -earth 36.802327,-74.145111 -fire 36.802327,-76.191379 -water 40.688888,-74.045111

::Message Format::.

All messages are TCP messages (UDP will not be supported). The protocol is ASCII based and each 
command is terminated with a Linefeed (LF) character (ASCII value: 0xA). The protocol description below 
will use the value ‘\n’ to represent the Linefeed characters The following commands are available:

Ping command:
     Used for: Testing connectivity with reporting service
     Sender sends: “PING\n”
     Service response: “PONG\n”

Report command:
     Used for: Reporting the ‘hot’ target and its position
     Sender sends: “REPORT;<target name>@<gps>\n”
     Service response: “SUCCESS\n” or ”FAIL\n” or “ERROR\n”
     Meaning of response: SUCCESS = reported accurately the hot target.
     FAIL = reported the wrong target or GPS.
     ERROR = the request is malformed.
     Possible values for <targetname>: “WATER”, “FIRE”, “EARTH”, “AIR”
     Format for GPS positions (in WGS84 datum): “<LATITUDE>,<LONGITUDE>”
     Note: Both latitudes and longitude are expected in decimal format.

Examples:
     Page 10/17
     ASV: “PING\n”
     Service: “PONG\n”
     ASV: “REPORT;WATER@40.688888,-74.045111\n”
     Service: “FAIL\n”
     ASV: “REPORT;FIRE@36.802327,-76.191379\n”
     Service: “SUCCESS\n”
     ASV: “TIMELEFT\n”
     Service: “ERROR\n"
     
Any other command issued will be answered with “ERROR\n”
