

/******************    Graph.hpp  **************************/

#ifndef _Modular_ObsGraph_

#define _Modular_ObsGraph_
#include<iostream>
#include<time.h>
#include<vector>
#include"Modular_Class_of_state.h"
typedef set<int> Set;
typedef vector<Modular_Class_Of_State*> Modular_Obs_Graph_Nodes;
class Modular_Obs_Graph
{
	private:
		
		Modular_Obs_Graph_Nodes GONodes;
	public:
		void Reset();
		void printGraph(Modular_Class_Of_State *, size_t &);
		Modular_Class_Of_State *initialstate;
		Modular_Class_Of_State *currentstate;
		size_t nbStates;
		size_t nbMarking;
		size_t nbArcs;

        //attributs ajoutés pour la soundness
		bool DeadMG;
		int stop;
		bool pc;
		bool nodeadTrans; 

		void Liberer(Modular_Class_Of_State *S);
		Modular_Class_Of_State* find(Modular_Class_Of_State*);
		Modular_Class_Of_State* find2(Modular_Class_Of_State*,Set);
		Modular_Edges& get_successor(Modular_Class_Of_State*);
		void printsuccessors(Modular_Class_Of_State *);
		void InitVisit(Modular_Class_Of_State * S,size_t nb);
		void TAB_BDDNODES(Modular_Class_Of_State *,size_t&);
		void printpredecessors(Modular_Class_Of_State *);
		void addArc(){nbArcs++;}
		void addArc(Modular_Class_Of_State*,Modular_Class_Of_State*,const char*);
		void insert(Modular_Class_Of_State*);
		Modular_Obs_Graph(){nbStates=nbArcs=nbMarking=stop=0;DeadMG=false;}
		void setInitialState(Modular_Class_Of_State*);  //Define the initial state of this graph
		void printCompleteInformation(int nbsubnets);
		set<Sets> computelamdaSyncro(set<Sets>* s,int nb,set<string> * tempObs,int aff);

		set<string> computeEt(set<string> *Et, set<string> *O,int n);
			//set<string> computeEt();
		//set<string> computelamdaSyncro(set<string> s[2],int nb,int aff);
		void construct_Modular_Obs_Graph(set<const char*> l, int nb);

//fonctions ajoutées pour soundness
		void reachfinal(int nbsubnets);
		void checksoundness();
        void checkweaksoundness();
        void checkeasysoundness();
		void reachfinalsuccessors(Modular_Class_Of_State *s,int & trouve);
		void reachfinalfromstate(Modular_Class_Of_State *s,int &n);


};
#endif
