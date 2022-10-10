%{
/************* INCLUDES AND EXTERNALS *************************************/

#include <stdio.h>
#include <string.h>
#include "RdPMonteur.tab.h"

%}

%%
"node"							{return(NOEUD); }
"</"							{return("</"); }
"								{return('"');}	
">"								{return('>');}
"/>"								{return('/>');}
"id="							{return(ID);}
"attribute"							{return(ATT);}
"nodeType=\"place\""				{ return(PLACE);}
"nodeType=\"transition\""			{ return(TRANS);}
"arcType=\"arc\""				{ return(ARCTYPE);}
"attribute name=\"marking\""			{ return(MK);}
"attribute name=\"name\""	{ return(NAME);}
"arc"							{return(ARC);}
"source="							{ return(IN);}
"target="							{ return(OUT);}
[\n]*								{return (RL);}
[0-9]+								{ sscanf(yytext,"%d",&yylval.i); return(ENTIER);}
[a-zA-Z_][a-zA-Z0-9_]*						{ yylval.s=strdup(yytext); return(VARIABLE);}
[\t]								{}
%%
