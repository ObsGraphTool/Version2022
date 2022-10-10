

/******************    Graph.hpp  **************************/

#ifndef _MDGRAPH_

#define _MDGRAPH_
#include <iostream>
#include <vector>
#include"Class_of_state.h"
#include <list>
typedef set<int> Set;
typedef vector<Class_Of_State*> MetaGrapheNodes;
//typedef vector<Set> LamdaSet;
class MDGraph
{
	private:
		
		MetaGrapheNodes GONodes;
	public:

		void Reset();
		void printGraph(Class_Of_State *, size_t &);
		Class_Of_State *initialstate;
		Class_Of_State *currentstate;
		double nbStates;
		double nbMarking;
		double nbArcs;

		//attributs ajoutés pour la soundness
		bool DeadMG;
        bool OptionToComp;
		int stop;
		bool pc;
		bool nodeadTrans;
		bool relaxedSound=true; 
		bool weakSound=true; 
		bool easySound=true; 

		

		Class_Of_State* find(Class_Of_State*);
		Edges& get_successor(Class_Of_State*);
		void printsuccessors(Class_Of_State *);
		int NbBddNode(Class_Of_State*,size_t&);	 
		void InitVisit(Class_Of_State * S,size_t nb);
		void printpredecessors(Class_Of_State *);
		void addArc(){nbArcs++;}
		void insert(Class_Of_State*);
		MDGraph(){nbStates=nbArcs=nbMarking=stop=0;DeadMG=false;}
		void setInitialState(Class_Of_State*);  //Define the initial state of this graph
		void printCompleteInformation();
		//Set computelamda(Class_of_State*,);
		//void computelamda(Class_Of_State*,LamdaSet&);
		//fonctions ajoutées pour soundness
		void reachfinal();
		void checksoundness();
		void checkrelaxedsoundness();

		void checkweaksoundness();
		void checkeasysoundness();
		void reachfinalsuccessors(Class_Of_State *s,int & trouve);
		void reachfinalfromstate(Class_Of_State *s,int &n);







};
#endif
