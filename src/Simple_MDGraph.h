

/******************    Graph.hpp  **************************/

#ifndef _SIMPLE_MDGRAPH_

#define _SIMPLE_MDGRAPH_
#include <iostream>
#include <vector>
#include"Simple_Class_of_state.h"
#include <list>
#include<time.h>

//typedef set<int> Set;
typedef vector<Simple_Class_Of_State*> Simple_MDGraph_Nodes;
class Simple_MDGraph
{
	private:
		
		Simple_MDGraph_Nodes GONodes;
	public:
		double nbStates;
		double nbArcs;
		Simple_MDGraph(const char* s);
		void insert(Simple_Class_Of_State*);
		Simple_Class_Of_State* find(string nom);
		Simple_MDGraph(){nbStates=nbArcs=0;}
		Simple_MDGraph computeResGraph(Simple_MDGraph);
		void saveonfile(string f);
Simple_MDGraph_Nodes::const_iterator find2(Simple_Class_Of_State * c);
set<Simple_Class_Of_State*> get_successor2(Simple_Class_Of_State *SC,set<string> o);

		set<string> computelamdaSyncro(set<string> s[2],int nb);
		Simple_Class_Of_State* get_successor(Simple_Class_Of_State*,string);



};
#endif
