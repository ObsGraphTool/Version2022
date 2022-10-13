
/********              Graph.cpp     *******/
#include"Modular_Obs_Graph.h"
#define TAILLEBUFF 500
//#include<conio.h>
#include<queue>
using namespace std;
bdd * Temp;//tableau interm�diaire pour calculer la taille (nb bdd) du graphe
/*********                  setInitialState    *****/

void Modular_Obs_Graph::setInitialState(Modular_Class_Of_State *c)
{
	currentstate=initialstate=c;

}
/*----------------------find()----------------*/
Modular_Class_Of_State * Modular_Obs_Graph::find(Modular_Class_Of_State* c)
{
	bool arret;
	for(Modular_Obs_Graph_Nodes::const_iterator i=GONodes.begin();!(i==GONodes.end());i++)
	{
		//if((c->blocage!=(*i)->blocage)||(c->boucle!=(*i)->boucle))
		//{
			arret=false;
			for(unsigned int k=0;((k<(c->State).size())&&(!arret));k++)
				if(!(c->State[k]==(*i)->State[k]))
					arret=true;
			if(!arret)
				return *i;
		//}
	}
		return NULL;
}
/*----------------------find2()----------------*/
//version modulaire on v�rifie la projection
Modular_Class_Of_State * Modular_Obs_Graph::find2(Modular_Class_Of_State* c, Set SRConcernes)
{
	bool arret;
	for(Modular_Obs_Graph_Nodes::const_iterator i=GONodes.begin();!(i==GONodes.end());i++)
	{
		//if((c->blocage!=(*i)->blocage)||(c->boucle!=(*i)->boucle))
		//{
			arret=false;
			for(unsigned int k=0;((k<(c->State).size())&&(!arret));k++)
				if(!(SRConcernes.find(k)==SRConcernes.end()))
					if(!(c->State[k]==(*i)->State[k]))
						arret=true;
			if(!arret&&(c->blocage==(*i)->blocage)&&(c->boucle==(*i)->boucle))
				return *i;
		//}
	}
		return NULL;
}
/*-----------------------AddARc2----------------*/
void Modular_Obs_Graph::addArc(Modular_Class_Of_State* Pred,Modular_Class_Of_State* Suc,const char* t)
{
	//cout<<"ici addArc entre "<<*Pred<<" et "<<*Suc<<endl;
	Modular_Edge arc=Modular_Edge(Suc,t);
	Modular_Edge cra=Modular_Edge(Pred,t);
	if(Pred->Successors.find(arc)==Pred->Successors.end())
	{
		//cout<<"OK \n";
		Pred->Successors.insert(Pred->Successors.begin(),arc);
		Suc->Predecessors.insert(Suc->Predecessors.begin(),cra);
		nbArcs++;
	}
	//else
	//	cout<<"KO \n";
	//int toto;cin>>toto;
}
/*----------------------insert() ------------*/
void Modular_Obs_Graph::insert(Modular_Class_Of_State *c)
{
	c->Visited=false;
	this->GONodes.push_back(c);
	nbStates++;
}

/*----------------------InitVisit()------------------------*/
void Modular_Obs_Graph::InitVisit(Modular_Class_Of_State * S,size_t nb)
{
	//cout<<"____________ nb = __________________ "<<nb<<endl;
	//cout<<"____________ nbStates = __________________ "<<nbStates<<endl;
	if(nb<=nbStates)
	{
	//	cout<<"nb < = nbStates\n";
		S->Visited=false;
		for(Modular_Edges::const_iterator i=S->Successors.begin();!(i==S->Successors.end());i++)
		{
				
				if((*i).first->Visited==true)
				{
					nb++;
					InitVisit((*i).first,nb);
				}
		}
		
	}
}
/*----------------------NbBddNod()------------------------*/
void Modular_Obs_Graph::TAB_BDDNODES(Modular_Class_Of_State * S, size_t &nb)
{

	if(S->Visited==false)
	{
		//cout<<"ETAT NON VISITE : ";
		//cout<<*S<<endl;
		//int tt;cin>>tt;
		for(unsigned int k=0;k<S->State.size();k++,nb++)
			Temp[nb-1]=S->State[k];
		nb--;
		S->Visited=true;
		//int bddnode=0;
		//for(k=0;k<S->State.size();k++)
		//	bddnode+=bdd_nodecount(S->State[k]);
		//int size_succ=0;
		for(Modular_Edges::const_iterator i=S->Successors.begin();!(i==S->Successors.end());i++)
		{
			if((*i).first->Visited==false)
			{
				nb++;
				TAB_BDDNODES((*i).first,nb);
			}
		}		
	}
}
/*------------------------------------------Affichage du graphe -------------*/
void Modular_Obs_Graph::printCompleteInformation(int nbsubnets) {

    if (DeadMG) {
        cout << "-------------------------------------------" << endl;
        cout << endl << "!!!THE GRAPH CONTAINS A DEADLOCK STATE!!!" << endl;
    } else
    {gicout << "-------------------------------------------" << endl;
    cout << endl << "***THE GRAPH IS DEADLOCKFREE***" << endl;
    }
        cout<<"-------------------------------------------"<<endl;

    cout << "\n\nGRAPH SIZE : \n";
	cout<< "\n\tNB MARKING : "<< nbMarking;
	cout<< "\n\tNB NODES : "<< nbStates;
	cout<<"\n\tNB ARCS : " <<nbArcs<<endl;


    char c;
//	cin>>c;
	//InitVisit(initialstate,n);
	Temp=new bdd[nbStates*nbsubnets];
	size_t n=1;
	//cout<<"NB BDD NODE : "<<NbBddNode(initialstate,n)<<endl;
	TAB_BDDNODES(initialstate,n);
	cout<<"\tNB BDD NODE : "<<bdd_anodecount(Temp,nbStates*nbsubnets)<<endl;
	//cout<<"Shared Nodes : "<<bdd_anodecount(Tab,nbStates)<<endl;
	InitVisit(initialstate,1);
/*
	if(c=='y'||c=='Y')
	{
		 size_t n=1;
		 printGraph(initialstate,n);
	}
*/
}
/*********                  printGraph    *****/
	
void Modular_Obs_Graph::printGraph(Modular_Class_Of_State *s,size_t &nb)
{
	//cout<<"Print Graph \n";
	//int toto;cin>>toto;
	if(nb<=nbStates)
	{
		cout<<"\nSTATE NUMBER "<<nb<<" : \n";
		s->Visited=true;
		printsuccessors(s);
		getchar();
		printpredecessors(s);
		getchar();
		Modular_Edges::const_iterator i;
		for(i=s->Successors.begin();!(i==s->Successors.end());i++)
		{
			if((*i).first->Visited==false)
			{
				nb++;
				printGraph((*i).first, nb);
			}
		}
		
	}	  
			
}
/*-------------------------Reset()----------------------------*/
void Modular_Obs_Graph::Reset()
{
	currentstate=NULL;
	nbArcs=nbMarking=nbStates=0;
}
/*---------void print_successors_class(Class_Of_State *)------------*/
void Modular_Obs_Graph::printsuccessors(Modular_Class_Of_State *s)
{

	Modular_Edges::const_iterator i;
	cout<<*s<<endl;
	if(s->boucle)
		cout<<"\n\tON BOUCLE DESSUS AVEC EPSILON\n";
	if(s->blocage)
		cout<<"\n\tEXISTENCE D'UN ETAT BLOCANT\n";
	cout<<"\n\tSES SUCCESSEURS SONT  ( "<<s->Successors.size()<<" ) :\n\n";
	getchar();
	for(i =s->Successors.begin();!(i==s->Successors.end());i++)
	{
		cout<<" \t---"<<(*i).second<<" -->";
		cout<<*((*i).first)<<endl;
		getchar();
	}
}
/*---------void printpredescessors(Class_Of_State *)------------*/
void Modular_Obs_Graph::printpredecessors(Modular_Class_Of_State *s)
{
	Modular_Edges::const_iterator i;
	cout<<"\n\tSES PREDESCESSEURS SONT  ( "<<s->Predecessors.size()<<" ) :\n\n";
	getchar();
	for(i =s->Predecessors.begin();!(i==s->Predecessors.end());i++)
	{
		cout<<" \t---"<<(*i).second<<" -->";
		cout<<*(*i).first<<endl;
		getchar();
	}
}


/*----------------------void reachfinal()-----------------*/
void Modular_Obs_Graph::reachfinal(int nbsubnets)
{   //cout<<endl<<"---------------reachfinal()----------------------------"<<endl;

	//InitVisit(initialstate,n);
	Temp=new bdd[nbStates*nbsubnets];
	size_t ns=1;
	TAB_BDDNODES(initialstate,ns);
		//cout<<"ns= "<<ns<<endl;
	InitVisit(initialstate,1);
		int n=1;
reachfinalfromstate(initialstate,n);
		//cout<<"n= "<<n<<endl;
		//cout<<"NS= "<<ns<<endl;
//if(n<ns) cout<<" WE can NOT reach a final state from one of the Graph's states"<<endl;
//else  cout<<" WE can reach a final state from each Graph's state"<<endl;
}

void Modular_Obs_Graph::reachfinalfromstate(Modular_Class_Of_State *s,int& nb)
{ int trouve=0;
	if(nb<=nbStates)
	{
		//cout<<"\nSTATE NUMBER "<<nb<<" : \n";
		s->Visited=true;
		//cout<<"trouve1= "<<trouve<<endl;
		reachfinalsuccessors(s,trouve);
		//	cout<<"trouve2= "<<trouve<<endl;

		// si on veut tester par rapport a chak state il faut mettre trouve comme variable locale a cette fonction et le test suivant sera si trouve!=1
	if (trouve!=1) {
		//cout<<"WE can NOT reach a final state from state number "<<nb<<" root number "<<bddtable<<"**"<<endl;
		stop=1;
	}
	else {
			//cout<<"WE can reach a final state from state number "<<nb<<" root number "<<bddtable<<"\n"<<s->class_state<<endl;
	if(stop==0)
{
//cout<<"we are here"<<endl;
			//Edges::const_iterator i;
		for(Modular_Edges::const_iterator i=s->Successors.begin();!(i==s->Successors.end());i++)

			{	//if((*i).first->final()) cout<<"ICI c final"<<endl;
				if((*i).first->Visited==false)
				{
					nb++;
					reachfinalfromstate((*i).first, nb);
				}
			}
		}
		}
	}	  
}


void Modular_Obs_Graph::reachfinalsuccessors(Modular_Class_Of_State *s,int & trouve)
{	
		Modular_Edges::const_iterator i;
	if(s->final) {
		  //cout<<"\tHELOOOOOOOOOO EXISTANCE D'UN ETAT FINAL"<<endl;
		  trouve=1;}
if(trouve!=1)
	for(i =s->Successors.begin();!(i==s->Successors.end());i++)
	{reachfinalsuccessors ((*i).first,trouve);
		//cout<<"kkkkkkkkkkkk"<<endl;

}}

void Modular_Obs_Graph::checksoundness()
{
cout<<endl<<"-------------------------------------------"<<endl;
	 if (!DeadMG && stop==0)
        cout<<"CONC: Option to complete is verified ..."<<endl;
 	else    cout<<"CONC: Option to complete is not verified ...  "<<endl;
	if (nodeadTrans)         cout<<"      No dead transitions is verified ... "<<endl;
	else cout<<"      No dead transitions is not verified ..."<<endl;
	if (pc)         cout<<"      proper completion is verified ..."<<endl<<endl;
	else cout<<"      proper completion is not verified ..."<<endl<<endl;
	if (!DeadMG && stop==0 && nodeadTrans && pc)         cout<<"      	The composition is sound ! "<<endl;
	else cout<<"      	The composition is not sound  !"<<endl;
	    cout<<"-------------------------------------------"<<endl;
}

void Modular_Obs_Graph::checkweaksoundness()
{
    cout<<endl<<"-------------------------------------------"<<endl;
    if (!DeadMG && stop==0)
        cout<<"CONC: Weak soundness is verified on the composition..."<<endl;
 	else    cout<<"CONC: Weak soundness is not verified on the composition...  "<<endl;
	    cout<<"-------------------------------------------"<<endl;
}
void Modular_Obs_Graph::checkeasysoundness()
{
    int finalfrominit=0;
    reachfinalsuccessors(initialstate, finalfrominit);
    cout<<endl<<"-------------------------------------------"<<endl;
    if (finalfrominit)
        cout<<"CONC: Easy soundness is verified on the composition ..."<<endl;
 	else    cout<<"CONC: Easy soundness is not verified on the composition ...  "<<endl;
	
    cout<<"-------------------------------------------"<<endl;
}
/*********** intersection entre deux set of string *************/
Sets Intersection(Sets s1,Sets s2)
{
    Sets Res;
    Sets::const_iterator it1=s1.begin();
    Sets::const_iterator it2=s2.begin();

    while(it1!=s1.end() && it2!=s2.end())
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
                *it1++;
            else
                *it2++;
    }
    return Res;
}
/*********** Minus entre deux set of string s1 - s2 *************/
Sets Minus(Sets s1,Sets s2)
{
    Sets Res;
    Sets::const_iterator it1=s1.begin();
    Sets::const_iterator it2=s2.begin();


    while(it1!=s1.end() && it2!=s2.end())
    {
        int comp=(*it1).compare((*it2));
        if(!comp)
        {
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

                *it2++;
    }
    while(it1!=s1.end())
    {
        Res.insert(*it1);
        it1++;
    }
    return Res;
}

/*********** Union entre deux set of string *************/
Sets Union_(Sets s1,Sets s2)
{
    Sets Res;
    Sets::const_iterator it1=s1.begin();
    Sets::const_iterator it2=s2.begin();

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
Sets Composition(Sets e1, Sets e2, Sets o1, Sets o2)
{


    return Union_(Union_(Intersection(e1,e2),Intersection(e1,Minus(o1,o2))),Intersection(e2,Minus(o2,o1)));

}
Sets CompositionEt(Sets e1, Sets e2, Sets o1, Sets o2)
{
    //cout<<"ici composition "<<endl;
  return Union_(Minus(e1,Intersection(o1,o2)),Intersection(e1,e2));
}
void Insertion(set<Sets> lambda, Sets e )
{
    
}
/******************computelamdaSynchro()****************/
set<string> Modular_Obs_Graph::computeEt(set<string> *Et, set<string> *O,int nb )
{
  set<string> s=O[0];
  set<string> Et_cour=Et[0];
    

    for(int i=1;i<nb;i++)
      {
	// for(set<string>::const_iterator it1=Et[i].begin(),it2=O[i].begin();it1!=Et[i].end(),it2!=O[i].end();it1++,it2++)  
	  { 
	    //Et_cour=Union_(Minus(Et_cour,Intersection(s,O[i])),Intersection(Et_cour,Et[i]));

	  }

      }

  return Et_cour;


}
/******************computelamdaSynchro()****************/
set<Sets> Modular_Obs_Graph::computelamdaSyncro(set<Sets>* s,int nb,set<string> * tempObs,int aff)
{
    Sets res;
    set<Sets> lambda_cour=s[0];
    set<string> Obsactuel=tempObs[0];
    int i;

    for(i=1;i<nb;i++)
    {
        set<Sets> lambda_nouv;
        for(set<Sets>::const_iterator it1=lambda_cour.begin();it1!=lambda_cour.end();it1++)
            for(set<Sets>::const_iterator it2=s[i].begin();it2!=s[i].end();it2++)
            {
                res=Composition(*it1,*it2,Obsactuel,tempObs[i]);
                if(res.size()==0 && i==nb-1)
                    DeadMG=true;
                lambda_nouv.insert(res);
            }


        lambda_cour=lambda_nouv;
        
        Obsactuel=Union_(Obsactuel,tempObs[i]);
    }


    return lambda_cour;
}
/*-----------------contruct_Modular_Obs_Graph()---------------------*/
void Modular_Obs_Graph::construct_Modular_Obs_Graph(set<const char*> l, int n)
{
  cout<<"Debut fct modular construct GO"<<endl;
  int nbetat[n];
  int pos[n];
  for(int g=0;g<n;g++)
	nbetat[g]=0;
 for(int g=0;g<n;g++)
	pos[g]=1;
  int k=0;
  set<string> *list_meta_states =new set<string>[n];


  for(set<const char*>::const_iterator t=l.begin();t!=l.end();t++)
    {
      FILE *in;
      int nb;
      char Buff[TAILLEBUFF], *z;
      in = fopen((*t),"r");
      if (in == NULL)
	{
	  cout << "file " << (*t) << " doesn't exist!!!!!" << endl;
	  exit(1);
	}
      nb = fread(Buff,1,TAILLEBUFF-1,in);
      Buff[nb] = '\0';
      z = strtok(Buff, "#");
      while(z!=NULL)
	{

	    string s;
	  s=z;
	  // cout<<"nom detat: "<<s.substr(0,s.find(" "))<<endl;
	  //s=s.substr(s.find(" ")+1);
	  cout<<"lamda: "<<s.substr(0,s.find("\n"))<<endl<<endl;
	  list_meta_states[k].insert(s.substr(0,s.find("\n")));
	  s=s.substr(s.find("\n")+1);

	  nbetat[k]++;
	  z= strtok(NULL,"#");

	}
      cout<<"nb etats : "<<nbetat[k]<<endl;
      k++;
    }
  set<string> tabfinal;
  int h=0;
  string nometatinitial="";
  while(h<n)
    {
      nometatinitial=nometatinitial+((*list_meta_states[h].begin()).substr(0,(*list_meta_states[h].begin()).find(" ")));
      h++;
    }
	cout<<"ici :"<<nometatinitial<<"++"<<endl;

  cout<<"FIN fct modular construct GO"<<endl;
    delete [] list_meta_states;
}

