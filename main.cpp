// 4.12.15
#include <iostream>
#include "electricity.h"

using namespace std;

int main()
{
    PowerNetwork p;

    p.addNode(0,0,90); //float maxGen, genCost, cons
    p.addNode(130,200,50);
    p.addNode(100,300,50);
    p.addNode(100,100,30);

    p.addEdge(0,1,50,0.02); //int node1, node2; float max, loss
    p.addEdge(0,2,70,0.04);
    p.addEdge(1,2,100,0.01);
    p.addEdge(2,3,100,0.03);

    cout << endl;
    p.showMap();
    p.optimize();
    return 0;
}
