/* -*- C++ -*- */
#ifndef NET_H
#define NET_H
#include <string>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <set>
#include <map>
#include <ext/hash_map>
#include "RdPMonteur.h"
#include "string.h"
typedef set<int> Set;
class Node{
private: 
  int id;
public:
  Node(int id){this->id=id;};
  ~Node(){};
  vector< pair<int,int> > pre, post, inhibitor, preAuto, postAuto;
  vector<int> reset;
  void addPre(int,int);
  void addPost(int,int);
  void addInhibitor(int,int);
  void addPreAuto(int,int);
  void addPostAuto(int,int);
  void addReset(int);
  int getid(){return(id);};
};

class Place:public Node{
public:
  string name;
  int marking, capacity;
  Place(const int id,const string &p,int m=0, int c=0):Node(id),name(p),marking(m),capacity(c){};
  ~Place(){};
  bool isLossQueue() const {return marking==-2;}
  bool isQueue() const {return marking<=-1;}
  bool hasCapacity() const {return capacity!=0;}
};

class Transition:public Node{
public:
  string name;
  Transition(const int id,const string &t):Node(id),name(t){};
  ~Transition(){};
};
/*-----------------------------------------------------------------*/
struct ltstr
{
	bool operator()(const char* s1, const char* s2) const
	{
		return strcmp(s1,s2)<0;
	}
};
typedef set<const char*,ltstr> Set_mot;
typedef vector<Place> PLACES;
typedef vector<Transition> TRANSITIONS;
class net: public RdPMonteur {
private:
 /*Initialisation des attributs*/
  bool Set_Observables(const char* file) ;
  bool Set_Interface_Trans(const char* file) ;
  bool Set_Formula_Trans(const char* file) ;
bool Set_final_State(const char * file);
  bool Set_ObsNonObservables(Set_mot obs) ;
  void Set_Non_Observables();
public:
  /* Attributs */ 
  vector<class Place> places;
  vector<class Transition> transitions;
  map<string,int> placeName;
  map<string,int> transitionName;
  Set Observable;
  Set NonObservable;
  Set InterfaceTrans;
  Set Formula_Trans;
  //Set Final_State;
  vector<map<int,int> > Final_Markings;

  //set<Set> Final_States;
  
  /* Constructors */  
  net(){};
  ~net(){};
  net(const char *file,const char*Obs="", const char* Int="",const char *Place="");
  /* Monteur */
  bool addPlace(const string &place,const int &id,int marking=0, int capacity=0);
  bool addQueue(const string &place,const int &id,int capacity=0);
  bool addLossQueue(const string &place,const int &id,int capacity=0);
  bool addTrans(const string &transition,const int &id,const int obs=0);
  bool addPre(const int &place,const int &transition,int valuation=1);
  bool addPost(const int &place,const int &transition,int valuation=1);  
  bool addPreQueue(const string &place,const string &transition,int valuation=1);
  bool addPostQueue(const string &place,const string &transition,int valuation=1);  
  bool addInhibitor(const string &place,const string &transition,int valuation=1);
  bool addPreAuto(const string &place,const string &transition,const string &valuation);
  bool addPostAuto(const string &place,const string &transition,const string &valuation);  
  bool addReset(const string &place,const string &transition);
 void addFinalMarking(map<int,int> *&);
  /* Visualisation */
  int nbPlace() const {return places.size();};
  int nbTransition() const {return transitions.size();};
bool finalS(const char * f);

  int findPlaceByid(const int id)
  {
    int i=0; 
    while((i< places.size())&&(id!=places[i].getid()))
	  i++;
     if(i==places.size())
       return -1;
      return i;
};
 int findTransitionByid(const int id)
  {
    int i=0; 
    while((i<transitions.size())&&(id!=transitions[i].getid()))
	  i++;
    if(i==transitions.size())
      return -1;
      return i;

};
};

ostream& operator<<(ostream &,const net &);
#endif
