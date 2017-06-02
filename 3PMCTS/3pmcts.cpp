#include <vector>
#include <atomic> 
#include <math.h>
#include <bits/stdc++.h>

using namespace std;

int getsize(int arr[])
{
	return 5;
}

/*lock free*/
class Node
{
public:
  int move;
  atomic<int> w;
  atomic<int> n;
  atomic<bool> isparent;
  atomic<int> untriedmoves;
  atomic<bool> isexpandable;
  atomic<bool> isfullyexpanded;
  Node * parent;
  Node * children[]; 
  
  Node(int []);
  Node * AddChild();
  bool IsFullyExpanded();
  float UCT(int);
  void Update(int);

};
Node::Node(int moves[]){
    /*Inicializando variables*/
    isparent=false;
    untriedmoves=-1;
    isexpandable=false;
    isfullyexpanded=false;
	/*Fin de la incializacion*/

    int nomoves = getsize(moves);
	if (!isparent.exchange(true)){
		children[nomoves];
		untriedmoves.store(nomoves);
		isexpandable.store(true,memory_order_release);
		}

	}
Node * Node::AddChild(){
	int index;
	if(isexpandable.load(memory_order_acquire)){
		if((index=untriedmoves.fetch_sub(1))==0){
			isfullyexpanded.store(true);
		}

		if(index<0)
			return this;
		else
			return children[index];
	}
}

bool Node::IsFullyExpanded(){
	return isfullyexpanded.load();
}
float Node::UCT(int cp){
	atomic<int> wp,np,npp;
	wp = w.load(memory_order_seq_cst);
	np = n.load(memory_order_seq_cst);
	npp = parent->n.load(memory_order_seq_cst);
	double ln=log((double)npp);
	return (wp/np)+(cp*sqrt(ln/np));
}
void Node::Update(int dif){
	w.fetch_add(dif,memory_order_seq_cst);
	n.fetch_add(1,memory_order_seq_cst);
}

/*Token*/
class Stage
{
public:
  int visit;
  Stage();
};

class Token
{
public:
  int id;
  Node * v;
  Stage * s;
  int dif;

  Token();
  Token(int a,Node<T> * b,Stage * c,int d)
  {
    id=a;v=b;s=c;dif=d;
  }	
};

/*MCTS algorithm*/
class MCTS
{
public:
	Token Select(Token);
	Token Expand(Token);
	Token RandomSimulation(Token);
	Token Evaluation(Token);
	void Backup(Token);

};

Token MCTS::Select(Token){
	int cp=5;
	while(t.v->IsFullyExpanded()){
		t.v = argmax(vp.UCT(cp));
		t.s->SetMove(t.v->move);
	}
	return t;
}
Token MCTS::Expand(Token){
	int moves[];
	Node * vp(moves);
	if(!ts->isTerminal()){
		moves = t.s->untriedmoves();
		t.v= new Node(moves);
		vp=t.v->AddChild();
		if(t.v!=vp){
			t.v=vp;
			t.s->SetMove(vp->move);
		}
	}
	return t;
}

Token MCTS::RandomSimulation(Token t){
	int moves[];
	moves=t.s->untriedmoves();
	while(!t.s->isTerminal()){
		t.s->SetMove(move);
	}
	return t;
}

Token MCTS::Evaluation(Token t){
	t.dif=t.s->Evalute();
	return t;
}
void MCTS::Backup(Token t){
	while(t.v!=NULL){
		t.v->Update(t.dif);
		t.v=t.v->parent;
	}
}

/*UCTSearch*/
void UCTSearch(int s0[]){
	Node * v0(s0);
	Token t0;
	t0.s=s0;
	t0.v=v0;
	MCTS m;
	while(v0!=NULL){
		tl=m.Select(t0);
		tl=m.Expand(tl);
		tl=m.Playout(tl);
		m.Backup(tl);
	}
}

int main()
{
	int a[5]={2,3,4,5,6};
	Node minode(a);
	Node * moves;
	
  	cout<<"hola"<<endl;
  	return 0;
}
