#ifndef ELECTRICITY_H_INCLUDED
#define ELECTRICITY_H_INCLUDED

#include <vector>
#include <iomanip>
using namespace std;

class Node;
class Edge;

class PowerNetwork {
private:
    vector<Node> nodes;
    void listEdges(Node*);
public:
    PowerNetwork();
    void addNode(float maxGen,float genCost,float cons);
    void addEdge(int n1, int n2, float max, float loss);
    void showMap();
    int count(); //число узлов
    void optimize();
};

class Node
{
private:
    vector<Edge> edges;
    int id;
    float production;
    float maxGen;
    float genCost;
    float consumption;
public:
    Node(int id, float maxGen,float genCost,float cons);
    void addEdge(Node *node, float max, float loss);
    int getId();
    float getGenCost();
    float getMaxGen();
    float getCons();

    int getEdgesCount();
    Edge* getEdge(int edge);
};

class Edge
{
private:
    float maxFlow;
    float loss;
    Node *destination;
public:
    Edge(Node *, float maxFlow, float loss);
    int getDestination();
    float getMaxFlow();
    float getLoss();
};

#define  MMAX  100
#define  NMAX  100
#define REAL double
typedef REAL MAT[MMAX][NMAX];

class Simplex
{
private:

    MAT  A;
    int  IPOSV[MMAX], IZROV[NMAX];
    int  i,j,ICASE,N,M,M1,M2,M3;
    REAL R;

    void simp1(MAT,int,int *,int,int,int *,REAL *);
    void simp2(MAT,int,int,int *,int,int *,int,REAL *);
    void simp3(MAT,int,int,int,int);
    void simplx(MAT,int,int,int,int,int,int *,int *, int *) ;
public:
    Simplex(REAL **tab, int _N, int _M1, int _M2, int _M3);
    int calc(REAL *);
};

#endif // ELECTRICITY_H_INCLUDED
