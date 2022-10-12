#include <time.h>
#include <iostream>
#include <string>
#include <ctime>
#include "bdd.h"
#include "fdd.h"
#include "Net.h"
#include "RdPBDD.h"
#include "Simple_MDGraph.h"
#include <cstring>
using namespace std;

//#include "tinyxml.h"
//#include <tinystr.h>
int Formula_transitions(const char *f, Set_mot &formula_trans, net Rv);

bdd EmersonLey(bdd, net);

double getTime() {
    return (((double) clock()) / (double) CLOCKS_PER_SEC);
}


void reordering(int nbvar) {

    // all the variables belong to a main block
    bdd_varblockall();
    // to each fdd variable corresponds a block
    for (int i = 0; i < nbvar; i++)
        fdd_intaddvarblock(i, i, BDD_REORDER_FIXED);

    bdd_autoreorder(BDD_REORDER_SIFT);

    bdd_reorder_verbose(2);
}


int main()
{
    int Nbservice;
    cout<<"\t\t______________________________________________"<<endl;
    cout<<"\t\t|                Welcome to                  |"<<endl;
    cout<<"\t\t|          OBSERVATION GRAPH TOOL            |"<<endl;
    cout<<"\t\t|____________________________________________|\n\n"<<endl;
    cout<<"\tPlease indicate the number of service you have: ";
    cout<<"\tQUIT : 0"<<endl;
    cin>>Nbservice;
    int nf=3*Nbservice;
    string strservice[nf+3];git 
    cout<<"There are * "<<nf<<" * files of "<<Nbservice<<" service to be input! Please follow the following instructions:"<<endl<<endl;
    int NbSubnets=Nbservice;
    net **Subnets=new net*[NbSubnets];
    RdPBDD **BddSubnets=new RdPBDD*[NbSubnets];
    bdd_init(1000000,10000);
    char Int[100]="";

    for (int i=1,j=0; i<nf+1;i=i+3,j++)
    {
        cout<<"Please enter the address of *The net file* for the Service No."<<j+1<<endl;
        cin>>strservice[i];
        cout<<"Please enter the address of *The Observed action* for the Service No."<<j+1<<endl;
        cin>>strservice[i+1];
        cout<<"Please enter the address of *The Final state file * for the Service No."<<j+1<<endl;
        cin>>strservice[i+2];
        cout<<"File input finished, constructing Subnets..."<<endl;
        Subnets[j]=new net(strservice[i].c_str(), strservice[i+1].c_str(), Int, strservice[i+2].c_str());
        BddSubnets[j]=new RdPBDD(*Subnets[j],1,true);
    }





    int NbBddVar=Subnets[0]->nbPlace();
    for(int i=0;i<NbSubnets;i++)
    {

        if(Subnets[i]->nbPlace()>NbBddVar)
            NbBddVar=Subnets[i]->nbPlace();
    }

    Modular_Obs_Graph Gv;
    bdd_gbc_hook(NULL);
    double TpsInit = (double)(clock()) / CLOCKS_PER_SEC;
    BddSubnets[0]->GeneralizedSynchProduct1(Gv,NbSubnets,BddSubnets,NbBddVar,1);




    double TpsConstruct = ((double)(clock()) / CLOCKS_PER_SEC) - TpsInit;
    cout << "SYNCHRONIZATION TIME:    " << TpsConstruct<< endl;
    cout<<"GRAPHE DE LA FORMULE \n";
    Gv.printCompleteInformation(NbSubnets);
    bdd_done();

    cout<<endl<<"\nWould you like to substitute a Service?(Y/N)"<<endl;
    char c;
    int selectservice;
    cin>>c;
     if(c=='y'||c=='Y')
    {
         cout<<"Please indicate the number of the service you want to substitute"<<endl;
         cin>>selectservice;
         cout<<"You have selected service  **"<<selectservice<<"**"<<endl;
         cout<<"Please input files for service to be substituted:"<<endl;
        cout<<"Please enter the address of *The net file* for the Alternative Service "<<endl;
        cin>>strservice[3*selectservice-2];
        cout<<"Please enter the address of *The Observed action* for the Alternative Service"<<endl;
        cin>>strservice[3*selectservice-1];
        cout<<"Please enter the address of *The Final state file * for the Alternative Service"<<endl;
        cin>>strservice[3*selectservice];

        for (int i=1,j=0; i<nf+1;i=i+3,j++)
        {

            Subnets[j]=new net(strservice[i].c_str(), strservice[i+1].c_str(), Int, strservice[i+2].c_str());
            BddSubnets[j]=new RdPBDD(*Subnets[j],1,true);
        }



        bdd_done();


        int NbBddVar=Subnets[0]->nbPlace();
        for(int i=0;i<NbSubnets;i++)
        {

            if(Subnets[i]->nbPlace()>NbBddVar)
                NbBddVar=Subnets[i]->nbPlace();
        }

        Modular_Obs_Graph Gv;
        bdd_gbc_hook(NULL);
        double TpsInit = (double)(clock()) / CLOCKS_PER_SEC;
        BddSubnets[0]->GeneralizedSynchProduct1(Gv,NbSubnets,BddSubnets,NbBddVar,1);




        double TpsConstruct = ((double)(clock()) / CLOCKS_PER_SEC) - TpsInit;
        bdd_done();



    }
     else if (c=='n'||c=='N') {
         cout << "Thank you for using the SoG tool!!!" << endl;
     }
     else
         cout<<"Invalid input"<<endl;
    return 0;
}





