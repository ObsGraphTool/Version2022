/* -*- C++ -*- */
#include <string>
#include <iostream>
//#include <set>
#include <vector>
#include <map>
#include <stack>
#include <ext/hash_map>
#include <bdd.h>
#include <math.h>
#include "bvec.h"
#include "RdPBDD.h"
#include "Class_of_state.h"

#include <cstdlib>
#include <sstream>
#include <algorithm>
//#include <stdlib>
using namespace std;
int NbIt;
int pc=1;
int opToC=1;
int itext,itint;
int MaxIntBdd;
bdd *TabMeta;
int nbmetastate;
double old_size;
const vector<class Place> *vplaces = NULL;
void my_error_handler(int errcode) {
	if (errcode == BDD_RANGE) {
		// Value out of range : increase the size of the variables...
cout<<"EROR"<<endl;		// but which one???
		bdd_default_errhandler(errcode);
	}
	else {
		bdd_default_errhandler(errcode);
	}
}

/*****************************************************************/
/*                         printhandler                          */
/*****************************************************************/
void printhandler(ostream &o, int var)
{
	o << (*vplaces)[var/2].name;
	if (var%2)
		o << "_p";
}

/*****************************************************************/
/*                         class Trans                           */
/*****************************************************************/
Trans::Trans(const bdd& v, bddPair* p, const bdd& r,const bdd& pr, const bdd& pre, const bdd& post): var(v),pair(p),rel(r),prerel(pr),Precond(pre),Postcond(post) {
}
//Franchissement avant
bdd Trans::operator()(const bdd& n) const {
	bdd res = bdd_relprod(n,rel,var);
	res = bdd_replace(res, pair);
	return res;
}
//Franchissement arriere
bdd Trans::operator[](const bdd& n) const {
	bdd res = bdd_relprod(n,prerel,var);
	res = bdd_replace(res, pair);
	return res;
}

/*****************************************************************/
/*                         class RdPBDD                          */
/*****************************************************************/

RdPBDD::RdPBDD(const net &R, int BOUND,bool init){
    cout<<"RdPBDD Constructing..."<<endl;
    int nbPlaces=R.places.size(), i, domain;
	vector<Place>::const_iterator p;
    DeadRdp=false;
    
    
	bvec *v = new bvec[nbPlaces];
	bvec *vp = new bvec[nbPlaces];
	bvec *prec = new bvec[nbPlaces];
	bvec *postc = new bvec[nbPlaces];
	int *idv = new int[nbPlaces];
	int *idvp = new int[nbPlaces];
	int *nbbit = new int[nbPlaces];
	if(!init)
        bdd_init(1000000,10000);
	// the error handler
	bdd_error_hook((bddinthandler)my_error_handler);
	//_______________
	transitions=R.transitions;
	Observable=R.Observable;
	NonObservable=R.NonObservable;
	Formula_Trans=R.Formula_Trans;
	Final_State=R.Final_State;
	Final_States=R.Final_States;
	transitionName=R.transitionName;
	InterfaceTrans=R.InterfaceTrans;
	Nb_places=R.places.size();
cout<<"Number of places  : "<<Nb_places<<endl;
  //  cout<<"herehh0"<<endl;

	cout<<"Final place is : "<<R.places[Nb_places-1].name<<endl;

	for(i=0,p=R.places.begin();p!=R.places.end();i++,p++) {
		if (p->hasCapacity()) {
			domain = p->capacity+1;
		}
		else {
			domain = BOUND+1; // the default domain
		}
		//cout<<"domaine: "<<domain<<endl;
		// variables are created one by one (implying contigue binary variables)
		fdd_extdomain(&domain, 1);
		//cout<<"nb created var : "<<bdd_varnum()<<endl;
		fdd_extdomain(&domain, 1);
		//cout<<"nb created var : "<<bdd_varnum()<<"domain "<<domain<<endl;
    }  //  cout<<"here00"<<endl;

    
	// bvec
	currentvar = bdd_true();
	for(i=0;i<nbPlaces;i++) {
		nbbit[i] = fdd_varnum(2*i);
		//cout<<"nb var pour "<<2*i<<" = "<<fdd_domainsize(2*i)<<endl;
		v[i]=bvec_varfdd(2*i);
		vp[i]=bvec_varfdd(2*i+1);
		//cout<<"nb var pour "<<2*i+1<<" = "<<fdd_domainsize(2*i+1)<<endl;
		currentvar = currentvar & fdd_ithset(2*i);
	}
   // cout<<"here0"<<endl;

    
	// initial marking
    M0=bdd_true();
    // cout<<"ici"<<endl;
   	for(i=0,p=R.places.begin();p!=R.places.end();i++,p++)
    {
        //cout<<"+++++++ i = "<<i<<" et p = "<<p->marking<<endl;
        M0 = M0 & fdd_ithvar(2*i,p->marking);
    }
  //  cout<<"initial state :"<<endl;
    //cout<<bddtable<<M0<<endl;
	//etats finaux
    //cout<<"here"<<endl;
    
    Set::const_iterator t;
   // finalstate=bdd_true();
  
    set<Set>::const_iterator k=R.Final_States.begin() ;
   
    do {
		 bdd finalstatetemp=bdd_true();
	    for(i=0,p=R.places.begin();p!=R.places.end();i++,p++)
        {
            int temp=0;
            
            for(Set::const_iterator h=(*k).begin();!(h==(*k).end())&&!temp;h++)
            { //cout<<"+++++++ i = "<<i<<" et p = "<<(*p).name<<" et t = "<<R.places[(*h)].name<<endl;
                
                if (((*p).name).compare(R.places[(*h)].name)==0)
                    temp=1;
                
            }
           // teststate = teststate & fdd_ithvar(2*i,0);
            finalstatetemp = finalstatetemp & fdd_ithvar(2*i,temp);
            
        }
      
        finalstate = finalstatetemp|finalstate ;
      
        k++;
    }while(k!=R.Final_States.end());
    
  /*  bdd a= bdd_true();
    a= a & fdd_ithvar(0,1);
        a= a & fdd_ithvar(2,1);
            a= a & fdd_ithvar(4,1);
            cout<<"a:"<<endl;
    cout<<bddtable<<a<<endl;
bdd b= bdd_true();
    b= b & fdd_ithvar(0,0);
        b= b & fdd_ithvar(2,1);
            b= b & fdd_ithvar(4,1);
            cout<<"b:"<<endl;
    cout<<bddtable<<b<<endl;
            bdd c;//= bdd_true();
            c=a|c;
            cout<<"c:"<<endl;
    cout<<bddtable<<c<<endl;
            c=b|c;
         cout<<"c:"<<endl;
    cout<<bddtable<<c<<endl;*/
     //cout<<"final state :"<<endl;
    //cout<<bddtable<<finalstate<<endl;
     /* cout<<"test state :"<<endl;
    cout<<bddtable<<teststate<<endl;
    cout<<"bdd_apply pas inferieur"<<endl;
    cout<<bddtable<<(finalstate<teststate)<<endl;
        cout<<bddtable<<(teststate<finalstate)<<endl;
        cout<<bddtable<<bdd_apply(teststate,finalstate,bddop_less)<<endl;
        cout<<bddtable<<bdd_apply(finalstate,teststate,bddop_less)<<endl;*/

//    cout<<"here2"<<endl;

	// place names
	vplaces = &R.places;
	fdd_strm_hook(printhandler);
    
    
    /* Transition relation */
    for(vector<Transition>::const_iterator t=R.transitions.begin();
        t!=R.transitions.end();t++){
		int np=0;
		bdd rel = bdd_true(), var = bdd_true(),prerel=bdd_true();
		bdd Precond=bdd_true(),Postcond=bdd_true();
		bddPair *p=bdd_newpair();
		
		for(i=0;i<nbPlaces;i++) {
			prec[i]=bvec_con(nbbit[i],0);
			postc[i]=bvec_con(nbbit[i],0);
		}
        
		// calculer les places adjacentes a la transition t
		// et la relation rel de la transition t
		set<int> adjacentPlace;
		
		// arcs pre
		for(vector< pair<int,int> >::const_iterator it=t->pre.begin();it!=t->pre.end(); it++) {
			adjacentPlace.insert(it->first);
			prec[it->first] = prec[it->first] + bvec_con(nbbit[it->first], it->second);
		}
		// arcs post
		for(vector< pair<int,int> >::const_iterator it=t->post.begin();it!=t->post.end(); it++) {
			adjacentPlace.insert(it->first);
			postc[it->first] = postc[it->first] + bvec_con(nbbit[it->first], it->second);
		}
		// arcs pre automodifiants
		for(vector< pair<int,int> >::const_iterator it=t->preAuto.begin();it!=t->preAuto.end(); it++) {
			adjacentPlace.insert(it->first);
			prec[it->first] = prec[it->first] + v[it->second];
		}
		// arcs post automodifiants
		for(vector< pair<int,int> >::const_iterator it=t->postAuto.begin();it!=t->postAuto.end(); it++) {
			adjacentPlace.insert(it->first);
			postc[it->first] = postc[it->first] + v[it->second];
		}
		// arcs reset
		for(vector<int>::const_iterator it=t->reset.begin();it!=t->reset.end(); it++) {
			adjacentPlace.insert(*it);
			prec[*it] = prec[*it] + v[*it];
		}
		
		np=0;
		for(set<int>::const_iterator it=adjacentPlace.begin();it!=adjacentPlace.end(); it++) {
			idv[np]=2*(*it);
			idvp[np]=2*(*it)+1;
			var = var & fdd_ithset(2*(*it));
			//Image
			// precondition
			rel = rel & (v[*it] >= prec[*it]);
			Precond = Precond & (v[*it] >= prec[*it]);
			// postcondition
			rel = rel & (vp[*it] == (v[*it] - prec[*it] + postc[*it]));
			// Pre image __________
			// precondition
			prerel = prerel & (v[*it] >= postc[*it]);
			// postcondition
			Postcond=Postcond & (v[*it]>=postc[*it]);
			prerel = prerel & (vp[*it] == (v[*it] - postc[*it] + prec[*it]));
			//___________________
			// capacit\E9
			if (R.places[*it].hasCapacity())
				rel = rel & (vp[*it] <= bvec_con(nbbit[*it], R.places[*it].capacity));
			np++;
		}
		fdd_setpairs(p, idvp, idv, np);
//		cout<<"heere1"<<endl;
		// arcs inhibitor
		for(vector< pair<int,int> >::const_iterator it=t->inhibitor.begin();it!=t->inhibitor.end(); it++)
			rel = rel & (v[it->first]< bvec_con(nbbit[it->first], it->second));
        
		relation.push_back(Trans(var, p, rel,prerel,Precond,Postcond));
    }
    //cout<<"ici2"<<endl;
	delete [] v;
	delete [] vp;
	delete [] prec;
	delete [] postc;
	delete [] idv;
	delete [] idvp;
	delete [] nbbit;
	cout<<"Construction Complete!!!"<<endl<<endl<<endl;
}

/*----------------------------------- Reachability space using bdd ----------*/
bdd RdPBDD:: ReachableBDD1()
{
    bdd M1;
    bdd M2=M0;
    double d,tps;
    d=(double)clock() / (double)CLOCKS_PER_SEC;
    NbIt=0;
    MaxIntBdd=bdd_nodecount(M0);
    while(M1!=M2){
		M1=M2;
		for(vector<Trans>::const_iterator i = relation.begin();
            i!=relation.end();i++) {
			bdd succ = (*i)(M2);
			M2=succ|M2;
			//cout << bdd_nodecount(succ) << endl;
			//if(succ!=bddfalse)
		}
		NbIt++;
		int Current_size=bdd_nodecount(M2);
		if(MaxIntBdd<Current_size)
            MaxIntBdd=Current_size;
		//cout<<"Iteration numero "<<NbIt<<" nb node de reached :"<<bdd_nodecount(M2)<<endl;
        //		cout << bdd_nodecount(M2) << endl;
	}
	cout << endl;
	tps = ((double)(clock()) / CLOCKS_PER_SEC) - d;
	cout<<"-----------------------------------------------------\n";
	cout << "CONSTRUCTION TIME  " << tps << endl;
	cout<<"Max Intermediary BDD "<<MaxIntBdd<<endl;
	cout<<"Nombre d'iteration : "<<NbIt<<endl;
	//bdd Cycle=EmersonLey(M2,0);
	//cout<<Cycle.id()<<endl;
	return M2;
}
bdd RdPBDD:: ReachableBDD2()
{
    bdd M1;
    bdd M2=M0;
    double d,tps;
    d=(double)clock() / (double)CLOCKS_PER_SEC;
    NbIt=0;
    MaxIntBdd=bdd_nodecount(M0);
    while(M1!=M2){
		M1=M2;
		bdd Reached;
		for(vector<Trans>::const_iterator i = relation.begin();
            i!=relation.end();i++) {
			bdd succ = (*i)(M2);
			Reached=succ|Reached;
			//cout << bdd_nodecount(succ) << endl;
			//if(succ!=bddfalse)
		}
		NbIt++;
		M2=M2|Reached;
		int Current_size=bdd_nodecount(M2);
		if(MaxIntBdd<Current_size)
            MaxIntBdd=Current_size;
		//cout<<"Iteration numero "<<NbIt<<" nb node de reached :"<<bdd_nodecount(M2)<<endl;
        //		cout << bdd_nodecount(M2) << endl;
	}
	cout << endl;
	tps = ((double)(clock()) / CLOCKS_PER_SEC) - d;
	cout<<"-----------------------------------------------------\n";
	cout << "CONSTRUCTION TIME  " << tps << endl;
	cout<<"Max Intermediary BDD "<<MaxIntBdd<<endl;
	cout<<"Nombre d'iteration : "<<NbIt<<endl;
	return M2;
}
bdd RdPBDD:: ReachableBDD3()
{
    double d,tps;
    d=(double)clock() / (double)CLOCKS_PER_SEC;
	bdd New,Reached,From;
	Reached=From=M0;
	NbIt=0;
	do{
        NbIt++;
        bdd succ;
        for(vector<Trans>::const_iterator i = relation.begin();	   	i!=relation.end();i++)
            succ=(*i)(From)|succ;
  	    New=succ-Reached;
	    From=New;
	    Reached=Reached | New;
	    cout<<"Iteration numero "<<NbIt<<" nb node de reached :"<<bdd_nodecount(Reached)<<endl;
	}while(New!=bddfalse);
	tps=(double)clock() / (double)CLOCKS_PER_SEC-d;
	cout << "TPS CONSTRUCTION : "<<tps<<endl;
	return Reached;
}
/*----------------Fermeture transitive sur les transitions non observ\E9es ---*/
bdd RdPBDD::Accessible_epsilon2(bdd Init)
{
    
    bdd Reached,New,From;
    Reached=From=Init;
    do{
        bdd succ;
        for(Set::const_iterator i=NonObservable.begin();!(i==NonObservable.end());i++)
            
            succ= relation[(*i)](From)|succ;
  	    New=succ-Reached;
	    From=New;
	    Reached=Reached | New;
	}while(New!=bddfalse);
	cout << endl;
	return Reached;
}
bdd RdPBDD::Accessible_epsilon(bdd From)
{
    bdd M1;
    bdd M2=From;
    //cout<<"final state"<<endl;
    //cout<<bddtable<<finalstate<<endl;

   // cout<<"hhhh Accessible_epsilon"<<endl;
    /* cout<<bddtable<<From<<endl;
     cout<<"finhhhh"<<endl;*/
    
    //if((From-finalstate)!=bddfalse) {if((From-finalstate)!=From) pc=0;}

   // if((bdd_apply(From,finalstate,<))!=bddfalse) 
   /* if(((From<finalstate)==bddfalse)&&(From!=finalstate))
    {
		cout<<"bdd_apply pas inferieur"<<endl;
    cout<<bddtable<<(From<finalstate)<<endl;
		pc=0;}*/
    //else
    /*cout<<"continue pc"<<pc<<endl;
     cout<<bddtable<<(From-finalstate)<<endl;
     cout<<"fin<<<<"<<endl;*/
    //if(From>=finalstate) cout<<"final state < From"<<endl;
    //if(From<finalstate) cout<<"final state > From"<<endl;
    int it=0;
    do{
        M1=M2;
        for(Set::const_iterator i=NonObservable.begin();!(i==NonObservable.end());i++)
        {
            
            bdd succ= relation[(*i)](M2);
			/*if(((succ<finalstate)==bddfalse)&&(succ!=finalstate))
				{
				//cout<<"bdd_apply pas inferieur"<<endl;
				//cout<<bddtable<<(succ<finalstate)<<endl;
                    cout<<"oooooPCooo"<<endl;
				pc=0;
				}*/
            M2=succ|M2;
                       // cout<<bddtable<<succ<<endl;

        }
        TabMeta[nbmetastate]=M2;
        int intsize=bdd_anodecount(TabMeta,nbmetastate+1);
        if(MaxIntBdd<intsize)
            MaxIntBdd=intsize;
        it++;
        //	cout << bdd_nodecount(M2) << endl;
    }while(M1!=M2);
  //  cout <<"end"<<endl;
	return M2;
}
bdd RdPBDD::Accessible_epsilon(bdd From, Class_Of_State*c)
{
    bdd M1;
    bdd M2=From;
    
    //cout<<"Im here"<<endl;
    //cout<<"final state"<<endl;
    //cout<<bddtable<<finalstate<<endl;
    
    // cout<<"hhhh Accessible_epsilon"<<endl;
    /* cout<<bddtable<<From<<endl;
     cout<<"finhhhh"<<endl;*/
    
    //if((From-finalstate)!=bddfalse) {if((From-finalstate)!=From) pc=0;}
    
    // if((bdd_apply(From,finalstate,<))!=bddfalse)
 /*   if(((From<finalstate)==bddfalse)&&(From!=finalstate))
    {
		cout<<"bdd_apply pas inferieur"<<endl;
        cout<<bddtable<<(From<finalstate)<<endl;
		pc=0;}
    //else
    /*cout<<"continue pc"<<pc<<endl;
     cout<<bddtable<<(From-finalstate)<<endl;
     cout<<"fin<<<<"<<endl;*/
    //if(From>=finalstate) cout<<"final state < From"<<endl;
    //if(From<finalstate) cout<<"final state > From"<<endl;
    int it=0;
    do{
        M1=M2;
        for(Set::const_iterator i=NonObservable.begin();!(i==NonObservable.end());i++)
        {
            
            bdd succ= relation[(*i)](M2);
            if(succ!=bddfalse)
                c->Enable.insert((*i));
                // cout<<"succ"<<bddtable<<succ;
            		/*	if(((succ<finalstate)==bddfalse)&&(succ!=finalstate))
            {
				//cout<<"bdd_apply pas inferieur"<<endl;
				//cout<<bddtable<<(succ<finalstate)<<endl;
                cout<<"oooooPCooo"<<endl;

				pc=0;
            }*/
            M2=succ|M2;
	    // c->Mf=succ|c->Mf;
            // cout<<bddtable<<succ<<endl;
            
        }
        TabMeta[nbmetastate]=M2;
        int intsize=bdd_anodecount(TabMeta,nbmetastate+1);
        if(MaxIntBdd<intsize)
            MaxIntBdd=intsize;
        it++;
        //	cout << bdd_nodecount(M2) << endl;
    }while(M1!=M2);
    //  cout <<"end"<<endl;
	return M2;
}

/*------------------------  StepForward()  --------------*/
bdd RdPBDD::StepForward2(bdd From)
{
    // cout<<"Debut Step Forward \n";
    bdd Res;
    for(Set::const_iterator i=NonObservable.begin();!(i==NonObservable.end());i++)
    {
        bdd succ = relation[(*i)](From);
        Res=Res|succ;
    }
    //cout<<"Fin Step Forward \n";
    return Res;
}
bdd RdPBDD::StepForward(bdd From)
{
    //cout<<"Debut Step Forward \n";
    bdd Res=From;
    for(Set::const_iterator i=NonObservable.begin();!(i==NonObservable.end());i++)
    {
        bdd succ = relation[(*i)](Res);
        Res=Res|succ;
    }
    //cout<<"Fin Step Forward \n";
    return Res;
}

//the set of states reachable from any state of From by the fiing of t
  bdd RdPBDD::Succ(bdd From, int t)
{
    return relation[t](From);
}
Set RdPBDD::ComputeTf(bdd Mf)
{
 Set res;
 // cout<<"FFFF4"<<endl;

    for(Set::const_iterator i=NonObservable.begin();!(i==NonObservable.end());i++)
    {
      //   cout<<"FFFF334"<<endl;
	  if((Succ(Mf,(*i))&Mf)!=bddfalse) 
               {
		 res.insert(*i);
		 //cout<<"FFFF"<<endl;

		 cout<<"in computetf: transition : "<<transitions[(*i)].name<<endl;

	       }
    }
    return res;

}
/*------------------------  StepBackward()  --------------*/
bdd RdPBDD::StepBackward2(bdd From)
{
    bdd Res;
    // cout<<"Ici Step Back : From.id() = "<<From.id()<<endl;
    for(vector<Trans>::const_iterator t=relation.begin();
        t!=relation.end();t++){
        bdd succ = (*t)[From];
        Res=Res|succ;
    }
    // cout<<"Res.id() = "<<Res.id()<<endl;
    return Res;
}
//StepBackward selon nonObs
bdd RdPBDD::StepBackward3(bdd From)
{
    bdd Res=From;
    int k=0;
    for(Set::const_iterator i=NonObservable.begin();!(i==NonObservable.end());i++)
        
    // for(vector<Trans>::const_iterator t=relation.begin();
      // t!=relation.end();t++,k++)
    {
      cout<<"transition : "<<transitions[(*i)].name<<endl;
        bdd succ = relation[(*i)][Res];
       // bdd succ = (*t)[Res];

        //cout<<"succ: "<<endl;
        //cout<<bddtable<<succ<<endl;

       // bdd succ = (*i)[Res];
        Res=Res|succ;
    }
    
    return Res;
}
//StepBackward selon nonObs avec l'intersection avc un etat donné s
bdd RdPBDD::StepBackward3(bdd From,bdd s)
{
    bdd Res=From;
    for(Set::const_iterator i=NonObservable.begin();!(i==NonObservable.end());i++)
        
    // for(vector<Trans>::const_iterator t=relation.begin();
      // t!=relation.end();t++,k++)
    {
      //cout<<"transition : "<<transitions[(*i)].name<<endl;
        bdd pred = relation[(*i)][Res];
       // bdd succ = (*t)[Res];

        //cout<<"succ: "<<endl;
        //cout<<bddtable<<succ<<endl;

       // bdd succ = (*i)[Res];
        Res=Res|(pred&s);
    }
    
    return Res;
}


//resultat avec intersection avec l'agregat donn\8E
bdd RdPBDD::StepBackward2(bdd From,bdd s)
{
    bdd Res;
    // cout<<"Ici Step Back : From.id() = "<<From.id()<<endl;
    for(vector<Trans>::const_iterator t=relation.begin();
        t!=relation.end();t++){
        bdd succ = (*t)[From];
        Res=Res|(succ & s);
    }
    // cout<<"Res.id() = "<<Res.id()<<endl;
    return Res;
}
//return predecesseur de from par t dans c 
bdd RdPBDD::StepBackward(bdd From,int tt,bdd c)
{
   
    int k=0;
    //  cout<<"From est : "<<endl;
    //cout<<bddtable<<From<<endl;
    // cout<<"Ici Step Back : From.id() = "<<From.id()<<endl;
    //  for(vector<Trans>::const_iterator t=relation.begin();
    //	 t!=relation.end();t++,k++){
       //if(k==tt)
    // bdd succ = (*t)[From];
        	bdd pred = relation[tt][From];
		cout<<"Stepbackward function: transition est "<<transitions[tt].name<<endl;
		/*cout<<"pred est : "<<endl;
		cout<<bddtable<<pred<<endl;
		cout<<"c est : "<<endl;
		cout<<bddtable<<c<<endl;*/
		
	
    // cout<<"Res.id() = "<<Res.id()<<endl;
    return pred&c;
}
bdd RdPBDD::StepBackward(bdd From)
{
    int k=0;
    bdd Res=From;
    for(vector<Trans>::const_iterator t=relation.begin();
        t!=relation.end();t++,k++){
        //cout<<"transitions "<<transitions[k].name<<endl;
        bdd succ = (*t)[Res];
        Res=Res|succ;
    }
    //cout<<endl;
    return Res;
}
//resultat avec intersection avec l'agregat donn\8E

bdd RdPBDD::StepBackward(bdd From,bdd s)
{
    bdd Res=From;
    for(vector<Trans>::const_iterator t=relation.begin();
        t!=relation.end();t++){
        bdd succ = (*t)[Res];
        Res=Res|(succ & s);
    }
    return Res;
}

/*---------------------------GetSuccessor()-----------*/
bdd RdPBDD::get_successor(bdd From,int t)
{
    return relation[t](From);
}
/*-------------------------Firable UnObs()--------------*/
set<string> RdPBDD::firable_Nonobs(bdd State)
{
  set<string> res;
    for(Set::const_iterator i=NonObservable.begin();!(i==NonObservable.end());i++)
    {
        {bdd succ =  relation[(*i)](State);
            if(succ!=bddfalse)
                res.insert(transitions[*i].name);}
    }
    return res;
}
/*-------------------------Firable Obs()--------------*/
Set RdPBDD::firable_obs(bdd State)
{
    Set res;
    for(Set::const_iterator i=Observable.begin();!(i==Observable.end());i++)
    {
        {bdd succ =  relation[(*i)](State);
            if(succ!=bddfalse)
                res.insert(*i);}
    }
    return res;
}
/*-------------------------Firable trans()--------------*/
Set RdPBDD::firable_trans(bdd State)
{
    Set res;
    for(Set::const_iterator i=Observable.begin();!(i==Observable.end());i++)
    {
        {bdd succ =  relation[(*i)](State);
            if(succ!=bddfalse)
                res.insert(*i);}
    }
    for(Set::const_iterator i=NonObservable.begin();!(i==NonObservable.end());i++)
    {
        {bdd succ =  relation[(*i)](State);
            if(succ!=bddfalse)
                res.insert(*i);}
    }
    return res;
}
/*------------------------------------Observation Graph ----------*/
void allsatPrintHandler(char* varset, int size) 
 { 
	 cout<<"size : "<<size<<endl<< "*";
   for (int v=0; v<size; ++v) 
   { 
	   if (varset[v] >= 0)
      cout <<(char)('0' + varset[v]); 
   } 
   cout <<"*"<< endl; 
 }
void  RdPBDD::compute_canonical_deterministic_graph_Opt(MDGraph& g)
{
    cout<<"COMPUTE CANONICAL DETERMINISTIC GRAPH_________________________ \n";
    
    //cout<<"nb bdd var : "<<bdd_varnum()<<endl;
    double d,tps;
    d=clock();
    TabMeta=new bdd[10000];
    nbmetastate=0;
    MaxIntBdd=0;
    typedef pair<Class_Of_State*,bdd> couple;
    typedef pair<couple, Set> Pair;
    typedef stack<Pair> pile;
    pile st;
    NbIt=0;
    itext=itint=0;
    Class_Of_State* reached_class;
    Set fire;
    Set fire2;
    Set unionfire,intermfire;
    // size_t max_meta_state_size;
    bdd globalBDD;

    Class_Of_State *c=new Class_Of_State;
    {
        // cout<<"Marquage initial :\n";
        //cout<<bddtable<<M0<<endl;
        //cout<<"FIN marquage initial :\n";
        //cout<<"Etat Fianux :\n";
        //cout<<bddtable<<finalstate<<endl;
        //cout<<"FIN Etat Finaux :\n";
       // cout<<"finalstate :\n";
         //cout<<bddtable<<finalstate<<endl;
           //                           bdd_allsat(finalstate, allsatPrintHandler);

        // cout<<"avant accessible epsilon \n";*/
        bdd Complete_meta_state=Accessible_epsilon(M0);
        
        fire=firable_obs(Complete_meta_state);
        fire2=firable_trans(Complete_meta_state);
        /* for(Set::const_iterator i=fire.begin();!(i==fire.end());i++)
         cout<<"TEST !!  "<<(*i)<<" et "<<transitions[*i].name<<endl;
         for(Set::const_iterator i=fire2.begin();!(i==fire2.end());i++)
         cout<<"TEST !!  "<<(*i)<<" et "<<transitions[*i].name<<endl;
         cout<<"taille de fire2 est : "<<fire2.size()<<endl;*/
        intermfire=fire2;
        //  c->blocage=Set_Bloc(Complete_meta_state);
        // c->boucle=Set_Div(Complete_meta_state);
        // c->final=((Complete_meta_state & finalstate)!=bdd_false());
        // cout<<"ettttttttt "<<Set_Bloc(Complete_meta_state)<<endl;
        // cout<<"**************Compute LAMDA:************** "<<endl;
        // cout<<"--------------------"<<endl;
        c->lamda=computelamda(Complete_meta_state,1);
		//   cout<<"**************Apres computing LAMDA*******"<<endl;

        //cout<<"--------------------"<<endl<<endl;
        
       /* cout<<"Complete_meta_state :\n";
                     cout<<bddtable<<Complete_meta_state<<endl;
              cout<<"FIN Complete_meta_state :\n";*/
             //                bdd_allsat(Complete_meta_state, allsatPrintHandler);
		//cout<<strictGrand(Complete_meta_state,finalstate)<<endl;
               //      cout<<"nombre de chemins existants "<<bdd_pathcount(Complete_meta_state)<<endl; 
                 //    bdd_printset(Complete_meta_state);
				//	cout<<"End"<<endl;
       // cout<<"hhhh:1111  "<<c->final<<endl;
         /* cout<<"Complete_meta_state :\n";
         cout<<bddtable<<Complete_meta_state<<endl;
         cout<<"FIN Complete_meta_state :\n";*/
      /*   cout<<"finalstate :\n";
        cout<<bddtable<<finalstate<<endl;
         cout<<"FIN finalstate :\n";*/
          //cout<<"nombre de chemins existants"<<bdd_pathcount(finalstate)<<endl; 
          //bdd_printset(finalstate);
		/*bdd a=bdd_true();
		int domaine= 2;
		fdd_extdomain(&domaine, 1);
		fdd_extdomain(&domaine, 1);
		fdd_extdomain(&domaine, 1);
		bdd finalbdd=bdd_true();
		bdd bd=bdd_true();
		bd=(fdd_ithvar(0,1)&fdd_ithvar(2,0)&fdd_ithvar(4,0));

		finalbdd=(fdd_ithvar(0,1)&fdd_ithvar(2,0)&fdd_ithvar(4,0))| (fdd_ithvar(0,1)&fdd_ithvar(2,1)&fdd_ithvar(4,0));

		//finalbdd=(fdd_ithvar(0,0)&fdd_ithvar(2,0)&fdd_ithvar(4,0))| (fdd_ithvar(0,1)&fdd_ithvar(2,0)&fdd_ithvar(4,1));
		//finalbdd=(bdd_ithvar(0,1)&bdd_ithvar(2,0)&bdd_ithvar(4,0))| (bdd_ithvar(0,1)&bdd_ithvar(2,0)&bdd_ithvar(4,1));


		//bdd cbdd=bdd_true();
		//cbdd=fdd_ithvar(0,0)&fdd_ithvar(2,0)&fdd_ithvar(4,1);
		//cout<<"cbb est "<<strictGrand(cbdd,finalbdd)<<" finalbdd"<<endl;

		cout<<"finalbdd :"<<endl;
		cout<<bddtable<<finalbdd<<endl;
        bdd_allsat(finalbdd, allsatPrintHandler);

		//  cout<<"finalbdd_bdd support: "<<bddtable<<bdd_support(finalbdd)<<endl;

		cout<<"nombre de chemins existants finalbdd "<<bdd_pathcount(finalbdd)<<endl;

		a=(fdd_ithvar(0,1)&fdd_ithvar(2,0)&fdd_ithvar(4,0))| (fdd_ithvar(0,1)&fdd_ithvar(2,1)&fdd_ithvar(4,0));

		//a=(fdd_ithvar(0,1)&fdd_ithvar(2,0)&fdd_ithvar(4,1))| (fdd_ithvar(0,0)&fdd_ithvar(2,0)&fdd_ithvar(4,1));
        cout<<"nombre de chemins existants "<<bdd_satcountset(finalbdd,bdd_support(a))<<" et "<<endl;

        cout<<"a :"<<bddtable<<a<<endl;
        //bdd_allsat(a, allsatPrintHandler);

		//   cout<<"a_bdd support : "<<bddtable<<bdd_support(a)<<endl;
		bdd dd,tempaa=finalbdd;
		while(tempaa!=dd){
		
		//dd=bdd_satone(tempaa);
		dd=bdd_satoneset(tempaa,bdd_support(bd),bddtrue);
		if(dd!=bddfalse)
		 {   cout << "dd true assignement "<< bddtable<< dd<<endl;
		    cout<<"taille: "<<bdd_varnum()<<endl;
		    cout<<"nombre de chemins existants "<<bdd_pathcount(dd)<<endl;
		                 bdd_allsat(dd, allsatPrintHandler);

		           cout<<"a est "<<strictGrand(a,dd)<<" finalbdd"<<endl<<endl;
		}
		tempaa=tempaa-dd;

		}
		// cout << "true assignement "<< bddtable<< bdd_satone(a)<<endl;
		cout<<"a est "<<strictGrandBDD(a,finalbdd)<<" finalbdd"<<endl<<endl;

		exit(0);
		//if("sup"=="sup") cout<<"!!!egale"<<endl;
		// cout<<"a est "<<strictGrand(a,finalbdd)<<" finalbdd"<<endl;


		//   bdd c=fdd_ithvar(6,1)
		bdd b=bdd_high(a);
		//b=b&dd_ithvar
		cout<<"a :"<<endl;
		cout<<bddtable<<a<<endl;
		cout<<"variable a: "<<bdd_var(a)<<endl;
		cout<<"sat count a: "<<bdd_satcount(a)<<endl;
		bdd_allsat(a, allsatPrintHandler);
		//  a=a&fdd_ithset(2);
		cout<<"b high:"<<endl;
		cout<<bddtable<<b<<endl;
		//  cout<<"variable b: "<<bdd_var(b)<<endl;
		cout<<"sat count b: "<<bdd_satcount(b)<<endl;
		bdd_allsat(b,allsatPrintHandler);
*/

		globalBDD=Complete_meta_state;
		string resgr=strictGrandBDD(globalBDD,finalstate);
		cout<<"globalBDD nv est "<<resgr<<" finalstate"<<endl;
       
		if (resgr=="sup")
		{pc=0; 				   }
		
		//    cout<<"FIN globalBDD :\n";
		/*  bdd d,tempa=finalstate;
         cout<<"globalBDDe :\n";
         cout<<bddtable<<globalBDD<<endl;
         cout<<"Finalstate :\n";
         cout<<bddtable<<finalstate<<endl;
		while(tempa!=d){
		
		//d=bdd_satone(tempa);
				d=bdd_satoneset(tempa,bdd_support(M0),bddtrue);

		    cout << "true assignementt "<< bddtable<< d<<endl;
		if(d!=bddfalse)
		       {  
				   string stringpc= strictGrand(globalBDD,d);
				   cout<<"globalBDD est "<<stringpc<<" finalbdd"<<endl;
				   if (stringpc=="sup")
					{pc=0;
					tempa=d;}
				   }
		tempa=tempa-d;



		}*/
         //cout<<"globalBDD est "<<strictGrand(globalBDD,finalstate)<<" finalbdd"<<endl;

        c->class_state=CanonizeR(Complete_meta_state,0);
        //  cout<<"Apres CanonizeR nb representant : "<<bdd_pathcount(c->class_state)<<endl;
        //c->class_state=Complete_meta_state;
        TabMeta[nbmetastate]=c->class_state;
        nbmetastate++;
        old_size=bdd_nodecount(c->class_state);
        //max_meta_state_size=bdd_pathcount(Complete_meta_state);
        st.push(Pair(couple(c,Complete_meta_state),fire));
    }
    g.setInitialState(c);
    g.insert(c);
    g.nbMarking+=bdd_pathcount(c->class_state);
    do
    {
        Pair  e=st.top();
        st.pop();
        nbmetastate--;
        while(!e.second.empty())
        {
            int t = *e.second.begin();
            cout<<"t:"<<t<<endl;
            e.second.erase(t);
            double nbnode;
            reached_class=new Class_Of_State;
            {
                //cout<<"Avant Accessible epsilon \n";
                bdd Complete_meta_state=Accessible_epsilon(get_successor(e.first.second,t));
                //cout<<"Apres accessible epsilon \n";
                //cout<<"Avant CanonizeR \n";
                // cout<<"globalBDD nv est "<<strictGrand((Complete_meta_state - (globalBDD&Complete_meta_state)),finalstate)<<" finalbdd"<<endl;
                 // cout<<"globalBDD nv est "<<strictGrandBDD((Complete_meta_state - (globalBDD&Complete_meta_state)),finalstate)<<" finalstate"<<endl;
				string resgr=strictGrandBDD((Complete_meta_state - (globalBDD&Complete_meta_state)),finalstate);
				cout<<"globalBDD nv est "<<resgr<<" finalstate"<<endl;
                
				if (resgr=="sup")
				{pc=0; 				   }
				/* bdd d,tempa=finalstate;
				while(tempa!=d){
		
					//d=bdd_satone(tempa);
					//extraire le bdd correspondant \E0 un seul chemin vers true
					d=bdd_satoneset(tempa,bdd_support(M0),bddtrue);

					cout << "true assignement "<< bddtable<< d<<endl;
					tempa=tempa-d;
					if(d!=bddfalse)
					{  
						string stringpc= strictGrand((Complete_meta_state - (globalBDD&Complete_meta_state)),d);
						cout<<"globalBDD est "<<stringpc<<" finalbdd"<<endl;
						if (stringpc=="sup")
						{	pc=0;
							tempa=d;
						}
					}
		         


				}*/
				globalBDD=Complete_meta_state|globalBDD ;
                reached_class->class_state=CanonizeR(Complete_meta_state,0);
                //cout<<"Apres CanonizeR \n";
                // cout<<"Apres CanonizeR nb representant : "<<bdd_pathcount(reached_class->class_state)<<endl;
                //reached_class->class_state=Complete_meta_state;
                Class_Of_State* pos=g.find(reached_class);
                nbnode=bdd_pathcount(reached_class->class_state);
                if(!pos)
                {
                    // reached_class->blocage=Set_Bloc(Complete_meta_state);
                    // reached_class->boucle=Set_Div(Complete_meta_state);
                    //reached_class->final=((Complete_meta_state & finalstate)!=bdd_false());
                    // cout<<"**************Compute LAMDA:************** "<<endl;
                    //cout<<"--------------------"<<endl;
                  //  cout<<"ettttttttt "<<Set_Bloc(Complete_meta_state)<<endl;
                    reached_class->lamda=computelamda(Complete_meta_state,1);
                    // cout<<"**************Apres computing LAMDA*******"<<endl;
                    //cout<<"--------------------"<<endl<<endl;
                    
                  //  cout<<"hhhh: "<<reached_class->final<<endl;
                     /*cout<<"Complete_meta_state :\n";
                     cout<<bddtable<<Complete_meta_state<<endl;
                     cout<<"FIN Complete_meta_state :\n";*/
                    // cout<<"finalstate :\n";
                     //cout<<bddtable<<finalstate<<endl;
                     //cout<<"FIN finalstate :\n"
                    fire=firable_obs(Complete_meta_state);
                    fire2=firable_trans(Complete_meta_state);
                    /* for(Set::const_iterator i=fire.begin();!(i==fire.end());i++)
                     cout<<"TEST !!  "<<(*i)<<" et "<<transitions[*i].name<<endl;
                     for(Set::const_iterator i=fire2.begin();!(i==fire2.end());i++)
                     cout<<"TEST !!  "<<(*i)<<" et "<<transitions[*i].name<<endl;
                     cout<<"taille de fire2 est : "<<fire2.size()<<endl;*/
                   // cout<<"Complete_meta_state :\n";
                     //                            bdd_allsat(Complete_meta_state, allsatPrintHandler);

              //       cout<<bddtable<<Complete_meta_state<<endl;
                //     cout<<"FIN Complete_meta_state :\n";
                  //                        cout<<"nombre de chemins existants"<<bdd_pathcount(Complete_meta_state)<<endl; 
					
                    //set_union(fire2.begin(),fire2.end(),intermfire.begin(),intermfire.end(),inserter(unionfire,unionfire.begin()));
                    intermfire=unionfire;
                    /*cout<<"taille de intermfire est : "<<intermfire.size()<<endl;
                     cout<<"taille de unionfire est : "<<unionfire.size()<<endl;
                     cout<<"taille de transitions est : "<<transitions.size()<<endl;*/
                    st.push(Pair(couple(reached_class,Complete_meta_state),fire));
                    TabMeta[nbmetastate]=reached_class->class_state;
                    nbmetastate++;
                    old_size=bdd_anodecount(TabMeta,nbmetastate);
                    e.first.first->Successors.insert(e.first.first->Successors.begin(),Edge(reached_class,t));
                    reached_class->Predecessors.insert(reached_class->Predecessors.begin(),Edge(e.first.first,t));
                    g.addArc();
                    g.insert(reached_class);
                    
                }
                else
                {
                    delete reached_class;
                    e.first.first->Successors.insert(e.first.first->Successors.begin(),Edge(pos,t));
                    pos->Predecessors.insert(pos->Predecessors.begin(),Edge(e.first.first,t));
                    g.addArc();
                }
            }
                    bool w=reached_class->blocage();
                    if(w)
                    {
                        cout<<"\n\tDEADLOCK !!!"<<endl;
                        cout<<" \n\nCOMPLETE GRAPH'S BUILDING ?(y/n)"<<endl;
                        char c;
                        cin>>c;
                        if(c=='n'||c=='N')
                        {
                            cout<<"No"<<endl;
                            size_t n=1;
                            g. printCompleteInformation();
                            
                            //g.printGraph(g.initialstate,n);
                            exit(0);
                        }
                        
                    }
        }//cout<<"test"<<endl;
    } while(!st.empty());
    if(unionfire.size()==transitions.size()) g.nodeadTrans=true;
    else g.nodeadTrans=false;

    if (DeadRdp==true) {g.DeadMG=true;}
    g.pc=pc;
    //cout<<" pc "<<pc<<endl;
    tps=(double)(clock()-d) / (double)CLOCKS_PER_SEC;
    // cout<<endl<<"DeadRdp "<<DeadRdp<<"  g.DeadMG"<<g.DeadMG<<endl;
    
    /* cout<<endl<<"-------------------------------------------"<<endl;
     
     if (g.DeadMG)
     cout<<"CONC: THE GRAPH CONTAINS A DEADLOCK STATE "<<endl;
     else    cout<<"CONC: THE GRAPH IS DEADLOCKFREE "<<endl;
     cout<<"-------------------------------------------"<<endl;*/
    cout<<"#TIME OF CONSTRUCTION : "<<tps<<endl;
    cout<<" MAXIMAL INTERMEDIARY BDD SIZE \n"<<MaxIntBdd<<endl;
    cout<<"OLD SIZE : "<<old_size<<endl;
    //cout<<"NB SHARED NODES : "<<bdd_anodecount(TabMeta,nbmetastate)<<endl;
    cout<<"NB META STATE DANS CONSTRUCTION : "<<nbmetastate<<endl;
    cout<<"NB ITERATIONS CONSTRUCTION : "<<NbIt<<endl;
    cout<<"Nb Iteration externes : "<<itext<<endl;
    cout<<"Nb Iteration internes : "<<itint<<endl;
    
    
}

/*void  RdPBDD::ParcoursProf(MDGraph& g , pile& st)
{ 
}*/
void  RdPBDD::compute_canonical_deterministic_graph_OptProf(MDGraph& g)
{
    cout<<"COMPUTE CANONICAL DETERMINISTIC GRAPH - Depth-traversal_________________________ \n";
    double d,tps;
    d=clock();
    TabMeta=new bdd[10000];
    nbmetastate=0;
    MaxIntBdd=0;
    typedef pair<Class_Of_State*,bdd> couple;
    typedef pair<couple, Set> Pair;
    typedef pair<couple, int> Pairint;

    typedef stack<Pair> pile;
    typedef stack<Pairint> pileint;
    //GlobalMf contient lunion des Mf calcul\8E en partant de letat final

    pile st;
    pileint stmf;
    NbIt=0;
    itext=itint=0;
    Class_Of_State* reached_class;
    //les transitions observes franchissables
    Set fire;
    //les transitions franchissables
    Set fire2;
    Set unionfire,intermfire;
    Set uniontf,intermtf;
    
    // size_t max_meta_state_size;
    //unionpred contient lunion des Mf calculee en partant de letat final
    //globalBDD contient lunion de tout les marquages calclees
    bdd globalBDD,unionpred_Mf,globalMf=bddfalse;
    bdd Mf;
    bdd RN;
    Class_Of_State *c=new Class_Of_State;
    {
        bdd Complete_meta_state=Accessible_epsilon(M0,c);
        cout<<"fff"<<endl;
        fire=firable_obs(Complete_meta_state);
        c->Enable.insert(fire.begin(),fire.end());
        cout<<"fff2"<<endl;

        fire2=firable_trans(Complete_meta_state);
        set_union(fire2.begin(),fire2.end(),intermfire.begin(),intermfire.end(),inserter(unionfire,unionfire.begin()));
        intermfire=unionfire; 
        cout<<"fff3"<<endl;

      
        c->lamda=computelamda(Complete_meta_state,1);
	        cout<<"fff4"<<endl;

      	globalBDD=Complete_meta_state;
        cout << "Global"<< bddtable<< globalBDD<<endl;

	string resgr=strictGrandBDD(globalBDD,finalstate);
       if (resgr=="sup")
		pc=0; 	
			  
        c->class_state=CanonizeR(Complete_meta_state,0);
        TabMeta[nbmetastate]=c->class_state;
        nbmetastate++;
        old_size=bdd_nodecount(c->class_state);
        //max_meta_state_size=bdd_pathcount(Complete_meta_state);
        st.push(Pair(couple(c,Complete_meta_state),fire));
	// bdd fin=finalstate & Complete_meta_state;$
	    cout<<"How many DEL (combien delmnt? "<<st.size()<<endl;

        
    }
    g.setInitialState(c);
    g.insert(c);
    g.nbMarking+=bdd_pathcount(c->class_state);
    do
    { 
        Pair  e=st.top();
       	if(e.second.size()!=0)
        {
            int t = *e.second.begin();
            cout<<"t : "<<transitions[t].name<<endl;
            st.top().second.erase(t);
            double nbnode;
            reached_class=new Class_Of_State;
	    //cout<<"1 son M_f est : "<<endl;
	    //cout<<bddtable<<reached_class->Mf<<endl;
            bdd Complete_meta_state=Accessible_epsilon(get_successor(e.first.second,t),reached_class);
	  
            string resgr=strictGrandBDD((Complete_meta_state - (globalBDD&Complete_meta_state)),finalstate);
	    if (resgr=="sup")
		pc=0; 				   
            globalBDD=globalBDD | Complete_meta_state ;
            reached_class->class_state=CanonizeR(Complete_meta_state,0);
  
            Class_Of_State* pos=g.find(reached_class);
            nbnode=bdd_pathcount(reached_class->class_state);
	    //reached_class is encountred for the first time
	    //cout<<"Finalstate table"<<endl;
	    // cout<<bddtable<<finalstate<<endl;
	      if(!pos)
            { 
	      cout<<endl<<"Nouvel aggregat !!!!!"<<endl;
	      reached_class->lamda=computelamda(Complete_meta_state,1);
              fire=firable_obs(Complete_meta_state);
              reached_class->Enable.insert(fire.begin(),fire.end());
	      fire2=firable_trans(Complete_meta_state);		
	      set_union(fire2.begin(),fire2.end(),intermfire.begin(),intermfire.end(),inserter(unionfire,unionfire.begin()));
	      intermfire=unionfire;
	      st.push(Pair(couple(reached_class,Complete_meta_state),fire));


              //stmf.push(Pairint(couple(reached_class,Complete_meta_state),t));
	      cout<<"Aggregat Empilee"<<endl;
	      //  cout<<"son M_f est : "<<endl;
	      //cout<<bddtable<<reached_class->Mf<<endl;
	      //cout<<"son bdd est : "<<endl;
	      //cout<<bddtable<<reached_class->class_state<<endl;
              bdd fin=finalstate & Complete_meta_state;
	      //cout<<"Complete_meta_state table"<<endl;
	      //cout<<bddtable<<Complete_meta_state<<endl;
	      //cout<<"Fin table"<<endl;
	      //cout<<bddtable<<fin<<endl;

                    //Calcul Mf
	      if (fin !=bdd_false())
	      { 

		unionpred_Mf=fin;
		bdd tempo;
            
		do {
		  tempo=unionpred_Mf ;
		  unionpred_Mf=(StepBackward3(unionpred_Mf,reached_class->class_state));
                
                
		}while(!(unionpred_Mf==tempo));
            
	    /*  do {
                Pairint pr = stmf.top();
                int tt=pr.second;
                cout<<transitions[tt].name<<" - "<<endl;

                //cout<<transitions[tt].name<<" - "<<endl;
                if(tt!=-1){
                    unionpred_Mf = unionpred_Mf | relation[(tt)](unionpred_Mf);
                    do {
                        tempo=unionpred_Mf ;
                        unionpred_Mf=(StepBackward3(unionpred_Mf));
                        
                        
                    }while(!(unionpred_Mf==tempo));
                }
                stmf.pop();
                
		} while(!stmf.empty());*/
		reached_class->Mf=unionpred_Mf;
		//	cout<<"Mf calculeeF table"<<endl;
		//cout<<bddtable<<reached_class->Mf<<endl;
	      }
	   
	      TabMeta[nbmetastate]=reached_class->class_state;
	      nbmetastate++;
	      old_size=bdd_anodecount(TabMeta,nbmetastate);
	      e.first.first->Successors.insert(e.first.first->Successors.begin(),Edge(reached_class,t));
	      reached_class->Predecessors.insert(reached_class->Predecessors.begin(),Edge(e.first.first,t));
	      g.addArc();
	      g.insert(reached_class);
                    
            }
            else
            {
	      fire=firable_obs(Complete_meta_state);
              stmf.push(Pairint(couple(reached_class,Complete_meta_state),t));
	      delete reached_class;
	      e.first.first->Successors.insert(e.first.first->Successors.begin(),Edge(pos,t));
	      pos->Predecessors.insert(pos->Predecessors.begin(),Edge(e.first.first,t));
	      g.addArc();
            }
                
	    /*    
	    // cout << "RN "<< bddtable<< globalBDD<<endl;
	    if(globalBDD==unionpred)
	    cout<<"YESssss"<<endl;
	    else
            
                     cout<<"NOooooo"<<endl;*/
                    
	    /* }*/
                
                /////////////////////////**********************///////////////////////
                /////////////////////////**********************///////////////////////
	    bool w=reached_class->blocage();
            if(w)
            {
                cout<<"\n\tDEADLOCK !!!"<<endl;
                cout<<" \n\nCOMPLETE GRAPH'S BUILDING ?(y/n)"<<endl;
                char c;
                cin>>c;
                if(c=='n'||c=='N')
                {
                    cout<<"No"<<endl;
                    size_t n=1;
                    g. printCompleteInformation();
                    
                    //g.printGraph(g.initialstate,n);
                    exit(0);
                }
                
            } 
	}	      
	else //if(!st.empty())
	  {
	    cout<<endl<<"Je depile!!!"<<endl;
	    st.pop();
	    cout<<"il reste combien delmnt?? "<<st.size()<<endl;
	    //if(st.size()!=0)
	     globalMf=globalMf|e.first.first->Mf;
	     //cout<<"FFFFFFF3"<<endl;
	     Set tfUnobs=ComputeTf(e.first.first->Mf);
	     set_union(e.first.first->T_f.begin(), e.first.first->T_f.end(),tfUnobs.begin(),tfUnobs.end(),inserter(e.first.first->T_f,e.first.first->T_f.begin()));
	      set_union(e.first.first->T_f.begin(),e.first.first->T_f.end(),uniontf.begin(),uniontf.end(),inserter(uniontf,uniontf.begin()));

	      if((st.size()!=0)&&(e.first.first->Mf!=bddfalse))
			  { 
			    int  t=e.first.first->Predecessors[e.first.first->Predecessors.size()-1].second;
			  //   }
			  //}
			    Pair Sommet=st.top();
			    Sommet.first.first->T_f.insert(t);
		       cout<<"tf: transition reliant l'elmt dpile au sommet de la pile : "<<transitions[(t)].name<<endl;

			    }
				
			//  //set_union( e.first.first->T_f.begin(), e.first.first->T_f.end(),intermtf.begin(),intermtf.end(),inserter(uniontf,uniontf.begin()));
			//  intermtf=uniontf; 
 //set_union( e.first.first->T_f.begin(), e.first.first->T_f.end(),uniontf.begin(),uniontf.end(),inserter(uniontf,uniontf.begin()));
	      if(e.first.first->Mf==bddfalse)
	      {
		cout<<"Option to Complete violee"<<endl;
		opToC=0;
	      }
	    else 
	      { //vector<pair<Class_Of_State*,int> >::iterator it ;
		int t;
		//	for (it=e.first.first->Predecessors.begin(); it<e.first.first->Predecessors.end(); it++)
		//{
		    //   if((*it).first==(st.top().first.first))
		    //      {
			//	t=(*it).second;
		if(st.size()!=0)
		  {
		    t=e.first.first->Predecessors[e.first.first->Predecessors.size()-1].second;
			  //   }
			  //}
		    Pair Sommet=st.top();
		    Sommet.first.first->Mf=Sommet.first.first->Mf|SatBack(Sommet.first.second,StepBackward(e.first.first->Mf,t,Sommet.first.second));
		    // UpdateMf(st.top().first,e.first,t);
		    cout<<"st.top().first.first apres modif"<<endl;
		    //cout<<bddtable<<st.top().first.first->Mf<<endl;
for(set<int>::const_iterator n=((st.top().first.first->T_f)).begin();n!=((st.top().first.first->T_f)).end();n++)
				  cout<<"ggggg ("<<transitions[(*n)].name<<")"<<endl;

		  }
		
	      }

	  }



    }while(!st.empty());
   
    if(globalMf!=globalBDD)
      opToC=0;
 if(globalMf!=globalBDD)
      g.weakSound=false;
    g.OptionToComp=opToC;
   if(globalMf==bddfalse)
      g.easySound=false;

   cout<<"RRRRRR transitions.size() "<<transitions.size()<<" uniontf.size()="<<unionfire.size()<<endl;
    if(unionfire.size()==transitions.size()) g.nodeadTrans=true;
    else g.nodeadTrans=false;
 if(uniontf.size()==transitions.size()) g.relaxedSound=true;
    else g.relaxedSound=false;

if(unionfire.size()==transitions.size()) g.nodeadTrans=true;
    else g.nodeadTrans=false;


 if(uniontf.size()==transitions.size()) g.relaxedSound=true;
    else g.relaxedSound=false;
    if (DeadRdp==true) {g.DeadMG=true;}
    g.pc=pc;
   
    //cout<<" pc "<<pc<<endl;
    tps=(double)(clock()-d) / (double)CLOCKS_PER_SEC;
    // cout<<endl<<"DeadRdp "<<DeadRdp<<"  g.DeadMG"<<g.DeadMG<<endl;
    
    /* cout<<endl<<"------<sv-------------------------------------"<<endl;
     
     if (g.DeadMG)
     cout<<"CONC: THE GRAPH CONTAINS A DEADLOCK STATE "<<endl;
     else    cout<<"CONC: THE GRAPH IS DEADLOCKFREE "<<endl;
     cout<<"-------------------------------------------"<<endl;*/
    cout<<"#TIME OF CONSTRUCTION : "<<tps<<endl;
    cout<<" MAXIMAL INTERMEDIARY BDD SIZE \n"<<MaxIntBdd<<endl;
    cout<<"OLD SIZE : "<<old_size<<endl;
    //cout<<"NB SHARED NODES : "<<bdd_anodecount(TabMeta,nbmetastate)<<endl;
    cout<<"NB META STATE DANS CONSTRUCTION : "<<nbmetastate<<endl;
    cout<<"NB ITERATIONS CONSTRUCTION : "<<NbIt<<endl;
    cout<<"Nb Iteration externes : "<<itext<<endl;
    cout<<"Nb Iteration internes : "<<itint<<endl;
}
    
/********************* Satback fuction******************/
//Sature from s dans S sur UnObs
bdd RdPBDD::SatBack( bdd S, bdd s )
{

  cout<<"Ici Stabck"<<endl;
  //cout<<"hhhh transition is "<<transitions[t].name<<endl;

  // bdd mf,unionpred_Mf;
  // cout<<"test lambda .."<< c->lambda<<endl
  //cout<<"Mf avant modif"<<endl;
  //cout<<bddtable<<c->Mf<<endl;
  //mf=StepBackward(a->Mf,t,c->class_state);
 bdd Res=s;
 // cout<<"mf interm est "<<endl;
 //	    cout<<bddtable<<mf<<endl;
   bdd tempo;            
            do {
                tempo=Res ;
		Res=StepBackward3(Res,S);
                
                
            }while(!(Res==tempo));

	    return Res;

}
/******************PRopercompletion functions****************/
string RdPBDD::strictGrand(bdd A,bdd mf)
{
	if(mf!=bddfalse && mf!=bddtrue) cout<<"variable actuel de mf"<<bdd_var(mf)<<endl;
if(A!=bddfalse && A!=bddtrue) cout<<"variable actuel de A "<<bdd_var(A)<<endl;
	/*cout<<"A :\n";
    //bdd_allsat(A, allsatPrintHandler);

    cout<<bddtable<<A<<endl;
    cout<<"mf :\n";
    //bdd_allsat(mf, allsatPrintHandler);

    cout<<bddtable<<mf<<endl;*/
	/*cout<<"A :\n";
    bdd_allsat(A, allsatPrintHandler);

    cout<<bddtable<<A<<endl;
    cout<<"mf :\n";
    bdd_allsat(mf, allsatPrintHandler);

    cout<<bddtable<<mf<<endl;*/
	bool strict= false;
	if( A==bddtrue)
	{ cout<<"here1"<<endl;
		while(mf != bddtrue)
		{ cout<<"here2"<<endl;
			if ( bdd_high(mf) !=bddfalse)
			{ //cout<<"sup"<<endl;
			cout<<"here3"<<endl;
			mf=bdd_high(mf);

			}
			else return "sup";

		}
				//cout<<"egal"<<endl;

		return "egal"; 
	}
	
	if (A==bddfalse)
	{		//cout<<"false"<<endl;

		return "false";
	}
	cout<<"here5"<<endl;
	//if(mf!=bddfalse && mf!=bddtrue)
	while (bdd_var(mf)!=bdd_var(A))//|| (mf!=bddfalse && mf!=bddtrue)|| (A!=bddfalse && A!=bddtrue))
	{cout<<"here6"<<endl;
		if (bdd_low(mf)!=bddfalse)
		{cout<<"here7"<<endl;
			strict = true;
			mf= bdd_low(mf);
		}
		else 
		{cout<<"here7"<<endl;
			mf= bdd_high(mf);
		}
	}
	bdd Arec,mfrec;
	cout<<"here8"<<endl;
	if (bdd_low(mf)!=bddfalse)
	{cout<<"here9"<<endl;
		string s=strictGrand(bdd_high(A),bdd_low(mf));
		if (s=="sup" || s=="egal")
		{ 	//	cout<<"sup"<<endl;

			return "sup";
		}
		else 
		{cout<<"here10"<<endl;
			Arec=bdd_low(A);
			mfrec=bdd_low(mf);
		}
	}
	else
	{
		Arec=bdd_high(A);
		mfrec=bdd_high(mf);
	}
	
	string resrec=strictGrand(Arec,mfrec);
	if ((resrec == "sup")||(resrec=="egal" && strict))
	{//cout<<"sup"<<endl;
		return "sup";
	}
	else 
	{
		return resrec;
	}
	
	 
				
   
	//return "";
}
string RdPBDD::strictGrandBDD(bdd A,bdd mf)
{
	//if(mf!=bddfalse && mf!=bddtrue) cout<<"variable actuel de mf"<<bdd_var(mf)<<endl;
	//if(A!=bddfalse && A!=bddtrue) cout<<"variable actuel de A "<<bdd_var(A)<<endl;
	/*cout<<"A :\n";
    bdd_allsat(A, allsatPrintHandler);

    cout<<bddtable<<A<<endl;
    cout<<"mf :\n";
    bdd_allsat(mf, allsatPrintHandler);

    cout<<bddtable<<mf<<endl;*/
	bool strict= false;
	if( A==bddtrue )
	{//cout<<"here1"<<endl;
		while(mf != bddtrue)
		{//cout<<"here2"<<endl;
		if(mf==bddfalse) return "false";
			if ( bdd_high(mf) !=bddfalse)
			{ //cout<<"sup"<<endl;
			//cout<<"here3"<<endl;
			mf=bdd_high(mf);
				
			}
			else {cout<<"sup1"<<endl;
			return "sup";
			}
		}
				//cout<<"egal1"<<endl;

		return "egal"; 
	}
	if (A==bddfalse)
	{		//cout<<"false"<<endl;

		return "false";
	}
	if(mf==bddtrue)
	{ //cout<<"true mf"<<endl;
		while(A!=bddtrue)
		{ //cout<<"while"<<endl;
				if(A==bddfalse) return "false";

			if(bdd_high(A)!=bddtrue)
			A=bdd_high(A);
			else 
			{//cout<<"false"<<endl;
			return "false";
			}
		}
			{//cout<<"egalemf"<<endl;
			 return "egal";
			}}
	if(mf==bddfalse)
	{//cout<<"falsemf"<<endl;
	return "false";
}
	if(mf!=bddfalse && mf!=bddtrue)
	{	//		cout<<"here4"<<endl;

		if (bdd_var(mf)<bdd_var(A))
		{ //		cout<<"here5"<<endl;
				string s=strictGrandBDD(A,bdd_low(mf));
				if(s=="egal"|| s=="sup")
				{
			//		cout<<"here6"<<endl;
					strict=true;
					mf=bdd_low(mf);
				}
				else
				{
				//	cout<<"here7"<<endl;
					//return(strictGrandBDD(A,bdd_high(mf)));
					mf=bdd_high(mf);
				}
			
	}
	
	
	
		if (bdd_var(mf)>bdd_var(A))
		{ 	
			//cout<<"here7"<<endl;
			while (bdd_var(mf)>bdd_var(A))
			{ if (bdd_high(A)!=bddfalse)
			{ //cout<<"here8"<<endl;
				A= bdd_high(A);
				}
				else {//cout<<"false8"<<endl;
				return "false";
				}
			}
		}
			
		}
		//cout<<"here"<<endl;
	
	bdd Arec,mfrec;
	
	
	if(bdd_low(mf)!=bddfalse)
	{	//if(bdd_low(mf)!=bddtrue)

{//cout<<"heheee strict "<<bddtable<<bdd_low(mf)<<endl;
		strict=true;

	string f=strictGrandBDD(bdd_high(A),bdd_low(mf));
		if(f=="sup"||f=="egal")
		{		cout<<"sup3"<<endl;

			return "sup";
		}
		else
		{ 	//cout<<"here11"<<endl;
//Arec=bdd_low(A);
//mfrec=bdd_low(mf);
strict=false;
			string s= strictGrandBDD(bdd_low(A),bdd_low(mf));
			if(s=="sup" || (s=="egal" && strict))
			{
							cout<<"sup4"<<endl;

				return "sup";
			}
			else return s;
		}
	} 

}
			else {			//cout<<"here12"<<endl;
					Arec=bdd_high(A);
					mfrec=bdd_high(mf);
					
				
				}
		
	
		string resrec=strictGrandBDD(Arec,mfrec);
	if ((resrec == "sup")||(resrec=="egal" && strict))
	{cout<<"sup"<<endl;
		return "sup";
	}
	else 
	{
		return resrec;
	}
	
				
   
	//return "";
}

/******************computelamda()****************/
/*set<string> RdPBDD::computelamda(bdd s,bool bloc,int aff)
{ //cout<<"aggregat: "<<endl;
    //cout<<bddtable<<s;
    // cout<<"je suis dans la fonction bloc= "<<bloc<<endl;
    map<string,bdd> Res;
    int nb=0;
    //checking whether a final state belongs to  states inside s
    if((s & finalstate)!=bdd_false())
    {
        
        bdd from=StepBackward(s,s);
        //  cout<<"StepBackward2: "<<endl;
        //cout<<bddtable<<(from);
        bdd tmp;
        do
        {
            tmp=from;
            from=(StepBackward(s & from,s));
        }while(!(from==tmp));
        //cout<<"term: "<<endl;
        //cout<<bddtable<<from;
        Res.insert(make_pair("term",from));
        nb++;
    }
    //filling the map with couples {{o},S}
    Set Obs=firable_obs(s);
    for(Set::const_iterator f= Obs.begin();!(f==Obs.end());f++)
    { //int a=(*f);
        //std::ostringstream os;
        //os<<a;
        //  cout<<"Obs="<<(*f)<<" qui est "<<transitions[(*f)].name<<endl;
        bdd temp=FrontiereNodes2(s,(*f));
        // cout<<"FrontiereNodes2 :"<<endl;
        //cout<<bddtable<<temp;
        //
        
        
        //  string s = os.str();
        
        Res.insert(make_pair(transitions[(*f)].name,temp));
        nb++;
    }
    //for (map<string,bdd>::const_iterator il= Res.begin();(il!=Res.end());il++)
    //cout<<"RES= "<<(*il).first<<endl;
    
    //reaffining of the map by associating elements which are enabled by the same set of states
    for (map<string,bdd>::const_iterator i= Res.begin();(i!=Res.end());i++)
    {
        for (map<string,bdd>::const_iterator j=i;(j!=Res.end());j++)
            //  for (map<string,bdd>::const_iterator j= Res.begin();(!((*i).first.compare((*j).first)));j++)
        {if (((*j).first.compare((*i).first)))
	    {
            if ((*i).second==(*j).second)
            {
                
                
                Res.insert(make_pair(((*i).first+","+(*j).first),(*i).second));
                // cout<<"---------"<<endl;
                //for (map<string,bdd>::const_iterator il= Res.begin();(il!=Res.end());il++)
                //  cout<<"RES= "<<(*il).first<<endl;
                //  cout<<"-----"<<(*j).first<<"----"<<endl;
                Res.erase((*j).first);
                //  cout<<"---Erase j-----"<<(*j).first<<"-"<<endl;
                // for (map<string,bdd>::const_iterator il= Res.begin();(il!=Res.end());il++)
                //   cout<<"RES= "<<(*il).first<<endl;
                //cout<<"---------"<<endl;
                
                
                j--;
                Res.erase((*i).first);
                //cout<<"----Erase i--"<<(*i).first<<"---"<<endl;
                //for (map<string,bdd>::const_iterator il= Res.begin();(il!=Res.end());il++)
                //  cout<<"RES= "<<(*il).first<<endl;
                //cout<<"---------"<<endl;
                
                i++;
            }
            
            
            
	    }
        }
    }
    
    //finding a deadlock state
    if(bloc)
    {
        Res.insert(make_pair("EV",bddfalse));
          cout<<"Im on the bloc bloc"<<endl;
        DeadRdp=true;
        //cout<<"DeadRdp= "<<DeadRdp<<endl;
    }
    else
        //finding a terminal state (livelock)
        if(Set_bouclebloc(s))
        {
            Res.insert(make_pair("EV",bddfalse));
            cout<<"Im on the bouc bloc"<<endl;
            
            DeadRdp=true;
            //cout<<"DeadRdp= "<<DeadRdp<<endl;
            
        }
    
    
    
    
    set<string> lamda;
    for (map<string,bdd>::const_iterator i= Res.begin();(i!=Res.end());i++)
    {
        lamda.insert((*i).first);
    }
    
    //affichage de lamda
    if(aff==1)
    {
        cout<<"    LAMDA : ";
        cout<<"{";
        
        for(set<string>::const_iterator i=lamda.begin();i!=lamda.end();i++)
        {
            string cr=(*i);
            
            
            cout<<"{"<<cr<<"}";
            
            // }
            
        }
        cout<<"}"<<endl;
    }
    return lamda;
}
*/

set<Sets> RdPBDD:: computelamda(bdd s,int aff)
{

    map<set<string>,bdd> Res;
    //a map contents a set of strings, and a bdd
    int nb=0;
    
    //checking whether a final state belongs to  states inside s
    bdd fin=s & finalstate;
    if(fin!=bdd_false())
    //a finalstate bdd called fin is the and of bdd s and the final state, and there exists such one
    {
        //bdd from=StepBackward(s,s);
        bdd from=fin;
        // cout<<"StepBackward2: "<<endl;
        //cout<<bddtable<<(from);
        bdd tmp;
        do
        {
            tmp=from;
            //  from=(StepBackward(s&from,s));
            from=(StepBackward(from,s));
        }while(!(from==tmp));
        //cout<<bddtable<<from;
        set<string> tint;
        tint.insert("term");
        //set<Set> G;
        //G.insert(tint);
        
        Res.insert(make_pair(tint,from));
        nb++;
    }

    //filling the map with couples {{o},S}
    set<int> Obs=firable_obs(s);
    for(set<int>::const_iterator f= Obs.begin();!(f==Obs.end());f++)
    {
        bdd temp=FrontiereNodes2(s,(*f));
        if(temp!=bddfalse)
	  { set<string> tint;
        
	    tint.insert(transitions[*f].name);
	    Res.insert(make_pair(tint,temp));
	    nb++;
	  }
        
    }
     for (map<set<string>,bdd>::const_iterator il= Res.begin();(il!=Res.end());il++)
     
     {
     set<string>::const_iterator jl=(*il).first.begin();
     do{
     jl++;
     }while (jl!=(*il).first.end());
     }

    // cout<<"here"<<endl;
    //reaffining of the map by associating elements which are enabled by the same set of states
    //  map<set<int>,bdd>::const_iterator i= Res.begin();

    for (map<set<string>,bdd>::iterator i= Res.begin();(i!=Res.end());i++)
    {
        
        Sets inter;
        inter.insert(i->first.begin(),i->first.end());
        bool un_ion=false;
        for (map<set<string>,bdd>::iterator j=i;(j!=Res.end());j++)
            //  for (map<string,bdd>::const_iterator j= Res.begin();(!((*i).first.compare((*j).first)));j++)
        {
            if (j!=i)
            {
         for(set<string>::iterator jl=(*j).first.begin();jl!=(*j).first.end();jl++)


                if ((*i).second==(*j).second)
                {             
		  un_ion=true;
     	  set_union(inter.begin(),inter.end(),(*j).first.begin(),(*j).first.end(),inserter(inter,inter.begin()));


                  map<set<string>,bdd>::iterator jj=--j;
                    Res.erase(++j);

	               j=jj;

                }
      
            }
            
          }//while((j!=Res.end()));
		 bdd interbdd = i->second;
        if(un_ion)
        {
	                 for(set<string>::const_iterator jl=(i->first).begin();jl!=(i->first).end();jl++)

		     Res.insert(i,make_pair(inter,i->second));
            Res.erase(--i);
	  map<set<string>,bdd>::iterator ii=i++;




  i=(ii!=Res.end())? ii : --ii;
        }

    }//while((i!=Res.end()));
    

    
    //finding a deadlock state
    /* if(bloc)
     {
     set<int> tint;
     tint.insert(-1);
     //set<Set> G;
     //G.insert(tint);
     
     Res.insert(make_pair(tint,bddfalse));
     cout<<"Im on the bloc bloc"<<endl;
     //DeadRdp=true;
     //cout<<"DeadRdp= "<<DeadRdp<<endl;
     }
     else*/

    //finding a terminal state (livelock)
    /* if(Set_bouclebloc(s))
    {
        set<string> tint;
	// tint.insert("");
        Res.insert(make_pair(tint,bddfalse)); // \88 modifier pour remplacer bddfalse par les \8Etats blocant
        cout<<"Im on the bouc bloc"<<endl;
        
        // DeadRdp=true;
        //cout<<"DeadRdp= "<<DeadRdp<<endl;
        
	}*/

    // cout<<"444"<<endl;
    set<Sets> lamda;
    
    for (map<set<string>,bdd>::const_iterator i= Res.begin();(i!=Res.end());i++)
    {
        lamda.insert((*i).first);
    }
    if(Set_bouclebloc(s))
    {  
	 set<string> tint;
	 lamda.insert(tint);
    }


    //cout<<"5555 "<<aff<<endl;
    
    //affichage de lamda | display lamda

    
  //  cout<<"hhhhcompute lamda"<<endl;
    return lamda;
}
/*-----------------------CanonizeR()----------------*/
bdd RdPBDD::CanonizeR(bdd s, unsigned int i)
{
    bdd s1,s2;
    do
    {
        itext++;
        s1 = s - bdd_nithvar(2*i);
        s2 = s - s1;
        if((!(s1==bddfalse))&&(!(s2==bddfalse)))
        {
            bdd front = s1;
            bdd reached = s1;
            do
            {
                //cout<<"premiere boucle interne \n";
                itint++;
                front=StepForward(front) - reached;
                reached = reached | front;
                s2 = s2 - front;
                //cout
                //j
            }while((!(front==bddfalse))&&(!(s2==bddfalse)));
        }
        if((!(s1==bddfalse))&&(!(s2==bddfalse)))
        {
            bdd front=s2;
            bdd reached = s2;
            do
            {
                // cout<<"deuxieme boucle interne \n";
                itint++;
                front=StepForward(front) - reached;
                reached = reached | front;
                s1 = s1 - front;
            }while((!(front==bddfalse))&&(!(s1==bddfalse)));
        }
        s=s1 | s2;
        i++;
    }while((i<Nb_places) &&((s1==bddfalse)||(s2==bddfalse)));
    if(i>=Nb_places)
    {
        //cout<<"____________oooooooppppppppsssssssss________\n";
        return s;
    }
    else
    {
        //cout<<"________________p a s o o o p p p s s s ______\n";
        return(CanonizeR(s1,i) | CanonizeR(s2,i));
    }
}
/*---------------------------  Set_Bloc()  -------*/
bool RdPBDD::Set_Bloc(bdd &S) const
{
    //cout<<"Ici detect blocage \n";
	int k=0;
	bdd Mt=bddtrue;
    for(vector<Trans>::const_iterator i = relation.begin();	   	i!=relation.end();i++,k++)
    {
	  //  cout<<"transition :"<<transitions[k].name<<endl;
        // cout<<"PRECOND :"<<bddtable<<(*i).Precond<<endl;
        //cout<<"POSTCOND :"<<bddtable<<(*i).Postcond<<endl;
	    //int toto;cin>>toto;
		Mt=Mt & !((*i).Precond);
    }
    // cout<<"finalstate :"<<bddtable<<finalstate<<endl;
    //  cout<<"S :"<<bddtable<<S<<endl;
    //  cout<<"MT :"<<bddtable<<(Mt )<<endl;
    
    //cout<<"MT& S :"<<bddtable<<(Mt & finalstate)<<endl;
   // cout<<"res bool :"<<(((S&Mt))!=bddfalse)<<endl;
    //cout<<"res bool :"<<bddtable<<((S&Mt)-finalstate)<<endl;
      //  cout<<"res  :"<<((((S&Mt))!=bddfalse)&&(((S&Mt)-finalstate)!=bddfalse))<<endl;

  return ((((S&Mt))!=bddfalse)&&(((S&Mt)-finalstate)!=bddfalse));

  //  return ((((S&Mt))!=bddfalse));
    //BLOCAGE
}

/*-------------------------Set_bouclebloc() \E0 revoir -----------------------------*/
bool RdPBDD::Set_bouclebloc(bdd &s)
{
    /*Set Obs=firable_obs(s);
     bdd temp=bddtrue;
     for(Set::const_iterator f= Obs.begin();!(f==Obs.end());f++)
     {
     temp=temp|FrontiereNodes2(s,(*f));
     }*/
   // cout<<"final state &s : "<<endl;
    //cout<<bddtable<<((s&finalstate))<<endl;
    bdd from=FrontiereNodes(s);
    if((s&finalstate)!=bddfalse)
       // if((s-finalstate)==bddfalse)

        from=from |(s&finalstate);
    bdd tmp;
    do
    {
        tmp=from;
       from=from|(StepBackward2(from,s));
      //        from=from|(StepBackward(s&from,s));

    }while(!(from==tmp));
    if(s==from)
        return false;
    else
        return true;
}
/*-------------------------Set_Div() \E0 revoir -----------------------------*/
bool RdPBDD::Set_Div(bdd &S) const
{
	Set::const_iterator i;
	bdd To,Reached;
	//cout<<"Ici detect divergence \n";
	Reached=S;
	do
	{
        bdd From=Reached;
		for(i=NonObservable.begin();!(i==NonObservable.end());i++)
		{
            
            To=relation[*i](Reached);
            Reached=Reached|To; //Reached=To ???
            //Reached=To;
		}
		if(Reached==From)
			//cout<<"SEQUENCE DIVERGENTE \n";
			return true;
		//From=Reached;
	}while(Reached!=bddfalse);
    return false;
	//cout<<"PAS DE SEQUENCE DIVERGENTE \n";
}

/*-----------FrontiereNodes() pour bdd ---------*/
bdd RdPBDD::FrontiereNodes(bdd From) const
{
	bdd res=bddfalse;
	for(Set::const_iterator i=Observable.begin();!(i==Observable.end());i++)
        res=res | (From & relation[*i].Precond);
	for(Set::const_iterator i=InterfaceTrans.begin();!(i==InterfaceTrans.end());i++)
        res=res | (From & relation[*i].Precond);
	return res;
}
/*-----------FrontiereNodes2() pour bdd ---------*/
bdd RdPBDD::FrontiereNodes2(bdd From,int i) const
{
	bdd res=bddfalse;
    
    res=res | (From & relation[i].Precond);
    
	return res;
}
/*-------- Produit synchronis\E9 \E0 la vol\E9e de n graphes d'observation : Adaptation \E0 la capture des s\E9quences bloquantes et les s\E9quences divergentes----------------------*/
void RdPBDD::GeneralizedSynchProduct1(Modular_Obs_Graph& Gv, int NbSubnets,RdPBDD* Subnets[] ,int nbbddvar,int stopvolee)
{
    cout<<"______________________  GeneralizedSynchProduct_________________________"<<endl;

    int pos_trans(TRANSITIONS,string);
    TabMeta=new bdd[1000000];
    nbmetastate=0;
    MaxIntBdd=0;
    nbmetastate=0;
    Stack st;
    set<string> uniontmp;
    //int cpt=0;
    int k;
    bdd *Complete_meta_state=new bdd[NbSubnets];
    Set *fire=new Set[NbSubnets];
    Set *fire2=new Set [NbSubnets];
    set<string> *tempObs=new set<string>[NbSubnets];
    set<string> unionObs,interO;
    Set unionfire;
    Set *intermfire=new Set[NbSubnets];
    Modular_Class_Of_State *Meta_State=new Modular_Class_Of_State;

    for(k=0;k<NbSubnets;k++)
    {
        Complete_meta_state[k]=Subnets[k]->Accessible_epsilon(Subnets[k]->M0);
        fire[k]=Subnets[k]->firable_obs(Complete_meta_state[k]);
        intermfire[k]=Subnets[k]->firable_trans(Complete_meta_state[k]);

        for(Set::const_iterator i=Subnets[k]->Observable.begin();!(i==Subnets[k]->Observable.end());i++)
        {tempObs[k].insert(Subnets[k]->transitions[*i].name);
            

        }
        Meta_State->State.push_back(Subnets[k]->CanonizeR(Complete_meta_state[k],0));
        TabMeta[nbmetastate]=Meta_State->State[k];
        nbmetastate++;
    }
    interO=tempObs[0];
    for(k=1;k<NbSubnets;k++){
        interO=unionObs;
    }

    old_size=bdd_anodecount(TabMeta,nbmetastate);
    Meta_State->blocage=true;
    for( k=0;((k<NbSubnets)&&(Meta_State->blocage));k++)
        Meta_State->blocage=Meta_State->blocage&&Subnets[k]->Set_Bloc(Complete_meta_state[k]);
    Meta_State->boucle=false;
    for(k=0;((k<NbSubnets)&&(!Meta_State->boucle));k++)
        Meta_State->boucle=Meta_State->boucle||Subnets[k]->Set_Div(Complete_meta_state[k]);
    Meta_State->final=false;
    for(int j=0;((j<NbSubnets)&&(!Meta_State->final));j++)
        Meta_State->final|=((Complete_meta_state[j]&Subnets[j]->finalstate)!=bdd_false());


    // set<string> res;
    // res=Subnets[0]->computelamda(Complete_meta_state[0],Meta_State->final,true);
    set<Sets>  *lamdas= new set<Sets>[NbSubnets];
    //   vector<set<string> >::iterator it= lamdas.begin();
    for(k=0;k<NbSubnets;k++)
    {
        lamdas[k]=Subnets[k]->computelamda(Complete_meta_state[k],1);
        
    }
    
    // Meta_State->lamda=Subnets[0]->computelamdaSyncro(lamdas,NbSubnets);
    Meta_State->lamda=Gv.computelamdaSyncro(lamdas,NbSubnets,tempObs,1);
    delete [] lamdas;
    Gv.setInitialState(Meta_State);
    Gv.insert(Meta_State);
    nbmetastate++;
    st.push(StackElt(Couple(Meta_State,Complete_meta_state),fire));
	 //   cout<<"combien delmnt?? "<<st.size()<<endl;

    do
    { //cout<<"nbit ="<<NbIt<<endl;
        NbIt++;
        StackElt e=st.top();
        st.pop();
        
      //  cout<<"SUIVI"<<endl;
        for(k=0;k<NbSubnets;k++)
        {
            while(!e.second[k].empty())
            {
                int t=*e.second[k].begin();
                e.second[k].erase(t);
                bool ok=true;
                Set ConcernedSubnets;
                ConcernedSubnets.insert(k);
                string tmp=Subnets[k]->transitions[t].name;
               // cout<<"tmp= "<<tmp<<endl;
                for(int j=0;j<NbSubnets;j++)
                {
                    if(j!=k)
                    {
                        int num=Subnets[j]->transitionName[tmp];
                   //     cout<<"num= "<<num<<endl;

                        int pos=pos_trans(Subnets[j]->transitions,tmp);
                        if((pos!=-1)&&!(Subnets[j]->Observable.find(num)==Subnets[j]->Observable.end()))
                        {
                            ConcernedSubnets.insert(j);
                       //     cout<<"ConcernedSubnets= "<<j<<endl;

                            if(e.second[j].find(num)==e.second[j].end())
                                ok=false;
                            else
                                e.second[j].erase(num);
                        }
                    }
                }
                if(ok)
                {
                    Complete_meta_state=new bdd[NbSubnets];
                    fire=new Set[NbSubnets];
                    fire2=new Set[NbSubnets];

                    Meta_State=new Modular_Class_Of_State;
                    for(int j=0;j<NbSubnets;j++)
                    {
                        if(ConcernedSubnets.find(j)==ConcernedSubnets.end())
                        {
                            Complete_meta_state[j]=e.first.second[j];
                            Meta_State->State.push_back(e.first.first->State[j]);
                        }
                        else
                        {
                            Complete_meta_state[j]=Subnets[j]->Accessible_epsilon(Subnets[j]->get_successor(e.first.second[j],Subnets[j]->transitionName[tmp]));
                            //Point de sortie
                            //Meta_State->State.push_back(Subnets[j]->FrontiereNodes(Complete_meta_state[j]));
                            //Meta_State->State.push_back(Subnets[j]->CanonizeR(Subnets[j]->FrontiereNodes(Complete_meta_state[j]),0));
                            Meta_State->State.push_back(Subnets[j]->CanonizeR(Complete_meta_state[j],0));
                            /*-------------------- STAT ----------------------*/
                            TabMeta[nbmetastate]=Meta_State->State[k];
                            nbmetastate++;
                            //cout<<"Subnets["<<j<<"]->transitionName[tmp]"<<transitions[(Subnets[j]->transitionName[tmp])].name<<endl<<endl;
                        }
                        fire[j]=Subnets[j]->firable_obs(Complete_meta_state[j]);
                        fire2[j]=Subnets[j]->firable_trans(Complete_meta_state[j]);

                        /*for(Set::const_iterator i=fire[j].begin();!(i==fire[j].end());i++)
                         cout<<"Test "<<j<<"!!  "<<(*i)<<" et "<<transitions[*i].name<<endl;
                         //set_union(fire[j].begin(),fire[j].end(),intermfire.begin(),intermfire.end(),inserter(unionfire,unionfire.begin()));
                         intermfire=unionfire;
                         cout<<"taille de intermfire est : "<<intermfire.size()<<endl;
                         cout<<"taille de unionfire est : "<<unionfire.size()<<endl;
                         cout<<"taille de transitions est : "<<transitions.size()<<endl;*/
                    }
                    
                    /*for(Set::const_iterator i=unionfire.begin();!(i==unionfire.end());i++)
                     cout<<"unionfire  "<<(*i)<<" et "<<transitions[*i].name<<endl;*/
                    Modular_Class_Of_State * pos=Gv.find(Meta_State);
                    if(!pos)
                    {
                        old_size=bdd_anodecount(TabMeta,nbmetastate);
                        //Calcul de deadlock et loop attributes
                        Meta_State->blocage=true;
                        for(int j=0;((j<NbSubnets)&&(Meta_State->blocage));j++)
                            Meta_State->blocage&=Subnets[j]->Set_Bloc(Complete_meta_state[j]);
                        Meta_State->boucle=false;
                        for(int j=0;((j<NbSubnets)&&(!Meta_State->boucle));j++)
                            Meta_State->boucle|=Subnets[j]->Set_Div(Complete_meta_state[j]);
                        Meta_State->final=false;
                        for(int j=0;((j<NbSubnets)&&(!Meta_State->final));j++)
                            Meta_State->final|=((Complete_meta_state[j]&Subnets[j]->finalstate)!=bdd_false());
                        //   cout<<"final = "<< Meta_State->final<<endl;
                        set<Sets>  *lamdas= new set<Sets>[NbSubnets];
                        for(int g=0;g<NbSubnets;g++)
                        {
                            lamdas[g]=Subnets[g]->computelamda(Complete_meta_state[g],1);
                        }
                        //  Meta_State->lamda=Subnets[0]->computelamdaSyncro(lamdas,NbSubnets);
                        Meta_State->lamda=Gv.computelamdaSyncro(lamdas,NbSubnets,tempObs,1);
                        delete [] lamdas;

                        /*    set<string>  glamdas[2];
                         set<string>  l;
                         l.insert("term");
                         l.insert("pspec");
                         
                         set<string>  l1; 
                         l1.insert("pcost");
                         glamdas[0]=l1;
                         glamdas[1]=l;
                         Meta_State->lamda=Subnets[0]->computelamdaSyncro(glamdas,2);*/
                        st.push(StackElt(Couple(Meta_State,Complete_meta_state),fire));
	  //  cout<<"combien delmnt?? "<<st.size()<<endl;

                        e.first.first->Successors.insert(e.first.first->Successors.begin(),Modular_Edge(Meta_State,tmp));
                        //cout<<"temp"<<tmp<<" et t: "<<t<<"et transitions:"<< transitions[t].name<<endl;
                        if(uniontmp.find(tmp)==uniontmp.end())
                            uniontmp.insert(tmp);
                        Meta_State->Predecessors.insert(Meta_State->Predecessors.begin(),Modular_Edge(e.first.first,tmp));
                        Gv.addArc();
                        Gv.insert(Meta_State);
                        
                        
                    }
                    else
                    {
                        e.first.first->Successors.insert(e.first.first->Successors.begin(),Modular_Edge(pos,tmp));
                        pos->Predecessors.insert(pos->Predecessors.begin(),Modular_Edge(e.first.first,tmp));
                        Gv.addArc();
                        delete Meta_State;
                        //Neoud d\E9ja rencontr\E9 ;
                    }
                    //Demander, en cas d'un blocage, si on veut terminer la consruction de GO
                   // if(stopvolee==1)
                    //{
                      //  set<string>::const_iterator t;
                        //t=Meta_State->lamda.find("EV");
                       /* if(t!=Meta_State->lamda.end())
                        {
                            cout<<"\n\tDEADLOCK !!!"<<endl; 
                            cout<<" \n\nCOMPLETE GRAPH'S BUILDING ?(y/n)"<<endl;
                            char c;
                            cin>>c;
                            if(c=='n'||c=='N')
                            {
                                //  cout<<"No"<<endl;
                                size_t n=1;
                                Gv.printCompleteInformation(NbSubnets);

                                  //Gv.printGraph(Gv.initialstate,n);
                                exit(0);
                            }
                            
                        }*/
                   // }
                } 
                
            }
            
        }
        
    }while(!st.empty());
    /*for(set<string>::const_iterator i=unionObs.begin();!(i==unionObs.end());i++)
     cout<<"unionObs est : "<<(*i)<<endl<<endl;
     for(set<string>::const_iterator i=uniontmp.begin();!(i==uniontmp.end());i++)
     cout<<"uniontmp est : "<<(*i)<<endl<<endl;*/
    if(uniontmp.size()==unionObs.size())
	{    	set<string> test;
		//set_union(uniontmp.begin(),uniontmp.end(),unionObs.begin(),unionObs.end(),inserter(test,test.begin()));
		/*for(set<string>::const_iterator i=test.begin();!(i==test.end());i++)
         cout<<"uniontmp est : "<<(*i)<<endl<<endl;*/
		if(test.size()==unionObs.size())  
            Gv.nodeadTrans=true;
		else Gv.nodeadTrans=false;
		
	}
    else
        Gv.nodeadTrans=false;

    /*cout<<"taille de intermfire est : "<<intermfire.size()<<endl;
     cout<<"taille de unionfire est : "<<unionfire.size()<<endl;
     cout<<"taille de transitions est : "<<transitions.size()<<endl;*/
    cout<<" MAXIMAL INTERMEDIARY BDD SIZE \n"<<MaxIntBdd<<endl;
    cout<<"OLD SIZE : "<<bdd_anodecount(TabMeta,nbmetastate)<<endl;
    cout<<"NB SHARED NODES : "<<bdd_anodecount(TabMeta,nbmetastate)<<endl;
    cout<<"NB META STATE DANS CONSTRUCTION : "<<nbmetastate<<endl;
    cout<<"NB ITERATIONS CONSTRUCTION : "<<NbIt<<endl;
    cout<<"NB ITERATIONS EXTERNES : "<<itext<<endl;
    cout<<"NB ITERATIONS INTERNES : "<<itint<<endl;


}
set<string> Union(set<string> s1,set<string> s2)
{
    set<string> Res;
    set<string>::const_iterator it1=s1.begin();
    set<string>::const_iterator it2=s2.begin();
    while(it1!=s1.end() &&it2!=s2.end())
    {
        int comp=(*it1).compare((*it2));
        if(!comp)
        {
            Res.insert(*it1);
            *it1++;
            *it2++;
        }
        else
            if(comp<0)
            {
                Res.insert(*it1);
                *it1++;
            }
            else
            {
                Res.insert(*it2);
                *it2++;
            }
    }
    while(it1!=s1.end())
    {
        Res.insert(*it1);
        it1++;
    }
    while(it2!=s2.end())
    {
        Res.insert(*it2);
        it2++;
    }
    return Res;
}

//parcours en profondeur

void RdPBDD::GeneralizedSynchProduct_Prof(Modular_Obs_Graph& Gv, int NbSubnets,RdPBDD* Subnets[] ,int nbbddvar,int stopvolee)
{
    cout<<"_____________  GeneralizedSynchProduct_Prof _________________________"<<NbSubnets<<"sous-reseaux "<<endl;
    int pos_trans(TRANSITIONS,string);
    TabMeta=new bdd[1000000];
    nbmetastate=0;
    MaxIntBdd=0;
    nbmetastate=0;
    Stack st;
    set<string> uniontmp;
    int k;
    bdd *Complete_meta_state=new bdd[NbSubnets];
    Set *fire=new Set[NbSubnets];
    Set *fire2=new Set [NbSubnets];
    set<string> *tempObs=new set<string>[NbSubnets];
    set<string> unionObs,interO;
    // Set *unionfire=new Set[NbSubnets],*intermfire=new Set[NbSubnets];
    Set unionfire;
    // set<string> *Et=new set<string>[NbSubnets];
    set<string> GlobalET,GlobalEtMeta;

    Modular_Class_Of_State *Meta_State=new Modular_Class_Of_State;
    //Remplir les tableaux avc les noms des transitions obs de chak reseau
    for(k=0;k<NbSubnets;k++)
    {
        Complete_meta_state[k]=Subnets[k]->Accessible_epsilon(Subnets[k]->M0);
        fire[k]=Subnets[k]->firable_obs(Complete_meta_state[k]);
	set<string> nonobs = Subnets[k]->firable_Nonobs(Complete_meta_state[k]);
	//	//set_union(Meta_State->Et.begin(),Meta_State->Et.end(),nonobs.begin(),nonobs.end(),inserter(Meta_State->Et, Meta_State->Et.begin()));

        for(Set::const_iterator i=Subnets[k]->Observable.begin();!(i==Subnets[k]->Observable.end());i++)
        {tempObs[k].insert(Subnets[k]->transitions[*i].name);
            
	  //cout<<"TEST "<<k<<"!!  "<<(*i)<<" et "<<Subnets[k]->transitions[*i].name<<endl;            
        }
      
        Meta_State->State.push_back(Subnets[k]->CanonizeR(Complete_meta_state[k],0));
      
        /*-------------------- STAT ----------------------*/
        TabMeta[nbmetastate]=Meta_State->State[k];
        nbmetastate++;
    }
    
    //union des Obs de chak reseau
    interO=tempObs[0];
    for(k=1;k<NbSubnets;k++){
      // //set_union(interO.begin(),interO.end(),tempObs[k].begin(),tempObs[k].end(),inserter(unionObs,unionObs.begin()));
        interO=unionObs;
    }
    /*for(set<string>::const_iterator i=unionObs.begin();!(i==unionObs.end());i++)
     cout<<"unionObs est : "<<(*i)<<endl<<endl;
     */
    //intermfire=fire2;
    // Meta_State->Et=Gv.computeEt(Et,tempObs,NbSubnets);
    GlobalET=Meta_State->Et;
 for(set<string>::const_iterator i= Meta_State->Et.begin();!(i== Meta_State->Et.end());i++)
     cout<<"Et est : "<<(*i)<<endl<<endl;
     
    set<Sets>  *lamdas= new set<Sets>[NbSubnets];
    //   vector<set<string> >::iterator it= lamdas.begin();
    for(k=0;k<NbSubnets;k++)
    {
        lamdas[k]=Subnets[k]->computelamda(Complete_meta_state[k],1);
        
    }
    

    Meta_State->lamda=Gv.computelamdaSyncro(lamdas,NbSubnets,tempObs,1);
    delete [] lamdas;
    Gv.setInitialState(Meta_State);
    Gv.insert(Meta_State);
    nbmetastate++;
    st.push(StackElt(Couple(Meta_State,Complete_meta_state),fire));


    do
    { 
        NbIt++;
        StackElt e=st.top();
        //st.pop();
	bool empty=true;
	for(k=0;k<NbSubnets && empty;k++)
        {
	  empty= empty && e.second[k].empty();
	  // { empty=true;
	  //	cout<<"Here Empty"<<endl;
	  //  }
	}
	int kk=k;
	 if(!empty)
	 {               
	   bool ok=true;
	   string tmp;
	   Set ConcernedSubnets;

		do
	       {
                int t=*e.second[k].begin();
		cout<<"t : "<<Subnets[k]->transitions[t].name<<endl;
                e.second[k].erase(t);
                ConcernedSubnets.insert(k);
                tmp=Subnets[k]->transitions[t].name;

                for(int j=kk;j<NbSubnets;j++)
                {
                    if(j!=k)
                    {
                        int num=Subnets[j]->transitionName[tmp];
                   //     cout<<"num= "<<num<<endl;

                        int pos=pos_trans(Subnets[j]->transitions,tmp);
                        if((pos!=-1)&&!(Subnets[j]->Observable.find(num)==Subnets[j]->Observable.end()))
                        {
                            ConcernedSubnets.insert(j);
                       //     cout<<"ConcernedSubnets= "<<j<<endl;

                            if(e.second[j].find(num)==e.second[j].end())
                                ok=false;
                            else
                                e.second[j].erase(num);
                        }
                    }
                } 
		k++;
	       }while(k<NbSubnets&&!ok);


                if(ok)
                {
		  e.first.first->Et.insert(tmp);
                    Complete_meta_state=new bdd[NbSubnets];
                    fire=new Set[NbSubnets];
                    fire2=new Set[NbSubnets];

                    Meta_State=new Modular_Class_Of_State;
                    for(int j=0;j<NbSubnets;j++)
                    {
                        if(ConcernedSubnets.find(j)==ConcernedSubnets.end())
                        {
                            Complete_meta_state[j]=e.first.second[j];
                            Meta_State->State.push_back(e.first.first->State[j]);
                        }
                        else
                        {
                            Complete_meta_state[j]=Subnets[j]->Accessible_epsilon(Subnets[j]->get_successor(e.first.second[j],Subnets[j]->transitionName[tmp]));
                            Meta_State->State.push_back(Subnets[j]->CanonizeR(Complete_meta_state[j],0));
                            /*-------------------- STAT ----------------------*/
                            TabMeta[nbmetastate]=Meta_State->State[k];
                            nbmetastate++;
                            //cout<<"Subnets["<<j<<"]->transitionName[tmp]"<<transitions[(Subnets[j]->transitionName[tmp])].name<<endl<<endl;
                        }
                        fire[j]=Subnets[j]->firable_obs(Complete_meta_state[j]);
                        fire2[j]=Subnets[j]->firable_trans(Complete_meta_state[j]);
			//	intermfire[j]=Subnets[j]->firable_trans(Complete_meta_state[j]);
			set<string> nonobs = Subnets[j]->firable_Nonobs(Complete_meta_state[j]);
			//////set_union(Meta_State->Et.begin(),Meta_State->Et.end(),nonobs.begin(),nonobs.end(),inserter(Meta_State->Et, Meta_State->Et.begin()));


		
                    }
                    
                    /*for(Set::const_iterator i=unionfire.begin();!(i==unionfire.end());i++)
                     cout<<"unionfire  "<<(*i)<<" et "<<transitions[*i].name<<endl;*/
                    Modular_Class_Of_State * pos=Gv.find(Meta_State);
                    if(!pos)
                    {

                        old_size=bdd_anodecount(TabMeta,nbmetastate);
                        //Calcul de deadlock et loop attributes
                        
                        //   cout<<"final = "<< Meta_State->final<<endl;
                        set<Sets>  *lamdas= new set<Sets>[NbSubnets];
                        for(int g=0;g<NbSubnets;g++)
                        {
                            lamdas[g]=Subnets[g]->computelamda(Complete_meta_state[g],1);
                        }
                        //  Meta_State->lamda=Subnets[0]->computelamdaSyncro(lamdas,NbSubnets);
                        Meta_State->lamda=Gv.computelamdaSyncro(lamdas,NbSubnets,tempObs,1);
                        delete [] lamdas;
			//	Meta_State->Et=Gv.computeEt(Et,tempObs,NbSubnets);
			for(set<string>::const_iterator i= Meta_State->Et.begin();!(i== Meta_State->Et.end());i++)
			  cout<<"Et est : "<<(*i)<<endl<<endl;

			//	GlobalET=Union(Meta_State->Et,GlobalET);

     
	      cout<<"Aggregat Empilee"<<endl;

                        
                        st.push(StackElt(Couple(Meta_State,Complete_meta_state),fire));


                        e.first.first->Successors.insert(e.first.first->Successors.begin(),Modular_Edge(Meta_State,tmp));
                        //cout<<"temp"<<tmp<<" et t: "<<t<<"et transitions:"<< transitions[t].name<<endl;
                        if(uniontmp.find(tmp)==uniontmp.end())
                            uniontmp.insert(tmp);
                        Meta_State->Predecessors.insert(Meta_State->Predecessors.begin(),Modular_Edge(e.first.first,tmp));
                        Gv.addArc();
                        Gv.insert(Meta_State);
                        
                        
                    }
                    else
                    {
                        e.first.first->Successors.insert(e.first.first->Successors.begin(),Modular_Edge(pos,tmp));
                        pos->Predecessors.insert(pos->Predecessors.begin(),Modular_Edge(e.first.first,tmp));
                        Gv.addArc();
                        delete Meta_State;
                        //Neoud d\E9ja rencontr\E9 ;
                    }
                   
                } 
                
    


	 }
	    else 
	      {

	    st.pop();

	    ///// //set_union(e.first.first->Et.begin(),e.first.first->Et.end(),GlobalET.begin(),GlobalET.end(),inserter(GlobalET,GlobalET.begin()));

	    }
		
            
       
        
    }while(!st.empty());
    /*for(set<string>::const_iterator i=unionObs.begin();!(i==unionObs.end());i++)
     cout<<"unionObs est : "<<(*i)<<endl<<endl;
     for(set<string>::const_iterator i=uniontmp.begin();!(i==uniontmp.end());i++)
     cout<<"uniontmp est : "<<(*i)<<endl<<endl;*/
    if(uniontmp.size()==unionObs.size())
	{    	set<string> test;
	  ////	//set_union(uniontmp.begin(),uniontmp.end(),unionObs.begin(),unionObs.end(),inserter(test,test.begin()));
		/*for(set<string>::const_iterator i=test.begin();!(i==test.end());i++)
         cout<<"uniontmp est : "<<(*i)<<endl<<endl;*/
		if(test.size()==unionObs.size())  
            Gv.nodeadTrans=true;
		else Gv.nodeadTrans=false;
		
	}
    else
        Gv.nodeadTrans=false;
    /*cout<<"taille de intermfire est : "<<intermfire.size()<<endl;
     cout<<"taille de unionfire est : "<<unionfire.size()<<endl;
     cout<<"taille de transitions est : "<<transitions.size()<<endl;*/
    cout<<"MAXIMAL INTERMEDIARY BDD SIZE "<<MaxIntBdd<<endl;
    cout<<"OLD SIZE : "<<bdd_anodecount(TabMeta,nbmetastate)<<endl;
    cout<<"NB SHARED NODES : "<<bdd_anodecount(TabMeta,nbmetastate)<<endl;
    cout<<"NB META STATE DANS CONSTRUCTION : "<<nbmetastate<<endl;
    cout<<"NB ITERATIONS CONSTRUCTION : "<<NbIt<<endl;
    cout<<"NB ITERATIONS EXTERNES : "<<itext<<endl;
    cout<<"NB ITERATIONS INTERNES : "<<itint<<endl;
}

/*------------------------EmersonLey ----------------------------*/
bdd RdPBDD::EmersonLey(bdd S,bool trace) 
{
	//cout<<"ICI EMERSON LEY \n";
	double		TpsInit, TpsDetect;
	double debitext,finitext;
	TpsInit = (double)(clock()) / CLOCKS_PER_SEC;
	bdd b=S;
	bdd Fair=bdd_ithvar(2*Nb_places-1);
	cout<<"PLACE TEMOIN \n";
	//cout<<places[places.size()-1].name<<endl;
	bdd oldb;
	bdd oldd,d;
	int extit=0;
	int init=0;
	do
	{
		
		extit++;
		if(trace)
		{
			
			cout<<"ITERATION EXTERNES NUMERO :"<<extit<<endl;
			debitext=(double)(clock()) / CLOCKS_PER_SEC;
			cout<<"TAILLE DE B AVANT IT INTERNE : "<<bdd_nodecount(b)<<endl;
			cout<<endl<<endl;
		}
		oldb=b;
		//cout<<"Fair : "<<Fair.id()<<endl;
		d=b & Fair;
		//cout<<"d : "<<d.id()<<endl;
		//init=0;
		do
		{
			init++;
			if(trace)
			{
				
				cout<<"ITERATION INTERNES NUMERO :"<<init<<endl;
				cout<<"HEURE : "<<(double)(clock()) / CLOCKS_PER_SEC<<endl;
				cout<<"TAILLE DE D : "<<bdd_nodecount(d)<<endl;
			}
			oldd=d;
			bdd inter=b & StepForward2(d);
			//cout<<"Tille de inter :"<<bdd_nodecount(inter)<<endl;
			d=d | inter;
		}while(!(oldd==d));
		if(trace)
			cout<<"\nTAILLE DE D APRES ITs INTERNES : "<<bdd_nodecount(d)<<endl;
		b=b & StepBackward2(d);
		init++;
		if(trace)
		{
			cout<<"\n\nTAILLE DE B APRES ELEMINER LES PRED DE D : "<<bdd_nodecount(b)<<endl;
			finitext=(double)(clock()) / CLOCKS_PER_SEC;
			cout<<"DUREE DE L'ITERATION EXTERNE NUMERO "<<extit<<"  :  "<<finitext-debitext<<endl;
			cout<<endl<<"_________________________________________________\n\n";
		}
	}while(!(b==oldb));
	cout<<"NOMBRE D'ITERATIONS EXTERNES -----:"<<extit<<endl;
	cout<<"NOMBRE D'ITERATIONS INTERNES -----:"<<init<<endl;
	TpsDetect = ((double)(clock()) / CLOCKS_PER_SEC) - TpsInit;
	cout << "DETECTION DE CYCLE TIME  " << TpsDetect << endl;
	return b;
}

