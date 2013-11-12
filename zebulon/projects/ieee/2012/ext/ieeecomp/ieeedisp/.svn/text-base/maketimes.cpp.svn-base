#include<stdio.h>
#include<string>
#include<vector>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#define RUNSLOT (6)
#define BREAKTIME (30)

int main()
{
	FILE *instream=fopen("../ieeedata/teams.txt","r");
	char tmpline[1024];
	std::vector<std::string> AllTeams;
	srand(time(NULL));
	while(fgets(tmpline,1024,instream)!=NULL)
	{
		std::string tmp(tmpline);
		if(strstr(tmpline,",1\n")!=NULL)
		{
			AllTeams.push_back(tmp);
		}
	}
	fclose(instream);

	FILE *outstream=fopen("www/times.html","w");
	fprintf(outstream,"<html>\n");
	fprintf(outstream,"<head>\n");
	fprintf(outstream,"<meta http-equiv=\"refresh\" content=\"60\">\n");
	fprintf(outstream,"<title>IEEE SoutheastCon 2012 Hardware Competition Times</title>\n");
	fprintf(outstream,"</head>\n");
	fprintf(outstream,"<body>\n");
	//fprintf(outstream,"<h1>IEEE SoutheastCon 2012 Competition Times</h1>\n");

	fprintf(outstream,"<table width=100%% border=0 style=\"border: 1px solid #000000\">\n");
	fprintf(outstream,"<tr>\n");
	int time=0;
	for(int t=0;t<3;t++)
	{
		fprintf(outstream,"<td align=center width=33%% border=0 style=\"border: 1px solid #000000\">\n");
		fprintf(outstream,"<b>Round %d:</b>\n",t+1);
		fprintf(outstream,"<table width=100%% border=0 style=\"border: 1px solid #000000\">\n");
		fprintf(outstream,"<tr bgcolor=#000000>\n");
		fprintf(outstream,"<td><font color=#FFFFFF><b>Time</b></font></td>\n");
		fprintf(outstream,"<td><font color=#FFFFFF><b>C</b></font></td>\n");
		fprintf(outstream,"<td><font color=#FFFFFF><b>Team Name</b></font></td>\n");
		fprintf(outstream,"<td><font color=#FFFFFF><b>T</b></font></td>\n");
		//fprintf(outstream,"<td><font color=#FFFFFF><b>Competition</b></font></td>\n");
		fprintf(outstream,"</tr>\n");
		for(int i=0;i<AllTeams.size();i++)
		{
			int r=rand()%AllTeams.size();
			std::string tmp=AllTeams[r];
			AllTeams[r]=AllTeams[i];
			AllTeams[i]=tmp;
		}
		for(int i=0;i<AllTeams.size();i++)
		{
			if(i!=0 && i%4==0)
			{
				time+=RUNSLOT;
			}
			int hour = 9+time/60;
			int minutes = time%60;
			if( i%2==1 )
			{
				fprintf(outstream,"<tr bgcolor=#DDDDDD>\n");
			}
			else
			{
				fprintf(outstream,"<tr bgcolor=#FFFFFF>\n");
			}
			fprintf(outstream,"<td>%d:%02d</td><td align=center>%d</td>",hour,minutes,i%4+1);
			std::string team;
			std::string comp;
			int pos1=AllTeams[i].find("\"",2);
			int pos2=AllTeams[i].find("\"",pos1+1);
			int len=pos2-pos1;
			team=AllTeams[i].substr(pos1+1,len-1);
			pos1=AllTeams[i].find("\"",pos2+1);
			pos1=AllTeams[i].find("\"",pos1+1);
			pos1=AllTeams[i].find("\"",pos1+1);
			pos2=AllTeams[i].find("\"",pos1+1);
			len=pos2-pos1;
			comp=AllTeams[i].substr(pos1+1,len-1);

			//printf("%d:%02d,%s",hour,minutes,AllTeams[i].c_str());
			fprintf(outstream,"<td>%s</td>",team.c_str());
			if(comp=="Official University Team")
			{
				fprintf(outstream,"<td>U</td>");
			}
			else
			{
				fprintf(outstream,"<td>O</td>");
			}
		}
		fprintf(outstream,"</table>\n");
		fprintf(outstream,"</td>\n");
		time+=BREAKTIME;
	}
	fprintf(outstream,"<tr>\n");
	fprintf(outstream,"</table>\n");
	fprintf(outstream,"<center><a href=\"../index.html\">Back</a></center>\n");
	fprintf(outstream,"</body>\n");
	fprintf(outstream,"</html>\n");
	fclose(outstream);
}
