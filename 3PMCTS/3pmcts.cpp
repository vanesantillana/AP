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
  atomic<int> w;
  atomic<bool> isparent;
  atomic<int> untriedmoves;
  atomic<bool> isexpandable;
  atomic<bool> isfullyexpanded;
  Node * parent;
  Node * children[]; 
  int a=2;
  Node(){
    w=0;
    atomic<int> n(0);
  };
  void Init (int[]);
  ~Node();
	
};

void Node::Init (int moves[]){
  int nomoves = 2;
  if (!isparent.exchange(true)){
      untriedmoves.store(nomoves);
      isexpandable.store(true,memory_order_release);
    }
}

int main()
{
  cout<<"hola"<<endl;
  return 0;
}
