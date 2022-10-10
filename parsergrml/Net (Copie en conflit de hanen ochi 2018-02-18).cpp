#include <string>
#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <ext/hash_map>
#include <algorithm>
#include "Net.h"
#define TAILLEBUFF 5000000
/***********************************************************/
/*                      class Node                         */
/***********************************************************/

void  Node::addPre(int node,int valuation){
    pair<int,int> x(node,valuation);
    pre.push_back(x);
}

void Node::addPost(int node,int valuation){
    pair<int,int> x(node,valuation);
    post.push_back(x);
}

void  Node::addInhibitor(int node,int valuation){
    pair<int,int> x(node,valuation);
    inhibitor.push_back(x);
}

void  Node::addPreAuto(int node,int valuation){
    pair<int,int> x(node,valuation);
    preAuto.push_back(x);
}

void Node::addPostAuto(int node,int valuation){
    pair<int,int> x(node,valuation);
    postAuto.push_back(x);
}

void Node::addReset(int node){
    reset.push_back(node);
}

/***********************************************************/
/*                      class RdPE                         */
/***********************************************************/

/*-----Constructeur Prod--------------*/
/*net::net(const char *f,const char*Formula_trans, const char* Int_trans,const char *PlaceF){
    cout<<"CREATION D'UN NOUVEAU SOUS-RESEAU \n";
    if(create(f)){
        // cout<<"create(f)"<<endl;
        
        for (vector<class Place>::iterator p=places.begin();p!=places.end();p++){
            //cout<<"p->name: "<<p->name<<endl;
            
            sort(p->pre.begin(),p->pre.end());
            sort(p->post.begin(),p->post.end());
        }
        for (vector<class Transition>::iterator p=transitions.begin();p!=transitions.end();p++){
            // cout<<"!!! t->name: "<<p->name<<endl;
            sort(p->pre.begin(),p->pre.end());
            sort(p->post.begin(),p->post.end());
        }
    }
    else{ //cout<<"else"<<endl;
        places.clear();
        transitions.clear();
        placeName.clear();
        transitionName.clear();
	exit(0);
    }
    if(strlen(Formula_trans)>0)
    {
      //    cout<<"transitions de la formule non vide \n";
      
        Set_Formula_Trans(Formula_trans);
        

        
        if(strlen(Int_trans)>0)
        {cout<<" fichier d'interface est donné et  qui est : "<<Int_trans<<endl;
            Set_Interface_Trans(Int_trans);
            //cout<<"transitions de l'interface non vide \n";
        }
        // cout<<"______________66666666666666666666666______________________\n";
        set_union(InterfaceTrans.begin(), InterfaceTrans.end(),Formula_Trans.begin(),Formula_Trans.end(), inserter(Observable, Observable.begin()));
        Set_Non_Observables();
        
    }
    else
	{ // cout<<"heheeeeeee"<<endl;
        for(unsigned int i=0;i<transitions.size();i++)
		{  Observable.insert(i);
            //cout<<"i= "<<i<<endl;
        }
    }
    
    
    cout<<"FIN CREATION \n";
}

*/



/*------Constructeur Grml-------------------*/
net::net(const char *f){
    cout<<"CREATION D'UN NOUVEAU SOUS-RESEAU Grml \n";
    if(createNet(f)){
        // cout<<"create(f)"<<endl;
        
        for (vector<class Place>::iterator p=places.begin();p!=places.end();p++){
            //cout<<"p->name: "<<p->name<<endl;
            
            sort(p->pre.begin(),p->pre.end());
            sort(p->post.begin(),p->post.end());
        }
        for (vector<class Transition>::iterator p=transitions.begin();p!=transitions.end();p++){
            // cout<<"!!! t->name: "<<p->name<<endl;
            sort(p->pre.begin(),p->pre.end());
            sort(p->post.begin(),p->post.end());
        }
    }
    else{ //cout<<"else"<<endl;
        places.clear();
        transitions.clear();
        placeName.clear();
        transitionName.clear();
	exit(0);
    }
    
    
    cout<<"FIN CREATION \n";
}



/*---------------------------------Init Set of  transitions ------------------------------*/
/*---------------------------------Set_formula_trans()------------------*/
bool net::Set_Formula_Trans(const char * f)
{
	FILE *in;
	int i,nb;
	//	cout<<"ici set formula transitions \n";
	//	int pos_trans (TRANSITIONS, string);
	char Buff[TAILLEBUFF], *z;
	in = fopen(f,"r");
	if (in == NULL)
	{
		cout << "file " << f << " doesn't exist" << endl;
		exit(1);
	}
	int nb_formula_trans;
	fscanf(in,"%d\n",&nb_formula_trans);
  	nb = fread(Buff,1,TAILLEBUFF-1,in);
  	Buff[nb] = '\0';
	z = strtok(Buff, " \t\n");
	//cout<<"taille "<<TAILLEBUFF<<" buff "<<Buff<<" z: "<<z<<"#"<<endl;
	for(i= 0;i<nb_formula_trans;i++)
	{
        // cout<<" z: "<<z<<"#"<<endl;
		if(z == NULL)
		{
			cout << "error in formula trans format "<< endl;
			return false;
	  	}
		string tmp(z);
		bool found=false;
		int pos=0;
        //cout<<"ammmmmmma"<<endl;
		for(TRANSITIONS::const_iterator i=transitions.begin();!(i==transitions.end());i++,pos++)
    {
      //  cout<<"test ici : "<<i->name<<"#"<<endl;
	    if(i->name==tmp)
	      {
		found=true;
		Formula_Trans.insert(pos);
		//	cout<<"Obs "<<transitions[pos].name<<endl;
	      }
 }
	
		if(!found)
		{      		 // cout<<"ttttttttttttttt"<<endl;
		    cout<<z<<"    Error??? : observale transition "<<tmp<<" doesn't exist \n";
		    //return false;
		}
		z = strtok(NULL, " \t\n");
		if (z == NULL)
		{
  			nb = fread(Buff,1,TAILLEBUFF-1,in);
  			Buff[nb] = '\0';
			z = strtok(Buff, " \t\n");
		}
	}
	fclose(in);
    cout<<"fin formule_trans fct"<<endl;
	return true;
}

/*---------------------------Set a final state------------------------*/
/*bool net::finalS(const char * f)
{	FILE *in;
	int nb;
	int pos_place ( PLACES, string);
	char Buff[TAILLEBUFF], *z;
	in = fopen(f,"r");
    if (in == NULL)
        
  	{
		cout << "file " << f << " doesn't exist!!!!!" << endl;
        exit(1);
	}
	int nb_final_state;
	fscanf(in,"%d\n",&nb_final_state);
  	nb = fread(Buff,1,TAILLEBUFF-1,in);
	//cout<<"nb = "<<nb<<endl;
  	Buff[nb] = '\0';
	z = strtok(Buff, "\n");
    Set SetF;
    if(z==NULL)
    {
        Final_States.insert(SetF);
    }
	while(z!=NULL)
    {cout<<"z: "<<z<<"*"<<endl;
        
	    string s;
	    s=z;
	    Set SF;
	    while(s.find(",")!=-1)
        {
            string zz=s.substr(0,s.find(","));
            //cout<<"zz : "<<zz<<"#"<<endl;
            int pos= pos_place(places,zz);
            //cout<<"pos de :"<<zz<<"est : "<<pos<<endl;
            if(pos==-1)
            {
                cout<<z<<"    Error??? : finale state "<<zz<<" doesn't exist \n";
                //return false;
            }
			else
                SF.insert(pos);
            s=s.substr(s.find(",")+1);
        }
	    Final_States.insert(SF);
        
	    z= strtok(NULL,"\n");
    }
    
 
 
	fclose(in);
	return true;
    
	}*/
/*---------------------------------Set_Interface_trans()------------------*/
bool net::Set_Interface_Trans(const char * f)
{
	FILE *in;
	int i,nb;
	int pos_trans (TRANSITIONS, string);
	char Buff[TAILLEBUFF],* z;
	in = fopen(f,"r");
	if (in == NULL)
	{
		cout << "file " << f << " doesn't exist mmmmmmmmmmm" << endl;
		exit(1);
	}
	int int_trans;
	fscanf(in,"%d\n",&int_trans);
  	nb = fread(Buff,1,TAILLEBUFF-1,in);
  	Buff[nb] = '\0';
	z = strtok(Buff, " \t\n");
	//cout<<"taille "<<TAILLEBUFF<<" buff "<<Buff<<" z: "<<z<<endl;
	for(i= 0;i<int_trans;i++)
	{
        //cout<<" z: "<<z<<endl;
		if(z == NULL)
		{
			cout << "error in interface format "<< endl;
			return false;
	  	}
		string tmp(z);
		
		int pos=pos_trans(transitions,tmp);
		//if(Formula_Trans.find(pos)==Formula_Trans.end())
		if(pos==-1)
		{
			cout<<z<<"         Error??? : interface transition doesn't exist \n";
            //	return false;
		}
		else
            InterfaceTrans.insert(pos);
		z = strtok(NULL, " \t\n");
		if (z == NULL)
		{
  			nb = fread(Buff,1,TAILLEBUFF-1,in);
  			Buff[nb] = '\0';
			z = strtok(Buff, " \t\n");
		}
	}
	fclose(in);
    cout<<"fin interface_tran fct"<<endl;
	return true;
}
/*---------------------------------Set_Non_Observables()------------------*/
void net::Set_Non_Observables()
{
	NonObservable.clear();
	for(unsigned int i=0;i<transitions.size();i++)
		if(Observable.find(i)==Observable.end())
		{ //cout<<"i= "<<i<<endl;
            //for(TRANSITIONS::const_iterator j=transitions.begin();!(j==transitions.end());j++)
            
            //cout<<"test ici : "<<j->name<<"#"<<endl;
			//   cout<<"!!! t->name: "<<transitions[i-1].name<<endl;
			NonObservable.insert(i);
		}
}
/*-----------------------pos_trans()--------------------*/
int pos_trans(TRANSITIONS T, string trans)
{
	int pos=0;
	//	cout<<"on cherche "<<trans<<"* dans :\n";
	for(TRANSITIONS::const_iterator i=T.begin();!(i==T.end());i++,pos++)
    {
        //  cout<<"test ici : "<<i->name<<"#"<<endl;
	    if(i->name==trans)
			return pos;
    }
	//cout<<"Non trouve :\n";
	return -1;
}
/*-----------------------pos_place()--------------------*/
int pos_place(PLACES P, string place)
{
	int pos=0;
	//	cout<<"on cherche "<<trans<<" dans :\n";
	for(PLACES::const_iterator i=P.begin();!(i==P.end());i++,pos++)
    {
	    //cout<<i->name<<"*"<<endl;
	    if(i->name==place)
			return pos;
    }
	cout<<"Non trouve :\n";
	return -1;
}
/* ------------------------------ operator<< -------------------------------- */
ostream& operator<<(ostream& os, const Set& s)
{
	bool b = false;
	
	if (s.size())
	{
		for (Set::const_iterator i=s.begin(); !(i==s.end()); i++)
		{
			if (b)
				os << ", ";
			else
				os << "{";
			os  << *i<<" ";
			b = true;
		}
		os << "}";
	}
	else
		os << "empty set";
	return os;
    
}
/*----------------------------------------------------------------------*/
bool net::addPlace(const string &place,const int &id,int marking,int capacity){
   map<string,int>::const_iterator pi=placeName.find(place);
   if(pi==placeName.end() && findPlaceByid(id)==-1){
    placeName[place]=places.size();
    Place p(id,place,marking, capacity);
    places.push_back(p);
    return true;
  }
  else
    return false;
}

bool net::addQueue(const string &place,const int &id,int capacity){
  map<string,int>::const_iterator pi=placeName.find(place);
  if(pi==placeName.end()){
    placeName[place]=places.size();
    Place p(id,place,-1, capacity);
    places.push_back(p);
    return true;
  }
  else
    return false;
}

bool net::addLossQueue(const string &place,const int &id,int capacity){
   map<string,int>::const_iterator pi=placeName.find(place);
  if(pi==placeName.end()){
    placeName[place]=places.size();
    Place p(id,place,-2, capacity);
    places.push_back(p);
    return true;
  }
  else
    return false;
}

bool net::addTrans(const string &trans,const int &id,const int obs){
  if(findTransitionByid(id)!=-1)
    return false;
  transitionName[trans]=transitions.size();
  Transition t(id,trans);
  transitions.push_back(t);
  if(obs)
    Observable.insert(transitions.size()-1);
  return true;
}
bool net::addPre(const int &src,const int &tg,int valuation){
  int p,t,posp,post;
  posp=findPlaceByid(src);
  if(posp==-1)
    return false;
 post=findTransitionByid(tg);
  if(post==-1)
    return false;

  transitions[post].addPre(posp,valuation);
  places[posp].addPost(post,valuation);
  return true;
}
bool net::addPost(const int &src,const int &tg,int valuation){
    
 int p,t,posp,post;
  posp=findPlaceByid(tg);
  if(posp==-1)
    return false;
 post=findTransitionByid(src);
  if(post==-1)
    return false;

  transitions[post].addPost(posp,valuation);
  places[posp].addPre(post,valuation);
  return true;
}
// Les fcts suivantes (jusqua addReset) ne sont pas utilisées ... a adapter en cas d'une éventuelle prise en compte
bool net::addPreQueue(const string &place,const string &trans,int valuation){
    int p,t;
  map<string,int>::const_iterator pi=placeName.find(place);
  if(pi==placeName.end() || !places[pi->second].isQueue())
    return false;
  else
    p=pi->second;
  map<string,int>::const_iterator ti=transitionName.find(trans);
  if(ti==transitionName.end())
    return false;
  else
    t=ti->second;
  transitions[t].addPre(p,valuation);
  places[p].addPost(t,valuation);
  return true;
}

bool net::addPostQueue(const string &place,const string &trans,int valuation){
   int p,t;
  map<string,int>::const_iterator pi=placeName.find(place);
  if(pi==placeName.end() || !places[pi->second].isQueue())
    return false;
  else
    p=pi->second;
  map<string,int>::const_iterator ti=transitionName.find(trans);
  if(ti==transitionName.end())
    return false;
  else
    t=ti->second;
  transitions[t].addPost(p,valuation);
  places[p].addPre(t,valuation);
  return true;
}

bool net::addInhibitor(const string &place,const string &trans,int valuation){
    int p,t;
  map<string,int>::const_iterator pi=placeName.find(place);
  if(pi==placeName.end())
    return false;
  else
    p=pi->second;
  map<string,int>::const_iterator ti=transitionName.find(trans);
  if(ti==transitionName.end())
    return false;
  else
    t=ti->second;
  transitions[t].addInhibitor(p,valuation);
  places[p].addInhibitor(t,valuation);
  return true;
}
bool net::addPreAuto(const string &place,const string &trans,const string &valuation){
  int p,t,v;
  map<string,int>::const_iterator pi=placeName.find(place);
  if(pi==placeName.end() || places[pi->second].isQueue())
    return false;
  else
    p=pi->second;
  map<string,int>::const_iterator ti=transitionName.find(trans);
  if(ti==transitionName.end())
    return false;
  else
    t=ti->second;
  map<string,int>::const_iterator pv=placeName.find(valuation);
  if(pv==placeName.end() || places[pv->second].isQueue())
    return false;
  else
    v=pv->second;
  transitions[t].addPreAuto(p,v);
  places[p].addPostAuto(t,v);
  return true;
}

bool net::addPostAuto(const string &place,const string &trans,const string &valuation){
  int p,t,v;
  map<string,int>::const_iterator pi=placeName.find(place);
  if(pi==placeName.end() || places[pi->second].isQueue())
    return false;
  else
    p=pi->second;
  map<string,int>::const_iterator ti=transitionName.find(trans);
  if(ti==transitionName.end())
    return false;
  else
    t=ti->second;
  map<string,int>::const_iterator pv=placeName.find(valuation);
  if(pv==placeName.end() || places[pi->second].isQueue())
    return false;
  else
    v=pv->second;
  transitions[t].addPostAuto(p,v);
  places[p].addPreAuto(t,v);
  return true;
}

bool net::addReset(const string &place,const string &trans){
    int p,t;
  map<string,int>::const_iterator pi=placeName.find(place);
  if(pi==placeName.end())
    return false;
  else
    p=pi->second;
  map<string,int>::const_iterator ti=transitionName.find(trans);
  if(ti==transitionName.end())
    return false;
  else
    t=ti->second;
  transitions[t].addReset(p);
  places[p].addReset(t);
  return true;
}


/* Add final marking */
void net::addFinalMarking(map<int,int> *&fm)
{
  cout<<"nouveau marquage final: [ ";
  for(map<int,int>::const_iterator it=(*fm).begin();it!=(*fm).end();it++)
    cout<<it->first<<"*"<<it->second<<" ";
  cout<<" ]"<<endl;
  Final_Markings.push_back(*fm);
  delete fm;
}
/* Visualisation */
ostream& operator<<(ostream &os,const net &R){
    
    /* affichage nombre de places et de transitions */
    os<<"***************************"<<endl;
    os<<"Nombre de places     :"<<R.nbPlace()<<endl;
    os<<"Nombre de transitions:"<<R.nbTransition()<<endl;
    
    /* affichage de la liste des places */
    os<<"********** places **********"<<endl;
    int i=0;
    for (vector<class Place>::const_iterator p=R.places.begin();p!=R.places.end();p++,i++){
        if (p->isQueue()) {
            os<<"queue " << setw(4)<<i<<":"<<p->name<<", cp("<<p->capacity<<")";
            if (p->isLossQueue())
                cout << " loss";
            cout << endl;
        }
        else
            os<<"place " << setw(4)<<i<<":"<<p->name<<":"<<p->marking<<" <..>, cp(" << p->capacity<<")"<<endl;
    }
    os<<"********** transitions  de la formule  **********"<<endl;
    for(Set::const_iterator h=R.Formula_Trans.begin();!(h==R.Formula_Trans.end());h++)
        cout<<R.transitions[*h].name<<endl;
    /* os<<"********** Places finales  **********"<<endl;
    for(Set::const_iterator h=R.Final_State.begin();!(h==R.Final_State.end());h++)
    cout<<R.places[*h].name<<endl;*/
    os<<"********** transitions  de l'interface  **********"<<endl;
    for(Set::const_iterator h=R.InterfaceTrans.begin();!(h==R.InterfaceTrans.end());h++)
        cout<<R.transitions[*h].name<<endl;
    os<<"Nombre de transitions observable:"<<R.Observable.size()<<endl;
    os<<"********** transitions observables **********"<<endl;
    for(Set::const_iterator h=R.Observable.begin();!(h==R.Observable.end());h++)
        cout<<R.transitions[*h].name<<endl;
    os<<"Nombre de transitions non observees:"<<R.NonObservable.size()<<endl;
    os<<"********** transitions  non observees **********"<<endl;
    for(Set::const_iterator h=R.NonObservable.begin();!(h==R.NonObservable.end());h++)
        cout<<R.transitions[*h].name<<endl;
    i=0;
    os<<"Nombre global de transitions :"<<R.nbTransition()<<endl;
    os<<"********** transitions  **********"<<endl;
    for (TRANSITIONS::const_iterator t=R.transitions.begin();t!=R.transitions.end();t++,i++)
    {
        os<<setw(4)<<i<<":"<<t->name<<endl;
        
        // os<<"        IN  { ";
        //for (vector< pair<int,int> >::const_iterator a=p->pre.begin();a!=p->pre.end();a++)
        //  if (R.places[a->first].isQueue())
        //  os<<R.places[a->first].name<<":<"<<a->second<<">;";
        //  else
        //  os<<R.places[a->first].name<<":"<<a->second<<";";
        //    for (vector< pair<int,int> >::const_iterator a=p->inhibitor.begin();a!=p->inhibitor.end();a++)
        //os<<R.places[a->first].name<<"<"<<a->second<<";";
        //for (vector< pair<int,int> >::const_iterator a=p->preAuto.begin();a!=p->preAuto.end();a++)
        //os<<R.places[a->first].name<<":"<<R.places[a->second].name<<";";
        //for (vector<int>::const_iterator a=p->reset.begin();a!=p->reset.end();a++)
        //os<<R.places[*a].name<<": reset;";
        //os<<" }"<<endl;
        
        //os<<"        OUT { ";
        //for (vector< pair<int,int> >::const_iterator a=p->post.begin();a!=p->post.end();a++)
        //  if (R.places[a->first].isQueue())
        //  os<<R.places[a->first].name<<":<"<<a->second<<">;";
        //  else
        //  os<<R.places[a->first].name<<":"<<a->second<<";";
        //for (vector< pair<int,int> >::const_iterator a=p->postAuto.begin();a!=p->postAuto.end();a++)
        //os<<R.places[a->first].name<<":"<<R.places[a->second].name<<";";
        //os<<" }"<<endl;
    }
    return(os);
}
