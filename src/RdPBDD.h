/* -*- C++ -*- */
#ifndef RdPBDD_H
#define RdPBDD_H
#include <vector>
#include <stack>
#include "Net.h"
#include "bdd.h"
#include"MDGraph.h"
#include"Modular_Obs_Graph.h"
#include "Modular_Class_of_state.h"
//#include"Modular_Obs_Graph.hpp"
 
/***********************************************/
class Trans {
public:
	Trans(const bdd& var, bddPair* pair, const bdd& rel,const bdd& prerel,const bdd& Precond, const bdd& Postcond);
	bdd operator()(const bdd& op) const;
	bdd operator[](const bdd& op) const;
  friend class RdPBDD;
private:
	bdd var;
	bddPair* pair;
        bdd Precond;
        bdd Postcond;
	bdd rel;
        bdd prerel;
};

class RdPBDD {
private :  
  vector<class Transition> transitions;
  Set Observable;
  Set NonObservable;
  map<string,int> transitionName;
  Set InterfaceTrans;
  Set Formula_Trans;
Set Final_State;
set<Set> Final_States;

  unsigned int Nb_places;
public:
  bdd M0;
  bdd finalstate;
  bdd currentvar;
  vector<Trans> relation;
  bdd ReachableBDD1();
  bdd ReachableBDD2();
  bdd ReachableBDD3();
bool DeadRdp;
/* Génération de graphes d'observation */
void compute_canonical_deterministic_graph_Opt(MDGraph& g) ;
void compute_canonical_deterministic_graph_OptProf(MDGraph& g) ;
//void  ParcoursProf(MDGraph& g, pile& st);

  bdd Accessible_epsilon(bdd From);
    bdd Accessible_epsilon(bdd From,Class_Of_State* c);

  bdd Accessible_epsilon2(bdd From);
  bdd StepForward(bdd From);
  bdd StepForward2(bdd From);
 bdd StepBackward(bdd From);
  bdd StepBackward2(bdd From);
    bdd StepBackward3(bdd From);
  bdd StepBackward3(bdd From,bdd s);

    bdd StepBackward(bdd From,int t,bdd c);
    bdd StepBackward2(bdd From,bdd s);
    bdd StepBackward(bdd From,bdd s);

  bdd EmersonLey(bdd S, bool trace);
  Set firable_obs(bdd State);
  set<string> firable_Nonobs(bdd State);
  Set firable_trans(bdd State);
  bdd get_successor(bdd From,int t);
  void GeneralizedSynchProduct1(Modular_Obs_Graph& Gv, int NbSubnets,RdPBDD* Subnets[] ,int nbbddvar,int stopvolee) ;
  void GeneralizedSynchProduct_Prof(Modular_Obs_Graph& Gv, int NbSubnets,RdPBDD* Subnets[] ,int nbbddvar,int stopvolee) ;

  bool Set_Div(bdd &S) const;
  bool Set_Bloc(bdd &S) const;
bool Set_bouclebloc(bdd &s) ;

  bool Set_Final(bdd &S) const;
  bdd FrontiereNodes(bdd From) const ;
bdd FrontiereNodes2(bdd From,int i) const ;
  bdd CanonizeR(bdd s, unsigned int i) ;
 //set<string> computelamda(bdd s,bool bloc,int aff);
    set<Sets> computelamda(bdd s,int aff);

set<string> computelamdaSyncro(set<string> s[2],int nb);
 //Fonctions utiles pour la soundness
string strictGrand(bdd A,bdd mf);
  string strictGrandBDD(bdd A,bdd mf);
  //void UpdateMf( Class_Of_State* c, Class_Of_State* a,int t);
  bdd SatBack( bdd S, bdd s );

  bdd Succ(bdd From, int t);
  Set ComputeTf(bdd Mf);



  RdPBDD(const net&,int BOUND=32,bool init=false);
  ~RdPBDD(){};
};
/*____________Structure utiles aux produit synchronisé généralisé de n graphes d'observations ________*/

/*typedef pair<Modular_Class_Of_State*,bdd*> CoupleNodes; // Méta-état (canonisé) + vecteur des bdds complets
typedef pair<Set*,Set*> CoupleSets;//ens d'ens des trans de la formule franchissables non encore traitées + ens d'ens des trans de l'interface franchissables non encore traitées
typedef pair<CoupleNodes,CoupleSets> PairProduct; // Couple + ens des ens des transitions franchissables non encore traitées
typedef pair<PairProduct,bool> StackElt;
typedef vector<StackElt> Stack;*/
typedef pair<Modular_Class_Of_State*,bdd*> Couple;
typedef pair<Couple,Set*> StackElt; 
typedef stack<StackElt> Stack;
typedef vector<StackElt> PileInt;
typedef pair<StackElt,PileInt> CanonicRep;
//typedef stack<CanonicRep> STACK;
typedef stack<PileInt> STACK;
int InStack(PileInt,Modular_Class_Of_State*);
bool MemeVecteur(vector<bdd>,vector<bdd>);
bool Inferieur(vector<bdd>,vector<bdd>);

#endif
