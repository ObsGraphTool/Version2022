
/********              Graph.cpp     *******/
#include"Simple_MDGraph.h"
#define TAILLEBUFF 500
//#include<conio.h>
#include <cstdlib>
#include <./trim.h>
#include <utility>
#include<string.h>
#include <iostream>
#include <fstream>
//bdd * Temp;//tableau intermédiaire pour calculer la taille (nb bdd) du graphe
/*********                  setInitialState    *****/
/*----------------------insert() ------------*/
void Simple_MDGraph::insert( Simple_Class_Of_State *c)
{
	this->GONodes.push_back(c);
	nbStates++;
}
/*----------------------find()----------------*/
Simple_Class_Of_State * Simple_MDGraph::find(string nom)
{//cout<<"on cherche "<<nom<<"*"<<endl;
	for(Simple_MDGraph_Nodes::const_iterator i=GONodes.begin();!(i==GONodes.end());i++)
	  { //cout<<"nommmmmm: "<<(*i)->nom<<"*"<<endl;//if((c->class_state.id()==(*i)->class_state.id())&&(c->blocage==(*i)->blocage)&&(c->boucle==(*i)->boucle))
	  if((*i)->nom.find(nom)!=-1)
	    {	return *i; cout<<"YESSSSSSSSS"<<endl;}}
		return NULL;

}
/*----------------------find()----------------*/
Simple_MDGraph_Nodes::const_iterator Simple_MDGraph::find2(Simple_Class_Of_State * c)
{
	for(Simple_MDGraph_Nodes::const_iterator i=GONodes.begin();!(i==GONodes.end());i++)
	  {
	    //cout<<"it->nom: "<<c->nom<<" et (*i)->nom: "<<(*i)->nom<<endl;
 if(c->nom.compare((*i)->nom)==0)
	return i;
	}
	//	return i;
}

/**************************** get_successor************************************************/
Simple_Class_Of_State* Simple_MDGraph::get_successor(Simple_Class_Of_State *SC,string o)
{
  Simple_Class_Of_State* res;
  for(set<pair<string,Simple_Class_Of_State*> >::const_iterator i=SC->Successors.begin();i!=SC->Successors.end();i++)
    {
      if((*i).first==o)
	res=(*i).second;
    }
  // res=(*(SC->Successors.find(o))).second;
  return res;
}
/**************************** get_successor************************************************/
set<Simple_Class_Of_State*> Simple_MDGraph::get_successor2(Simple_Class_Of_State *SC,set<string> o)
{
  set<Simple_Class_Of_State*> res;
  for(set<string>::const_iterator i=o.begin();i!=o.end();i++)
    { // cout<<"on cherche successeur de *"<<SC->nom<<"* en franchissant :"<<(*i)<<"*"<<endl;
    if(((*i).find("term")==-1)&&((*i).find("EV")==-1))
	 {
	  for(set<pair<string,Simple_Class_Of_State*> >::const_iterator ii=SC->Successors.begin();ii!=SC->Successors.end();ii++)
	    {//cout<<"Un des successeurs en franchissant: "<<(*ii).first<<(*i)<<endl;
	     string der=(*ii).first;
   der.erase (der.find_last_not_of (" " ) + 1);
	     // cout<<"der *"<<der<<"*et kkkkkkkkk *"<<(*i)<<"* et *"<<((*i)==(der))<<endl;
	   //    if((der.compare(*i))!=string::npos)
 if(der==(*i))
		{
		  //cout<<"le bon succ est: "<<((*ii).second)->nom<<endl;
		  res.insert((*ii).second);
		}
	    }

	}
      else

	  res.insert(NULL);
    }
  return res;
}

/***********************constructor1**************************/
Simple_MDGraph::Simple_MDGraph(const char* s)
{
  FILE *in;
  int nb;
  char Buff[TAILLEBUFF], *z;
 string sk;
  in = fopen(s,"r");
if (in == NULL)
  {
    cout << "file " << s<< " doesn't exist!!!!!" << endl;
    exit(1);
  }
nb = fread(Buff,1,TAILLEBUFF-1,in);
Buff[nb] = '\0';
 sk=Buff;
 while(sk.find("#state: ")!=-1)
   {
    
     string nom,lamda;
     nom=sk.substr(sk.find(":")+1,(sk.find("{")-sk.find(":"))-1);
     while(nom.find(" ")!=-1)
 	 nom=(nom.substr(0,nom.find(" ")))+(nom.substr(nom.find(" ")+1));
     // cout<<"nom detat: *"<<nom<<"*"<<endl;
   
     lamda=sk.substr(sk.find("{")+1,(sk.find("\n")-sk.find("{"))-2);
     // cout<<"lamda: "<<lamda<<"/lamda"<<endl;
     Simple_Class_Of_State* SC=new Simple_Class_Of_State( nom,lamda);
     insert(SC);
     sk=sk.substr(sk.find("\n")+1);
     // cout<<"z= *"<<sk<<"*"<<endl;

   }
 /*for(Simple_MDGraph_Nodes::const_iterator i=GONodes.begin();!(i==GONodes.end());i++)
  { cout<<"nommmmmm: "<<(*i)->nom;
    for(set<string>::const_iterator h=(*i)->lamda.begin();h!=(*i)->lamda.end();h++)
    cout<<"et "<<(*h)<<endl;}*/

while(sk.find("#trans: ")!=-1)
  {
    // cout<<"heyyoo :"<<sk<<endl;
    string etat=sk.substr(sk.find("<")+1,(sk.find(",")-sk.find("<"))-1);
    // cout<<"etat"<<etat<<"/etat"<<endl;
    sk=sk.substr(sk.find(",")+1);
    string obs=sk.substr(0,sk.find(","));
    // cout<<"obs:"<<obs<<"/obs"<<endl;
    sk=sk.substr(sk.find(",")+1);
    string etatsuiv=sk.substr(0,sk.find(">"));
    // cout<<"etat suiv"<<etatsuiv<<"/etatsuiv"<<endl;
    sk=sk.substr(sk.find("\n")+1);

  Simple_Class_Of_State * h, *p;
  pair<string,Simple_Class_Of_State*>   R;
  h=find(etat);
  R.first=obs;
  R.second=find(etatsuiv);
  h->Successors.insert(R);
  }
	 
/* for(Simple_MDGraph_Nodes::const_iterator i=GONodes.begin();!(i==GONodes.end());i++)

   { cout<<"etat:"<<(*i)->nom<<" avec lamda : {";
for(set<string>::const_iterator h=(*i)->lamda.begin();h!=(*i)->lamda.end();h++)
  cout<<"{"<<(*h)<<"}";
 cout<<"}"<<endl;
 for (set<pair<string,Simple_Class_Of_State*> >::const_iterator ir= (*i)->Successors.begin();(ir!=(*i)->Successors.end());ir++)
	   {
	     cout<<"\t"<<(*ir).first<<" ---->: "<<(*ir).second->nom<<endl;
	   }
   }
*/
}

/***********************computeResGraph**************************/
 Simple_MDGraph Simple_MDGraph::computeResGraph(Simple_MDGraph g1)
{
  Simple_MDGraph RG;
  if(nbStates==0)
    { return g1;
      /*    GONodes=g1.GONodes;
	    nbStates=g1.nbStates;*/
    }
  else 
    {
      //cout<<"on est la"<<endl;
      /*for(Simple_MDGraph_Nodes::const_iterator i=g1.GONodes.begin();!(i==g1.GONodes.end());i++)
	{ cout<<"nommmmmm: "<<(*i)->nom;
	for(set<string>::const_iterator h=(*i)->lamda.begin();h!=(*i)->lamda.end();h++)
	cout<<"et "<<(*h)<<endl;}*/
      Simple_MDGraph_Nodes::const_iterator i=g1.GONodes.begin(), j=GONodes.begin();
      string lamdares;
      set<string> lamdas[2];
      set<string> lamdasf;
      lamdas[0]=(*i)->lamda;
      lamdas[1]=(*j)->lamda;
      lamdasf=computelamdaSyncro(lamdas,2);
      Simple_Class_Of_State * r= new Simple_Class_Of_State((*i)->nom+(*j)->nom,lamdasf);
  RG.insert(r);
      cout<<"\netat: "<<r->nom<<endl;
      cout<<"\tlamda:{";
      for(set<string>::const_iterator it=r->lamda.begin();it!=r->lamda.end();it++)
	cout<<"{"<<(*it)<<"}";
      cout<<"}"<<endl;
     
      while((i!=(g1.GONodes.end()-1))||(j!=(GONodes.end()-1)))
	{     
	  // cout<<"g1.GONOdes: "<<(*i)->nom<<"   GONodes: "<<(*j)->nom<<endl;
	  set< Simple_Class_Of_State*>  R1,R2;
	  /*for(set<string>::const_iterator it=lamdasf.begin();it!=lamdasf.end();it++)
	    cout<<"lamdasf:"<<(*it)<<endl*/
	  R1=g1.get_successor2((*i),lamdasf);
	  R2=get_successor2((*j),lamdasf);
set<string>::const_iterator tt=lamdasf.begin();
	  for(set<Simple_Class_Of_State*>::const_iterator it1=R1.begin(), it2=R2.begin();it1!=R1.end(),it2!=R2.end();it1++,it2++)
	{
	  //cout<<"tt "<<(*tt)<<endl;
	    if(((*it1)!=NULL)&&((*it2)!=NULL))
	      {  
		i=g1.find2(*it1);
		j=find2(*it2);
	      }	  
	    else  tt++;
	  }	
 pair<string,Simple_Class_Of_State*>   R;

	  R.first=(*tt);
		  R.second=new Simple_Class_Of_State((*i)->nom+(*j)->nom,lamdasf);

		  (*(RG.find2(r)))->Successors.insert(R);
	  lamdas[0]=(*i)->lamda;
	  lamdas[1]=(*j)->lamda;
	  lamdasf=computelamdaSyncro(lamdas,2);
	 

	  /*	  for(set<Simple_Class_Of_State*>::const_iterator it1=R1.begin(), it2=R2.begin();it1!=R1.end(),it2!=R2.end();it1++,it2++)
	    { 	
	     
		  //cout<<"etat courant"<<(*it1)->nom<<"/"<<(*it2)->nom<<endl;
		 
		  pair<string,Simple_Class_Of_State*>   R;
		  R.first=obs;
		  R.second=new Simple_Class_Of_State((*i)->nom+(*j)->nom,lamdasf);
//find(etatsuiv);
		  h->Successors.insert(R);
	      if((*it1)!=NULL)	 
		i=g1.find2(*it1);
	      if((*it2)!=NULL)
		j=find2(*it2);

	      lamdas[0]=(*i)->lamda;
	      lamdas[1]=(*j)->lamda;
	      lamdasf=computelamdaSyncro(lamdas,2);

	      Simple_Class_Of_State * r2= new Simple_Class_Of_State((*i)->nom+(*j)->nom,lamdasf);
	      cout<<"etat : "<<r2->nom<<endl;
	      cout<<"\tlamda:{";
	      for(set<string>::const_iterator it=r2->lamda.begin();it!=r2->lamda.end();it++)
		cout<<"{"<<(*it)<<"}";
	      cout<<"}"<<endl;
	      RG.insert(r2);
r.
	    }*/

	 
		
	  Simple_Class_Of_State * r2= new Simple_Class_Of_State((*i)->nom+(*j)->nom,lamdasf);
	  cout<<"etat : "<<r2->nom<<endl;
	  cout<<"\tlamda:{";
	  for(set<string>::const_iterator it=r2->lamda.begin();it!=r2->lamda.end();it++)
	    cout<<"{"<<(*it)<<"}";
	  cout<<"}"<<endl;
	  RG.insert(r2);
	  r=r2;
	}

    }
  for(Simple_MDGraph_Nodes::const_iterator i=RG.GONodes.begin();!(i==RG.GONodes.end());i++)

   { cout<<"etat:"<<(*i)->nom<<" avec lamda : {";
for(set<string>::const_iterator h=(*i)->lamda.begin();h!=(*i)->lamda.end();h++)
  cout<<"{"<<(*h)<<"}";
 cout<<"}"<<endl;
 for (set<pair<string,Simple_Class_Of_State*> >::const_iterator ir= (*i)->Successors.begin();(ir!=(*i)->Successors.end());ir++)
	   {
	     cout<<"\t"<<(*ir).first<<" ---->: "<<(*ir).second->nom<<endl;
	   }
	   }

  return RG;

 
}
/**************************saveonfile(s)****************************/
void Simple_MDGraph::saveonfile(string f)
{ 
  ofstream myfile (std::string(f).c_str());
  if (myfile.is_open())
    {
      for(Simple_MDGraph_Nodes::const_iterator i=GONodes.begin();!(i==GONodes.end());i++)
	{
	  myfile<<"#state: "<<(*i)->nom<<" {";
	  // cout<<"nommmmmm: "<<(*i)->nom;
	  for(set<string>::const_iterator h=(*i)->lamda.begin();h!=(*i)->lamda.end();h++)
	    //cout<<"et "<<(*h)<<endl;
	    myfile<<"{"<<(*h)<<"}";
	  myfile<<"}"<<endl;
	}
      myfile<<"\n";
     
      for(Simple_MDGraph_Nodes::const_iterator i=GONodes.begin();!(i==GONodes.end());i++)
	{ 
	  cout<<"\n NOM "<<(*i)->nom<<" --> Successors : ";
	  for(set<pair<string,Simple_Class_Of_State*> >::const_iterator h=(*i)->Successors.begin();h!=(*i)->Successors.end();h++)
	    { 
	      cout<<(*h).second->nom<<" BY "<<(*h).first<<" // ";
	      myfile<<"#trans: <"<<(*i)->nom<<","<<(*h).first<<","<<(*h).second->nom<<">"<<endl;
	    }
	}
      myfile.close();
    }
}
/******************computelamdaSynchro()***************************/
set<string> Simple_MDGraph::computelamdaSyncro(set<string> s[2],int nb)
{
  set<string> Res;
  bool found[nb-1];
 
  for(int i=0;i<nb;i++)
    { //for(int i=0;i<nb-1;i++)
    found[i]=false;
      for(set<string>::const_iterator it1=s[i].begin();it1!=s[i].end();it1++)
	
	{string cr=(*it1);
	  int k =0;
	  for (int j=0;j<nb;j++) 
	    {     
	     if(j!=i)
		{
		  bool test=true;
		  set<string>::const_iterator it2=s[j].begin();
		  while (it2!=s[j].end()&& test)
		    {
		      if(((*it2).find((*it1))==-1))
			   if((*it1).find(*it2)==-1)
			      test=false;
			  
			   else 
			     cr=(*it2);
		      it2++;
		    }
		  found[k]=test;
		  k++;
		 }
	    }
	  bool founds=true;
	  for(int m=0;m<nb-1;m++)
	    if(found[m]==false)
	      founds=false;
	
	  if(founds /*&& (Res.find(*it1)==Res.end())*/)
	      Res.insert((cr));
	  else
	      Res.insert("EV");

	}   
    }

   return Res;
  
}
