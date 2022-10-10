#ifndef CLASS_OF_STATE
#define CLASS_OF_STATE
#include "bdd.h"
#include <map>
#include <set>
#include <utility>
#include<vector>
//#include <backward/vector.h>

//#include <ext/hash_map>
using namespace std;
typedef set<string> Sets;

typedef set<int> Set;

class Class_Of_State
{
	public:
  Class_Of_State(){/*boucle=blocage=*/Visited=0;Mf=bddfalse;}
		Set firable;
		bdd class_state;
		//set<bdd> Mf;
		set<int> Enable;
		bool finalreached;
		//bool boucle; 
		bool blocage(){for(set<Sets>::const_iterator it=lamda.begin();it!=lamda.end();it++)
		    {if((*it).find("EV")!=(*it).end())
			return true;
		      else
			return false;
		    }}
		bool Visited;
		//bool final;
		bool final(){for(set<Sets>::const_iterator it=lamda.begin();it!=lamda.end();it++)
		    {if((*it).find("term")!=(*it).end())
			return true;
		      else
			return false;
		    }}

		set<Sets> lamda;
		bdd Mf=bddfalse;
		Set T_f;

		void * Class_Appartenance;
		vector<pair<Class_Of_State*,int> > Predecessors, Successors;
		pair<Class_Of_State*,int>  LastEdge;
};
typedef pair<Class_Of_State*, int> Edge;
typedef vector<Edge> Edges;
#endif
