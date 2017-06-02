#include <vector>
#include <atomic> 
#include <bits/stdc++.h>

using namespace std;

template <typename T>
class node
{
public:
	T dato;
	node * next;
	node();
	~node();
};

class Stage
{
public:
	int visit;
	Stage();
	~Stage();
};

template <typename T>
class token
{
public:
	int id;
	node<T> * v;
	Stage * s;
	int dif;

	token();
	token(int a,node<T> * b,Stage * c,int d)
	{
		id=a;
		v=b;
		s=c;
		dif=d;
	}
	~token();
	
};

/*lock free*/
class Node
{
public:
	int move;
	std::atomic<int> w;
	std::atomic<int> n;
	std::atomic<bool> isparent (0);
	std::atomic<int> untriedmoves (-1);
	std::atomic<bool> isexpandable (false);
	std::atomic<bool> isfullyexpanded (false);
	Node * parent;
	Node * children[]; 

	Node();
	void Init ();
	~Node();
	
};

void Node::Init (int moves[]){
	int nomoves = moves.size();
	if !(isparent.exchange(true)){
		untriedmoves.store(nomoves);
		isexpandable.store(true,memory_order_release);
	}
}

int main()
{
	cout<<"hola"<<endl;
   	return 0;
}