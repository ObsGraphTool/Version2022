 // version 0.1
#include <time.h>
#include <iostream>
#include <string>
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
int Menu()
{
	int choix;
	cout<<"\t\t______________________________________________"<<endl;
	cout<<"\t\t|                                            |"<<endl;
	cout<<"\t\t|        OBSERVATION GRAPHE TOOL             |"<<endl;
	cout<<"\t\t|____________________________________________|\n\n"<<endl;
	cout<<"\tConstruction of the accessible space of states using OBDDs        : 1\n"<<endl;
	cout<<"\tConstruction of the observation graph with Canonize Dichotomous   : 2\n"<<endl;
    cout<<"\tGenerate Synchronized SOG in mode on-the-fly                      : 3\n"<<endl;
    cout<<"\tGenerate Synchronized Canonized SOG                               : 4\n"<<endl;
    cout<<"\tChecking soundness                                                : 5\n"<<endl;
    cout<<"\tChecking Compatibility via soundness                              : 6\n"<<endl;
    cout<<"\tQUIT : 0"<<endl;
    cout<<"\t___________________________________________________________________"<<endl;
    cout<<"\t|           *Please indicate your selected function*               |"<<endl;
    cout<<"\t___________________________________________________________________"<<endl;
	cin>>choix;
	return choix;
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

//char *
/*void parserGML(char* FileGML)
 {
 TiXmlDocument doc(FileGML );
 doc.LoadFile();
 }*/
/***********************************************/
int main(int argc, char** argv) {
    int choix;
    const char* Int="";

    //   do
    {
        choix=Menu();
        //choix=2;
        switch(choix)
        {
            case 1 :{
                if (argc<2)
                    return 0;
                int b = 32;
                if (argc>2) b = atoi(argv[argc-1]);
                net R(argv[1]);
                RdPBDD DR(R, b);
                cout << "parser...done" << endl;
                // no print when garbage collection operates
                bdd_gbc_hook(NULL);
                //	reordering(R.places.size()*2);
                //	bdd_reorder(BDD_REORDER_RANDOM);
                double d, tps;
                bdd M2;
                d = getTime();
                M2 = DR.ReachableBDD1();
                tps = getTime() - d;
                cout << argv[1] << " \n Nb states " << bdd_satcountset(M2,DR.currentvar);
                cout << " \nNb bdd node :" << bdd_nodecount(M2)<<endl; // nombre de noeuds
                // cout << " \nNb bdd edge :" << bdd_edgecount(M2)<<endl; // nombre de noeuds
                
                cout << " Ex Time : " << tps << endl;
                
                // if (argc>3)
                // cout << fddset << M2 << endl;
                break;
            }
            case 2:{
                // Variables
                // time_t tbegin,tend;
                // double texec=0.;
                
                int b = 32;
                
                if(argc<3)
                    return 0;
                char Obs[900000]="";
               // char Int[100000]="";
                char PlacesF[1000000]="";
                
                if(argc>3)
                {
                    strcpy(Obs,argv[2]);
                }
                if(argc>4)
                    strcpy(PlacesF,argv[3]);
               // if(argc>5)
                 //   strcpy(Int,argv[4]);
                
                
                
                b = atoi(argv[argc-1]);
            
            //    cout<<"b=1"<<b<<endl;
                cout<<"Fichier net : "<<argv[1]<<endl;
                cout<<"Fichier des actions observe : "<<Obs<<endl;
                //cout<<"Fichier Interface : "<<Int<<endl;
                cout<<"Fichier etats finaux : "<<PlacesF<<endl;
                cout<<"______________________________________\n";
                // parserGML("testXML");
                //tbegin=time(NULL);              // get the current calendar time
                //cout<<"tbgin= "<<tbegin<<endl;
                clock_t start;
                start = clock();
                //cout<<"mnt il est "<<start<<endl;

                net R(argv[1],Obs,Int,PlacesF);
                //cout<<"HEYOOO "<<argv[1];
                //	  net R(argv[1],Obs,Int,PlacesF);
                
                //cout<<"heeeeeeeeeeeeeeeeeere :\n" << R;
                
                cout<<"______________Apres construction de net________________________\n";
                //cout<<R<<endl;
                double d,tps;
                RdPBDD DR(R, b);
                
                //cout<<"im here \n";
                MDGraph g;
                // no print when garbage collection operates
                bdd_gbc_hook(NULL);
                //	reordering(R.places.size()*2);
                //	bdd_reorder(BDD_REORDER_RANDOM);
                double temps;
                
                // d = getTime();
                //DR.compute_canonical_deterministic_graph_Opt(g);
               DR.compute_canonical_deterministic_graph_OptProf(g);

                // tps = getTime() - d;
                //tend=time(NULL);                // get the current calendar time
                //cout<<"tend= "<<tend<<endl;
                //texec=difftime(tend,tbegin);    // tend-tbegin (result in second)
                
                d=(double)(clock()-start) / (double)CLOCKS_PER_SEC;
                //cout<<"temps est :"<<texec<<endl;
                
                cout << "Temps de construction du GO" << d << endl;
   //  g.reachfinal();
//cout<<"PASS11"<<endl;
     //    g.checksoundness();
cout<<"PASS"<<endl;                
                g. printCompleteInformation();
                
                break;
            }
            case 3  :{
                cout<<"Entering Function 3 ..."<<endl;
                char Int[100]="";

                int NbSubnets=(argc-1)/3;
                cout<<"nombre de sous-reseaux(Number of subnets) : "<<(argc-1)/3<<endl;
                //int toto;
                //cin>>toto;
                net **Subnets=new net*[NbSubnets];
                RdPBDD **BddSubnets=new RdPBDD*[NbSubnets];
                bdd_init(1000000,10000);
                //char Int[100]="";
                
                // int NbSubnets=(argc-2)/3;
                 cout<<"nombre de sous-r�seaux (Number of subnets): "<<NbSubnets<<endl;
                 /*//int toto;
                 //cin>>toto;
                 net **Subnets=new net*[NbSubnets];
                 RdPBDD **BddSubnets=new RdPBDD*[NbSubnets];
                 bdd_init(1000000,10000);
                 char Int[100]="";*/
                
                for(int i=1,j=0;i<=argc-3;i=i+3,j++)
                {
                 //   cout<<"i= "<<i<<endl;
                   // cout<<"argc-3= "<<argc-3<<endl;
                    cout<<"\n________ Subnet(sous-reseau) No."<<j+1<<"__________"<<endl;
                    cout<<"File of net | Fichier net : "<<argv[i]<<endl;
                   // cout<<"Fichier formule : "<<argv[argc-1]<<endl;
                    cout<<"File of Interface |Fichier Interface : "<<argv[i+1]<<endl;
                    cout<<"File of Final state | Fichier etats finaux : "<<argv[i+2]<<endl;
                    cout<<"______________________________________\n";
                    Subnets[j]=new net(argv[i],argv[i+1],Int,argv[i+2]);
                  //  net R(argv[1],Obs,Int,PlacesF);
                    //  Subnets[j]=new net(argv[i],argv[argc-1],Int,argv[i+1]);
                    // cout<<"Subnets["<<j<<"]=new net("<<argv[i]<<","<<argv[argc-1]<<","<<Int<<","<<argv[i+1]<<");"<<endl;
                    BddSubnets[j]=new RdPBDD(*Subnets[j],1,true);
                }
                
                
                /*  for(int i=1,j=0;i<argc-3;i=i+3,j++)
                 {
                 cout<<"________ sous-reseau _______________"<<j+1<<endl;
                 Subnets[j]=new net(argv[i],argv[argc-1],argv[i+1],argv[i+2]);
                 //  Subnets[j]=new net(argv[i],argv[argc-1],Int,argv[i+1]);
                 cout<<"Subnets["<<j<<"]=new net("<<argv[i]<<","<<argv[argc-1]<<","<<Int<<","<<argv[i+1]<<");"<<endl;
                 
                 BddSubnets[j]=new RdPBDD(*Subnets[j],1,true);
                 }
                 */
                //for(int k=0;k<NbSubnets;k++)
                //{
                //cout<<*Subnets[k]<<endl;
                //cin>>toto;
                //}
                int NbBddVar=Subnets[0]->nbPlace();
                //cout<<"NB BDD VAR : "<<NbBddVar<<endl;
                //cout<<"<NbSubnets : "<<NbSubnets<<endl;
                for(int i=0;i<NbSubnets;i++)
                {
                    //cout<<"\nSous-reseau "<<i<<endl;
                    //cout<<"son nb places : "<<Subnets[i]->nbPlace()<<endl;
                    if(Subnets[i]->nbPlace()>NbBddVar)
                        NbBddVar=Subnets[i]->nbPlace();
                }
                //cin>>toto;
                // bdd_setvarnum(NbBddVar);
                Modular_Obs_Graph Gv;
                // no print when garbage collection operates
                bdd_gbc_hook(NULL);
                //	reordering(R.places.size()*2);
                //	bdd_reorder(BDD_REORDER_RANDOM);
                double TpsInit = (double)(clock()) / CLOCKS_PER_SEC;
                BddSubnets[0]->GeneralizedSynchProduct1(Gv,NbSubnets,BddSubnets,NbBddVar,1);
                
                double TpsConstruct = ((double)(clock()) / CLOCKS_PER_SEC) - TpsInit;
                cout << "SYNCHRONIZATION TIME:    " << TpsConstruct<< endl;
                cout<<"GRAPHE DE LA FORMULE \n";
                Gv.printCompleteInformation(NbSubnets);
                bdd_done();
                break;
            }
            case 4  :
            {
                int NbOB=(argc-1);
                cout<<"nbOB :"<<NbOB<<endl;
                Simple_MDGraph **Gv=new Simple_MDGraph*[NbOB];
                for(int i=0;i<NbOB;i++)
                    Gv[i]=new Simple_MDGraph((argv[i+1]));
                Simple_MDGraph Res;
                for(int i=0;i<NbOB;i++)
                    Res=Res.computeResGraph(*Gv[i]);
                char c;
                cout<<"Save the composed ObsGraph on file? (y/n) \n";
                cin>>c;
                if(c=='y'||c=='Y')
                {
                    string filename;
                    cout<<"Enter the file's name: ";
                    cin>>filename;
                    Res.saveonfile(filename);
                }
                
                break;
            }
            case 5:{
                // Variables
                // time_t tbegin,tend;
                // double texec=0.;
                
                int b = 32;
                
                if(argc<3)
                    return 0;
                char Obs[900000]="";
                char Int[100000]="";
                char PlacesF[100000]="";
                
                if(argc>3)
                {
                    strcpy(Obs,argv[2]);
                }
                if(argc>4)
                   strcpy(PlacesF,argv[3]);
                
                if(argc>5)
                    strcpy(Int,argv[4]);

                
                b = atoi(argv[argc-1]);
                
                //    cout<<"b=1"<<b<<endl;
                cout<<"Fichier net : "<<argv[1]<<endl;
                cout<<"Fichier formule : "<<Obs<<endl;
                cout<<"Fichier Interface : "<<Int<<endl;
                cout<<"Fichier etats finaux : "<<PlacesF<<endl;
                cout<<"______________________________________\n";
                clock_t start;
                start = clock();
                //cout<<"mnt il est "<<start<<endl;
              // net R(argv[1],Obs,Int);

               net R(argv[1],Obs,Int,PlacesF);
                //cout<<"HEYOOO "<<argv[1];
                //	  net R(argv[1],Obs,Int,PlacesF);
                
                //cout<<"heeeeeeeeeeeeeeeeeere :\n" << R;
                
                cout<<"______________Apres construction de net________________________\n";
                //cout<<R<<endl;
                double d,tps;
                RdPBDD DR(R, b);
                
                //cout<<"im here \n";
                MDGraph g;
                // no print when garbage collection operates
                bdd_gbc_hook(NULL);
                //	reordering(R.places.size()*2);
                //	bdd_reorder(BDD_REORDER_RANDOM);
                double temps;
                
                // d = getTime();
                //DR.compute_canonical_deterministic_graph_Opt(g);
                DR.compute_canonical_deterministic_graph_OptProf(g);

                // tps = getTime() - d;
                //tend=time(NULL);                // get the current calendar time
                //cout<<"tend= "<<tend<<endl;
                //texec=difftime(tend,tbegin);    // tend-tbegin (result in second)
                
                d=(double)(clock()-start) / (double)CLOCKS_PER_SEC;
                //cout<<"temps est :"<<texec<<endl;
                
                cout << "Temps de construction du GO" << d << endl;
                 //g.reachfinal();
                //cout<<"PASS11"<<endl;
                    g.checksoundness();
                    g.checkrelaxedsoundness();
		    g.checkweaksoundness();
		    g.checkeasysoundness();

                //cout<<"PASS"<<endl;
                g. printCompleteInformation();
                              
                

              
                
                break;
}
            case 6  :{
                
                char Int[100]="";

                int NbSubnets=(argc-1)/3;
                cout<<"nombre de sous-reseaux : "<<(argc-1)/3<<endl;
                
                
                //int toto;
                //cin>>toto;
                net **Subnets=new net*[NbSubnets];
                RdPBDD **BddSubnets=new RdPBDD*[NbSubnets];
                bdd_init(1000000,10000);
                //char Int[100]="";
                
                /* int NbSubnets=(argc-2)/3;
                 cout<<"nombre de sous-r�seaux : "<<NbSubnets<<endl;
                 //int toto;
                 //cin>>toto;
                 net **Subnets=new net*[NbSubnets];
                 RdPBDD **BddSubnets=new RdPBDD*[NbSubnets];
                 bdd_init(1000000,10000);
                 char Int[100]="";*/
                
              /*  for(int i=1,j=0;i<argc-3;i=i+3,j++)
                {		
                    cout<<"\n________ sous-reseau _______________"<<j+1<<endl;
                    cout<<"Fichier net : "<<argv[i]<<endl;
                    cout<<"Fichier formule : "<<argv[argc-1]<<endl;
                    cout<<"Fichier Interface : "<<argv[i+1]<<endl;
                    cout<<"Fichier etats finaux : "<<argv[i+2]<<endl;
                    cout<<"______________________________________\n";
                    Subnets[j]=new net(argv[i],argv[argc-1],argv[i+1],argv[i+2]);
                    //  Subnets[j]=new net(argv[i],argv[argc-1],Int,argv[i+1]);
                    // cout<<"Subnets["<<j<<"]=new net("<<argv[i]<<","<<argv[argc-1]<<","<<Int<<","<<argv[i+1]<<");"<<endl;
                    
                    BddSubnets[j]=new RdPBDD(*Subnets[j],1,true);
                }
                */
                for(int i=1,j=0;i<=argc-3;i=i+3,j++)
                {
                    //   cout<<"i= "<<i<<endl;
                    // cout<<"argc-3= "<<argc-3<<endl;
                    cout<<"\n________ sous-reseau _______________"<<j+1<<endl;
                    cout<<"Fichier net : "<<argv[i]<<endl;
                    // cout<<"Fichier formule : "<<argv[argc-1]<<endl;
                    cout<<"Fichier Interface : "<<argv[i+1]<<endl;
                    cout<<"Fichier etats finaux : "<<argv[i+2]<<endl;
                    cout<<"______________________________________\n";
                    Subnets[j]=new net(argv[i],argv[i+1],Int,argv[i+2]);
                    //  net R(argv[1],Obs,Int,PlacesF);
                    //  Subnets[j]=new net(argv[i],argv[argc-1],Int,argv[i+1]);
                    // cout<<"Subnets["<<j<<"]=new net("<<argv[i]<<","<<argv[argc-1]<<","<<Int<<","<<argv[i+1]<<");"<<endl;
                    BddSubnets[j]=new RdPBDD(*Subnets[j],1,true);
                }
                
                
                /*  for(int i=1,j=0;i<argc-3;i=i+3,j++)
                 {		
                 cout<<"________ sous-reseau _______________"<<j+1<<endl;
                 Subnets[j]=new net(argv[i],argv[argc-1],argv[i+1],argv[i+2]);
                 //  Subnets[j]=new net(argv[i],argv[argc-1],Int,argv[i+1]);
                 cout<<"Subnets["<<j<<"]=new net("<<argv[i]<<","<<argv[argc-1]<<","<<Int<<","<<argv[i+1]<<");"<<endl;
                 
                 BddSubnets[j]=new RdPBDD(*Subnets[j],1,true);
                 }
                 */
                //for(int k=0;k<NbSubnets;k++)
                //{
                //cout<<*Subnets[k]<<endl;
                //cin>>toto;
                //}
                int NbBddVar=Subnets[0]->nbPlace();
                //cout<<"NB BDD VAR : "<<NbBddVar<<endl;
                //cout<<"<NbSubnets : "<<NbSubnets<<endl;
                for(int i=0;i<NbSubnets;i++)
                {
                    //cout<<"\nSous-reseau "<<i<<endl;
                    //cout<<"son nb places : "<<Subnets[i]->nbPlace()<<endl;
                    if(Subnets[i]->nbPlace()>NbBddVar)
                        NbBddVar=Subnets[i]->nbPlace();
                }
                
                
                
                //cin>>toto;
                // bdd_setvarnum(NbBddVar);					
                Modular_Obs_Graph Gv;
                // no print when garbage collection operates
                bdd_gbc_hook(NULL);
                //	reordering(R.places.size()*2);
                //	bdd_reorder(BDD_REORDER_RANDOM);
                
                double TpsInit = (double)(clock()) / CLOCKS_PER_SEC;
                //BddSubnets[0]->GeneralizedSynchProduct1(Gv,NbSubnets,BddSubnets,NbBddVar,0);
		 BddSubnets[0]->GeneralizedSynchProduct_Prof(Gv,NbSubnets,BddSubnets,NbBddVar,0);

                Gv.reachfinal(NbSubnets);
                Gv.checksoundness();
                Gv.checkweaksoundness();
                Gv.checkeasysoundness();

                double TpsConstruct = ((double)(clock()) / CLOCKS_PER_SEC) - TpsInit;
                cout << "SYNCHRONIZATION TIME:    " << TpsConstruct<< endl;
                cout<<"GRAPHE DE LA FORMULE \n";
                Gv.printCompleteInformation(NbSubnets);
                bdd_done();
                break;
            }
        }
    }//while(choix);
    return 1;
}
