%{

/* .......... Declarations C ........*/

  //#include <stdio.h>
#include <string>
#include <cstring>
  //#include <iostream>
#include <stdio.h>
#include "RdPMonteur.h"
#define NB_ELEMENT_TAB 10000000


static	RdPMonteur *R;
static  string t;
string transitionName[NB_ELEMENT_TAB];
string placeName[NB_ELEMENT_TAB];
extern	FILE *yyin;
extern char yytext[];

/*........... Code C additionnel ....*/
extern "C" {
  int yylex();
  int yyerror(const char* s){
    printf("parsing error\n");
    return (1);
  }
  
  int yywrap(){
    return (1);
  }
}

%}
/*........... Declarations Yacc .....*/
%union { 	
  int i;
  char *s;
}

%token IN, OUT, MK, ID, PLACE, ARC, TRANS, /*MODELD, MODELF, */NOEUD, FNOEUD, NAME, RL, FATT, ARCTYPE
%token <i> ENTIER
%token <s> VARIABLE



%start reseau

%%
/*........... Regles de grammaire ...*/

	/*********************/
	/* lecture du reseau */
	/*********************/

reseau	        : noeud | reseau noeud 
		;

noeud		: place | transition | arc
		;

 	/***********************/
	/* lecture d'une place */
	/***********************/

place		: NOEUD ID '"' ENTIER '"' PLACE RL NAME VARIABLE FATT RL FNOEUD  
		{	
		  
		  if(!R->addPlace($9))
		  { 	
		    yyerror("");return(1);
		  }
		else
			placeName[$4]=$9;	
		}
                | NOEUD ID '"' ENTIER '"' PLACE RL NAME VARIABLE FATT RL MK ENTIER FATT RL FNOEUD 
		{	
		  if(!R->addPlace($9,$13))
		  { 	
		    yyerror("");return(1);
		  }
		else
			placeName[$4]=$9;
		}
		;

	/***********************/
	/* lecture d'une trans */
	/***********************/

transition   	: NOEUD ID '"' ENTIER '"' TRANS RL NAME VARIABLE FATT RL FNOEUD  
                {
		  if(!R->addTrans($9))
		  { 	
		    yyerror("");return(1);
		  }
		else
			transitionName[$4]=$9;
		 // t=$2;
		}
                ;
	/**************************************************/
	/* lecture des entrées et sorties d'une transition*/
	/**************************************************/

arc		: ARC ID '"' ENTIER '"' ARCTYPE IN '"' ENTIER '"' OUT '"' ENTIER '"'
		{
  		//map<int,string>::const_iterator ti=transitionName.find($9);
			if(strcmp(transitionName[$9].c_str(),NULL)!=0)
			{
				// map<int,string>::const_iterator tj=placeName.find($13);
				if(!R->addPost(placeName[$13],transitionName[$9],1))
		  		{ 	
		   		 yyerror("");return(1);
		   		}
			}
			else {
			//ti=placeName.find($9);
		if(strcmp(placeName[$9].c_str(),NULL)!=0)
			{
				// map<int,string>::const_iterator tj=transitionName.find($13);
				if(!R->addPre(placeName[$9],transitionName[$13],1))
		  		{ 	
		   		 yyerror("");return(1);
		   		}
			}
			}

		} ;



%%

bool RdPMonteur::create(const char *f){	
  int i;
  if ((yyin=fopen(f,"r"))==NULL)
    return false;
  R=this;
  i=(int) yyparse();
  fclose(yyin);
  return (i==0);
}


