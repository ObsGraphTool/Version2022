
/********              Graph.cpp     *******/
#include"MDGraph.h"
/*#include <conio>*/
#include <cstdlib>
bdd * Tab;
/*********                  setInitialState    *****/

void MDGraph::setInitialState(Class_Of_State *c)
{
	currentstate=initialstate=c;
    
}
/*----------------------find()----------------*/
Class_Of_State * MDGraph::find(Class_Of_State* c)
{
	for(MetaGrapheNodes::const_iterator i=GONodes.begin();!(i==GONodes.end());i++)
        //if((c->class_state.id()==(*i)->class_state.id())&&(c->blocage==(*i)->blocage)&&(c->boucle==(*i)->boucle))
        if(c->class_state.id()==(*i)->class_state.id())
			return *i;
    return NULL;
}


/*----------------------insert() ------------*/
void MDGraph::insert(Class_Of_State *c)
{
	c->Visited=false;
	this->GONodes.push_back(c);
	nbStates++;
}

/*----------------------NbBddNod()------------------------*/
int MDGraph::NbBddNode(Class_Of_State * S, size_t& nb)
{
	if(S->Visited==false)
	{
		//cout<<"insertion du meta etat numero :"<<nb<<"son id est :"<<S->class_state.id()<<endl;
		//cout<<"sa taille est :"<<bdd_nodecount(S->class_state)<<" noeuds \n";
		
		Tab[nb-1]=S->class_state;
		S->Visited=true;
		int bddnode=bdd_nodecount(S->class_state);
		int size_succ=0;
		for(Edges::const_iterator i=S->Successors.begin();!(i==S->Successors.end());i++)
		{
			if((*i).first->Visited==false)
			{
				nb++;
				size_succ+=NbBddNode((*i).first,nb);
			}
		}
		return size_succ+bddnode;
		
	}
	else
		return 0;
}

/*----------------------Visualisation du graphe------------------------*/
void MDGraph::printCompleteInformation()
{
    /*cout<<endl<<"-------------------------------------------"<<endl;
     
     if (DeadMG)
     cout<<"CONC: THE GRAPH CONTAINS A DEADLOCK STATE "<<endl;
     else    cout<<"CONC: THE GRAPH IS DEADLOCKFREE "<<endl;
     cout<<"-------------------------------------------"<<endl;*/
	
	cout << "\n\nGRAPH SIZE : \n";
	cout<< "\n\tNB MARKING : "<< nbMarking;
	cout<< "\n\t*NB NODES : "<< nbStates;
	cout<<"\n\tNB ARCS : " <<nbArcs<<"*"<<endl;
	//cout<<" \n\nCOMPLETE INFORMATION ?(y/n)\n";
	//char c;
	//cin>>c;
	//InitVisit(initialstate,n);
	Tab=new bdd[(int)nbStates];
	size_t n=1;
	//cout<<"NB BDD NODE : "<<NbBddNode(initialstate,n)<<endl;
	NbBddNode(initialstate,n);
	cout<<"NB BDD NODE : "<<bdd_anodecount(Tab,(int)nbStates)<<endl;
	//cout<<"Shared Nodes : "<<bdd_anodecount(Tab,nbStates)<<endl;
	InitVisit(initialstate,1);
	//int toto;cin>>toto;
	//bdd Union=UnionMetaState(initialstate,1);
	//cout<<"a titre indicatif taille de l'union : "<<bdd_nodecount(Union)<<endl;
	/*if(c=='y'||c=='Y')
     {
     
     size_t n=1;
     printGraph(initialstate,n);
     }*/
	
	
}
/*----------------------InitVisit()------------------------*/
void MDGraph::InitVisit(Class_Of_State * S,size_t nb)
{
    
	if(nb<=nbStates)
	{
		S->Visited=false;
		for(Edges::const_iterator i=S->Successors.begin();!(i==S->Successors.end());i++)
		{
            
            if((*i).first->Visited==true)
            {
                nb++;
                InitVisit((*i).first,nb);
            }
		}
		
	}
}
/*********                  printGraph    *****/

void MDGraph::printGraph(Class_Of_State *s,size_t &nb)
{
	if(nb<=nbStates)
	{
		cout<<"\nSTATE NUMBER "<<nb<<" : \n";
		s->Visited=true;
		printsuccessors(s);
		getchar();
		printpredecessors(s);
		getchar();
		Edges::const_iterator i;
		for(i=s->Successors.begin();!(i==s->Successors.end());i++)
		{ if((*i).first->final()) cout<<"ICI c final"<<endl;
			if((*i).first->Visited==false)
			{
				nb++;
				printGraph((*i).first, nb);
			}
		}
		
	}
    
}


/*---------void print_successors_class(Class_Of_State *)------------*/
void MDGraph::printsuccessors(Class_Of_State *s)
{
	Edges::const_iterator i;
	cout<<bddtable<<s->class_state<<endl;
	//	if(s->boucle)
	//	cout<<"\n\tON BOUCLE DESSUS AVEC EPSILON\n";
    if(s->blocage())
		cout<<"\n\tEXISTENCE D'UN ETAT BLOCANT\n";
	if(s->final())
        cout<<"\n\tEXISTANCE D'UN ETAT FINAL";
    
	cout<<"\n\tSES SUCCESSEURS SONT  ( "<<s->Successors.size()<<" ) :\n\n";
	getchar();
	for(i =s->Successors.begin();!(i==s->Successors.end());i++)
	{
		cout<<" \t- t"<<(*i).second<<" ->";
		cout<<bddtable<<(*i).first->class_state<<endl;
		printsuccessors((*i).first);
		getchar();
	}
}
/*---------void printpredescessors(Class_Of_State *)------------*/
void MDGraph::printpredecessors(Class_Of_State *s)
{
	Edges::const_iterator i;
	cout<<"\n\tSES PREDESCESSEURS SONT  ( "<<s->Predecessors.size()<<" ) :\n\n";
	getchar();
	for(i =s->Predecessors.begin();!(i==s->Predecessors.end());i++)
	{
		cout<<" \t- t"<<(*i).second<<" ->";
		cout<<bddtable<<(*i).first->class_state<<endl;
		getchar();
	}
}

/*----------------------void reachfinal()-----------------*/
void MDGraph::reachfinal()
{   //cout<<endl<<"---------------reachfinal()----------------------------"<<endl;
    
	//InitVisit(initialstate,n);
	Tab=new bdd[(int)nbStates];
	size_t ns=1;
	NbBddNode(initialstate,ns);
    //cout<<"ns= "<<ns<<endl;
	InitVisit(initialstate,1);
    int n=1;
    reachfinalfromstate(initialstate,n);
    //cout<<"n= "<<n<<endl;
    //cout<<"NS= "<<ns<<endl;
    /*if(n<ns) cout<<" WE can NOT reach a final state from one of the Graph's states"<<endl;
     else  cout<<" WE can reach a final state from each Graph's state"<<endl;	*/
}

void MDGraph::reachfinalfromstate(Class_Of_State *s,int& nb)
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
                Edges::const_iterator i;
                for(i=s->Successors.begin();!(i==s->Successors.end());i++)
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

void MDGraph::reachfinalsuccessors(Class_Of_State *s,int & trouve)
{
	Edges::const_iterator i;
	if(s->final()) {
		//  cout<<"\tHELOOOOOOOOOO EXISTANCE D'UN ETAT FINAL"<<endl;
        trouve=1;}
    if(trouve!=1)
        for(i =s->Successors.begin();!(i==s->Successors.end());i++)
        {reachfinalsuccessors ((*i).first,trouve);
            //cout<<"kkkkkkkkkkkk"<<endl;
            
        }}

void MDGraph::checksoundness()
{
    cout<<endl<<"-------------------------------------------"<<endl;
    //if (!DeadMG && stop==0)
    if (!DeadMG && OptionToComp)
        cout<<"CONC: Option to complete is verified ..."<<endl;
 	else    cout<<"CONC: Option to complete is not verified ...  "<<endl;
	if (nodeadTrans)         cout<<"      No dead transitions is verified ... "<<endl;
	else cout<<"      No dead transitions is not verified ..."<<endl;
	if (pc)         cout<<"      proper completion is verified ..."<<endl<<endl;
	else cout<<"      proper completion is not verified ..."<<endl<<endl;
	//if (!DeadMG && stop==0  && pc && nodeadTrans)
    if (!DeadMG && OptionToComp  && pc && nodeadTrans)
        cout<<"      	The model is sound ! "<<endl;
	else cout<<"      	The model is not sound  !"<<endl;
    cout<<"-------------------------------------------"<<endl;
}

void MDGraph::checkweaksoundness()
{
    cout<<endl<<"-------------------------------------------"<<endl;
    // if (easySound)
    if (OptionToComp)

        cout<<"CONC: Weak soundness is verified ..."<<endl;
 	else    cout<<"CONC: Weak soundness is not verified ...  "<<endl;
    cout<<"-------------------------------------------"<<endl;
}
void MDGraph::checkeasysoundness()
{
  
    cout<<endl<<"-------------------------------------------"<<endl;
    if (easySound)
        cout<<"CONC: Easy soundness is verified  ..."<<endl;
 	else    cout<<"CONC: Easy soundness is not verified  ...  "<<endl;
	
    cout<<"-------------------------------------------"<<endl;
}
void MDGraph::checkrelaxedsoundness()
{
        cout<<endl<<"-------------------------------------------"<<endl;
    if (relaxedSound)
        cout<<"CONC: Relaxed soundness is verified  ..."<<endl;
 	else    cout<<"CONC: Relaxed soundness is not verified  ...  "<<endl;
	
    cout<<"-------------------------------------------"<<endl;
}
