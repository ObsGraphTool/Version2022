%{
/************* INCLUDES AND EXTERNALS *************************************/
//#include "global.h"
#include <stdio.h>
#include <string.h>
#include "RdPMonteur.tab.h"

%}

%%
"<model formalismUrl=\"http://formalisms.cosyverif.org/pt-net.fml\" xmlns=\"http://cosyverif.org/ns/model\">"							{return(MODEL);}
"<?xml version=\"1.0\" encoding=\"UTF-8\"?>"			{return(XML);}
"</model>"							{return(FMODEL);}
"\""								{return('"');}	
">"								{return('>');}
"/>"								{return(FINBALISE);}
^"<node"[ ]*							{return(NOEUD); }
^"</node>"							{return(FNOEUD); }
"id"[ ]*"="[ ]*							{return(ID);}
"</attribute>"							{return(FATT);}
"nodeType"[ ]*"="[ ]*"\"place\""[ ]*""				{ return(PLACE);}
"nodeType"[ ]*"="[ ]*"\"transition\""[ ]*""			{ return(TRANS);}
"arcType"[ ]*"="[ ]*"\"arc\""[ ]*  				{ return(ARCTYPE);}
"<"[ ]*"attribute"[ ]*" name"[ ]*"="[ ]*""			{ return(ATT);}
"<"[ ]*"attribute"[ ]*" name"[ ]*"="[ ]*"\"marking\""	{ return(MK);}
"<"[ ]*"attribute"[ ]*" name"[ ]*"="[ ]*"\"name\""	{ return(NAME);}
"<"[ ]*"attribute"[ ]*" name"[ ]*"="[ ]*"\"FinalMarking\""	{ return(FINALMARKING);}
"<"[ ]*"attribute"[ ]*" name"[ ]*"="[ ]*"\"observed\""	{ return(OBS);}
"<"[ ]*"attribute"[ ]*" name"[ ]*"="[ ]*"\"valuation\""	{ return(VALUATION);}
^"<arc"							{return(ARC);}
"</arc>"							{return(FARC);}
"source"[ ]*"="[ ]*							{ return(IN);}
"target"[ ]*"="[ ]*								{ return(OUT);}
[ ]*"x=\""[ ]*[0-9]+"\""[ ]*"y=\""[ ]*[0-9]+"\">"				{return (CORD);}
[\n]								{return (RL);}
"*"								{return (MULT);}
"+"								{return (PLUS);}
[0-9]+								{ sscanf(yytext,"%d",&yylval.i); return(ENTIER);}
[a-zA-Z0-9_,][a-zA-Z0-9_,\ ]*						{ yylval.s=strdup(yytext); return(VARIABLE);}
[\t]								{}
%%
