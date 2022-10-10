%{

/* .......... Declarations C ........*/

  //#include <stdio.h>
#include <string>
#include <cstring>
  //#include <iostream>
#include <stdio.h>
#include <map>

#include "net.h"


static	net *R;
//static SOG *G;
static  string t;
static map<int,int> * fm=NULL;

		

extern	FILE *yyin;
extern char yytext[];

/*........... Code C additionnel ....*/
extern "C" {
  int yylex();
  int yyerror(const char* s){
    printf("parsing error * %s *\n",s);
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

%token IN OUT MK ID PLACE ARC FARC TRANS NOEUD FNOEUD NAME RL ATT FATT ARCTYPE FINBALISE MODEL FMODEL VALUATION XML CORD OBS FINALMARKING MULT PLUS 
%token <i> ENTIER
%token <s> VARIABLE

%start reseau

%%
/*........... Regles de grammaire ...*/

	/*********************/
	/* lecture du reseau */
	/*********************/

Model 		: reseau  | SOG
		;
reseau	        : noeud | reseau noeud 
		{	
				
 		};
		;
SOG	        : Node | SOG Node 
		{	
 		};

Node 		: agregat | arc | model | XML | RL
		;
	
noeud		: place | transition | arc | model | FMODEL | RL | att | XML | finalmarking
		;



model : MODEL
{	
 };




att		: ATT '"' VARIABLE '"' CORD  VARIABLE FATT | ATT '"' VARIABLE '"' CORD  FATT
		{	
			
		};

finalmarking	: FINALMARKING CORD FM FATT RL
		{
			//printf(" finalmarking \n ");
			R->addFinalMarking(fm);	
		};
FM		: ENTIER MULT ENTIER
		{ //printf(" finalmarking 1 \n ");
			fm=new map<int,int>;
			(*fm)[$3]=$1;
			
			 
		}
| ENTIER MULT ENTIER PLUS FM
		{
		//printf(" finalmarking 2 \n ");

			(*fm)[$3]=$1;	
		};

		
 	/***********************/
	/* lecture d'une place */
	/***********************/

place		: NOEUD ID '"' ENTIER '"' PLACE CORD RL NAME CORD VARIABLE FATT RL FNOEUD RL 
		{
            printf("here1 place %s, %d!!\n",$11,$4);

		  
		  if(!R->addPlace($11,$4))
		  { 	
			printf("ID or/and NAME of place already used —— \n");
		    yyerror("ID place already used");return(1);
		  }
			
		}
                | NOEUD ID '"' ENTIER '"' PLACE CORD RL NAME CORD VARIABLE FATT RL MK CORD ENTIER FATT RL FNOEUD RL
		{		printf("here22 variable %s entier %d MK %d !!\n",$11,$4,$16);

		  if(!R->addPlace($11,$4,$16))
		  { 	
		    printf("ID place already used $11—— \n");
		    yyerror("ID place already used");return(1);
		  }
			
		}
		;

	/***********************/
	/* lecture d'une trans */
	/***********************/

transition   	: NOEUD ID '"' ENTIER '"' TRANS CORD RL NAME CORD VARIABLE FATT RL FNOEUD RL 
                {  		printf("here transition %s, %d\n",$11,$4);

		  if(!R->addTrans($11,$4))
		  { 	
		    printf("ID transition already used —— \n");
		    yyerror("ID transition already used");return(1);
		  }
		
		}
		| NOEUD ID '"' ENTIER '"' TRANS CORD RL NAME CORD VARIABLE FATT RL OBS CORD ENTIER FATT RL FNOEUD RL 
                {  		printf("here transition %s, %d\n",$11,$4);

		  if(!R->addTrans($11,$4,$16))
		  { 	
		    printf("ID transition already used —— \n");
		    yyerror("ID transition already used");return(1);
		  }
		
		}
	
                ;
	/**************************************************/
	/* lecture des entrées et sorties d'une transition*/
	/**************************************************/

arc		: ARC ID '"' ENTIER '"' IN '"' ENTIER '"' OUT '"' ENTIER '"' ARCTYPE '>' RL VALUATION CORD ENTIER FATT RL FARC 
		{ 

  			
		printf("here5 arc %d source %d cible %d\n",$4,$8,$12);
		if(!R->addPre($8,$12,$19) && !R->addPost($8,$12,$19))
		{
			yyerror("Arc source and/or target do not match");
			return(1);
		}

		}
;
/**************************************************/
	/* lecture des agrégats d’un SOG*/
	/**************************************************/



agregat : ARC
	{	
			
		};


%%

bool RdPMonteur::createNet(const char *f){	
  int i;
  if ((yyin=fopen(f,"r"))==NULL)
    return false;
  R=(net*)this;
  i=(int) yyparse();
  
  fclose(yyin);
  return (i==0);
}


