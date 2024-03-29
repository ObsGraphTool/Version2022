#ifndef MODULAR_CLASS_OF_STATE
#define MODULAR_CLASS_OF_STATE
//#include"Modular_Edge.h"
//#include<hash_map.h>
#include"Net.h"
#include <set>
//#include <vector>
#include"bdd.h"
typedef set<string> Sets;

//_____________________   Vecteur d'entier : repr�sente les id des bdd relatifs � un �tat synchronis� de n GO _________________________
//typedef vector<bdd> SynchronizedState;
//struct less_SynchronizedState
//{
//bool operator()(const SynchronizedState,const SynchronizedState)const;
//};
//inline bool less_SynchronizedState::operator ()(const SynchronizedState p1,const SynchronizedState p2)const
//{
	
//for(int i=0;i<p1.size();i++)
//	if(p1[i].id()>p2[i].id())
//		return false;
//return true;
	
//}
//typedef set<SynchronizedState, less_SynchronizedState> Gen_Prod_Synch;
class Modular_Class_Of_State
{
  //friend struct less_modular_class_of_state;
	public:
		Modular_Class_Of_State(){boucle=blocage=Visited=final=0;}
		vector <bdd> State;
		bool boucle; 
		bool blocage;
		bool Visited;
		bool final;
		bool finalreachable=false;
		set<Sets> lamda;
		set<string> Et;
		set<pair<Modular_Class_Of_State*, string> > Predecessors;
		set<pair<Modular_Class_Of_State*, string> > Successors;
		friend ostream& operator<<(ostream &os,const Modular_Class_Of_State &c){
		os<<"{";
		for(unsigned int k=0;k<c.State.size();k++)
			os<<c.State[k].id()<<", ";
		os<<"}";
		return(os);}
};
/*____________________ MODULAR EDGE________________*/
class Modular_Class_Of_State;
typedef pair<Modular_Class_Of_State*, string> Modular_Edge;
struct LessModularEdge
{
	bool operator()(const Modular_Edge,const Modular_Edge)const;
};
inline bool LessModularEdge::operator ()(const Modular_Edge a1,const Modular_Edge a2)const
{
	//cout<<"on compare les etiquettes des arcs : "<<a1.second<<" avec "<<a2.second<<endl;
	return (a1.first<a2.first);
	
}
typedef set<Modular_Edge,LessModularEdge> Modular_Edges;
#endif
