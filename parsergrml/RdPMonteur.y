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

%token IN OUT MK ID PLACE ARC FARC TRANS NOEUD FNOEUD NAME RL ATT FATT ARCTYPE FINBALISE MODEL FMODEL VALUATION XML CORD
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

noeud		: place | transition | arc | model | FMODEL | RL | VARIABLE | att | XML
		;
model : MODEL
{	for(int i=0;i<NB_ELEMENT_TAB;i++)
{transitionName[i]="";	
placeName[i]="";}
 };

att		: ATT '"' VARIABLE '"' CORD  VARIABLE FATT | ATT '"' VARIABLE '"' CORD  FATT
{	};
 	/***********************/
	/* lecture d'une place */
	/***********************/

place		: NOEUD ID '"' ENTIER '"' PLACE CORD RL NAME CORD VARIABLE FATT RL FNOEUD RL 
		{
           // printf("here1 place %s, %d!!\n",$11,$4);

		  
		  if(!R->addPlace($11))
		  { 	
		    yyerror("a1");return(1);
		  }
		else
			placeName[$4]=$11;	
		}
                | NOEUD ID '"' ENTIER '"' PLACE CORD RL NAME CORD VARIABLE FATT RL MK CORD ENTIER FATT RL FNOEUD RL
		{	//	printf("here22 variable %s entier %d MK %d !!\n",$11,$4,$16);

		  if(!R->addPlace($11,$16))
		  { 	
		    yyerror("");return(1);
		  }
		else
        { 	//printf("d5alnaaaaa\n");
placeName[$4]=$11;		  }
			
		}
		;

	/***********************/
	/* lecture d'une trans */
	/***********************/

transition   	: NOEUD ID '"' ENTIER '"' TRANS CORD RL NAME CORD VARIABLE FATT RL FNOEUD RL 
                {  	//	printf("here transition %s, %d\n",$9,$4);

		  if(!R->addTrans($11))
		  { 	
		    yyerror("a2");return(1);
		  }
		else
		{	transitionName[$4]=$11;
        //for (int i=0;i<5;i++)
         //  printf("here3 %s \n",transitionName[$4].c_str());
		 }
         // t=$2;
		}
                ;
	/**************************************************/
	/* lecture des entrées et sorties d'une transition*/
	/**************************************************/

arc		: /*ARC ID '"' ENTIER '"' ARCTYPE IN '"' ENTIER '"' OUT '"' ENTIER '"' FINBALISE RL
		{	//printf("here4 arc %d source %d cible %d\n",$4,$9,$13);
      //  		 printf("fin");

    //   for (int i=0;i<5;i++)
  //          printf("GRrrr %s, %d\n",transitionName[i].c_str(),$4);
//		 printf("fin");
  //       if(strcmp(transitionName[$9].c_str(),"")!=0)
//printf("njarbou ++%s++",transitionName[$9].c_str());
  		
        
    
			if(strcmp(transitionName[$9].c_str(),"")!=0)
			{
				if(!R->addPost(placeName[$13],transitionName[$9],1))
		  		{ 	
		   		 yyerror("a3");return(1);
		   		}
			}
			else {
		if(strcmp(placeName[$9].c_str(),"")!=0)
			{
				if(!R->addPre(placeName[$9],transitionName[$13],1))
		  		{ 	
		   		 yyerror("a4");return(1);
		   		}
			}
			}
            

		} 
| */ ARC ID '"' ENTIER '"' IN '"' ENTIER '"' OUT '"' ENTIER '"' ARCTYPE '>' RL VALUATION CORD ENTIER FATT RL FARC 
{ 

  			
	//printf("here5 arc %d source %d cible %d\n",$4,$8,$12);
//for (int i=0;i<5;i++)file://localhost/Users/lipn/Desktop/hanen/ObsGraphToolGML/parser/RdPMonteur.y
           // printf("GRrrr %s, %d\n",transitionName[i].c_str(),$4);
		 //printf("fin");
        // if(strcmp(transitionName[$8].c_str(),"")!=0)
//printf("njarbou ++%s++",transitionName[$8].c_str());
  		
        
    
			if(strcmp(transitionName[$8].c_str(),"")!=0)
			{ //printf("hiiiii1,%s et %s",transitionName[$8].c_str(),placeName[$12].c_str());
				if(!R->addPost(placeName[$12],transitionName[$8],1))
		  		{ 			   		 yyerror("a5");return(1);
		   		}
			}
			else {
//printf("hiiiii2");
		if(strcmp(placeName[$8].c_str(),"")!=0)
			{
				
				if(!R->addPre(placeName[$8],transitionName[$12],1))
		  		{ 	
		   		 yyerror("a6");return(1);
		   		}
			}
			}

		}
;




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


