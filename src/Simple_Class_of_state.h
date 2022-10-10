#ifndef SIMPLE_CLASS_OF_STATE
#define SIMPLE_CLASS_OF_STATE
#include "bdd.h"
#include <map>
#include <set>
#include <utility>
#include<vector>
#include<string>
#include <cstdlib>
#include<string.h>

using namespace std;

typedef set<int> Set;

class Simple_Class_Of_State
{
	public:
		Set firable;
		string nom;
		bool boucle; 
		set<string> lamda;
	
		set<pair<string,Simple_Class_Of_State*> > Successors;		
		Simple_Class_Of_State(){boucle=0;}
		Simple_Class_Of_State(string n,set<string> l){nom=n; lamda=l;}
		Simple_Class_Of_State(string n,string l){	nom=n;
								while(l.find("{")!=-1)	
									{ 	lamda.insert(l.substr(l.find("{")+1,(l.find("}")+1)-(l.find("{")+2)));
										l=l.substr(l.find("}")+1);
									}
									//lamda.insert(l);
							}
	

		
	
};
typedef pair<Simple_Class_Of_State*, string> Simple_Edge;
typedef vector<Simple_Edge> Simple_Edges;
#endif
