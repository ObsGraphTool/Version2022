 // version 0.1
#include <time.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <ctime>
#include "bdd.h"
#include "fdd.h"
#include "Net.h"
#include "RdPBDD.h"
#include "Simple_MDGraph.h"
using namespace std;
//#include "tinyxml.h"
//#include <tinystr.h>
int Formula_transitions(const char * f, Set_mot& formula_trans, net Rv) ;
bdd EmersonLey(bdd,net);
double getTime(){
	return( ((double)clock()) / (double)CLOCKS_PER_SEC);
}



void reordering(int nbvar) {
    
	// all the variables belong to a main block
	bdd_varblockall();
	// to each fdd variable corresponds a block
	for (int i=0; i<nbvar; i++)
		fdd_intaddvarblock(i, i, BDD_REORDER_FIXED);
    
	bdd_autoreorder(BDD_REORDER_SIFT);
    
	bdd_reorder_verbose(2);
}


/***********************************************/
int main(int argc, char** argv) 
{
   
  bool Soundness = false;
  bool RelaxedSoundness = false;
  bool WeakSoundness = false;
  bool EasySoundness = false;
  bool Deadlock = false;
  int pn_index = 0;
  for (;;) 
    {
      if (argc < pn_index + 3)
	cout<<"Erreur de Syntaxe"<<endl;
      ++pn_index;
      if (!strncmp(argv[pn_index], "-S", 2)) {
	Soundness=true;
      }
      else if (!strncmp(argv[pn_index], "-R", 2)) {
	RelaxedSoundness=true;
      }
      else if (!strncmp(argv[pn_index], "-W", 2)) {
	WeakSoundness=true;
      }
      else if (!strncmp(argv[pn_index], "-E", 2)) {
	EasySoundness=true;
      }
      else if (!strncmp(argv[pn_index], "-D", 2)) {
	Deadlock=true;
      }
      else 
	break;
       
    }
  char Int[100]="";

  int NbSubnets=(argc-pn_index-1)/3;
  cout<<"nombre de sous-reseaux : "<<NbSubnets<<endl;
                
  net **Subnets=new net*[NbSubnets];
  RdPBDD **BddSubnets=new RdPBDD*[NbSubnets];
  bdd_init(1000000,10000);
  string interfacefilename;
  string ob_tr;

  for(int i=pn_index,j=0;i<=argc-pn_index-1;i=i+3,j++)
    {
      cout<<"\n________ sous-reseau _______________"<<j+1<<endl;
      cout<<"Fichier net : "<<argv[i]<<endl;
      if((!strncmp(argv[i+1], "-O", 2)))
	{
	  interfacefilename=argv[i+1]+2;
	  cout<<"Fichier Interface : "<<interfacefilename<<endl;
	}
      int cpt=0;

      if((!strncmp(argv[i+1], "-o", 2)))
	{
	  std::istringstream is(argv[i+1]+2);
	  std::string s;
	  is >> s;
	  while (!is.fail()) 
	    {
	      cpt++;
	      ob_tr=ob_tr+s+' ';
	      is >> s;
	    }

	  interfacefilename="obsfile";
	  ofstream fichier(interfacefilename, ios::out | ios::trunc);
	  if(fichier)
	    {
	      
	      fichier << cpt << 24<< endl;
	      fichier << ob_tr;
 
	      fichier.close();
	    }
	  else
	    cerr << "Impossible d'ouvrir le fichier !" << endl;
 

	}
      if((!strncmp(argv[i+1], "-", 2)))
	{
	  interfacefilename=argv[i+1]+2;
	  cout<<"Fichier Interface : "<<interfacefilename<<endl;
	}
      if((!strncmp(argv[i+1], "ALL", 3)))
	{
	  interfacefilename="ALL";
	  cout<<"Toutes les transitions sont obsérvées "<<endl;
	}
      if((!strncmp(argv[i+1], "EMPTY", 3)))
	{
	  interfacefilename="EMPTY";
	  cout<<"Aucune des transitions n'est obsérvée "<<endl;
	}

      cout<<"Fichier etats finaux : "<<argv[i+2]<<endl;
      cout<<"______________________________________\n";
      const char * c = interfacefilename.c_str();

      Subnets[j]=new net(argv[i],c,Int,argv[i+2]);
                    
      BddSubnets[j]=new RdPBDD(*Subnets[j],1,true);
    }
                
               
               
               					
                Modular_Obs_Graph Gv;
                bdd_gbc_hook(NULL);
                

		int algo=0;
		if(Soundness)
		  algo=1;

		if((!algo) && WeakSoundness && RelaxedSoundness)
		  algo=2;

		if((!algo) && Deadlock && RelaxedSoundness)
		  algo=3;

		if((!algo) && WeakSoundness)
		  algo=4;

		if((!algo) && RelaxedSoundness)
		  algo=5;

		if((!algo) && Deadlock)
		  algo=6;
		
		if((!algo) && EasySoundness)
		  algo=7;
                
		double TpsInit = (double)(clock()) / CLOCKS_PER_SEC;

		switch(algo)
		  {
		  case 1 :
		    {
		      BddSubnets[0]->Check_Soundness(Gv,NbSubnets,BddSubnets);
		      break;
		    }
		    /*case 2 :
		    {
		      BddSubnets[0]->Check_Weak_Relaxed_Soundness(Gv,NbSubnets,BddSubnets,0);
		      break;
		    }
		  case 3 :
		    {
		      BddSubnets[0]->Check_Deadlock_Relaxed_Soundness(Gv,NbSubnets,BddSubnets,0);
		      break;
		    }
		  case 4 :
		    {
		      BddSubnets[0]->Check_Weak_Soundness(Gv,NbSubnets,BddSubnets,0);
		      break;
		      }*/
		  case 5 :
		    {
		      BddSubnets[0]->Check_Relaxed_Soundness(Gv,NbSubnets,BddSubnets);
		      break;
		    }
		    /*case 6 :
		    {
		      BddSubnets[0]->Check_Deadlock(Gv,NbSubnets,BddSubnets,0);
		      break;
		    }
		  case 7 :
		    {
		      BddSubnets[0]->Check_Easy_Soundness(Gv,NbSubnets,BddSubnets,0);
		      break;
		      }*/
		  default:
		    {
		      BddSubnets[0]->GeneralizedSynchProduct_DSF(Gv,NbSubnets,BddSubnets,0);
		      break;
		    } 

		  }


                double TpsConstruct = ((double)(clock()) / CLOCKS_PER_SEC) - TpsInit;
                cout << "CONSTRUCTION / VERIFCATION TIME:    " << TpsConstruct<< endl;
                cout<<"GRAPHE D'OBSERVATION SYMBOLIQUE \n";
                Gv.printCompleteInformation(NbSubnets);
                bdd_done();
    
    return 1;
}
