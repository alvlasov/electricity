#include <iostream>
#include "electricity.h"

using namespace std;

Node::Node(int _id, float _maxGen,float _genCost,float _consumption)
{
    id=_id;
    maxGen=_maxGen;
    genCost=_genCost;
    consumption=_consumption;
    cout << "Node " << id << " created" << endl;
}

void Node::addEdge(Node *node, float max, float loss)
{
    edges.push_back(Edge(node,max,loss));
}

int Node::getId()
{
    return id;
}

int Node::getEdgesCount()
{
    return edges.size();
}


float Node::getGenCost()
{
    return genCost;
}

float Node::getMaxGen()
{
    return maxGen;
}

float Node::getCons()
{
    return consumption;
}

Edge* Node::getEdge(int i)
{
    return &edges[i];
}
