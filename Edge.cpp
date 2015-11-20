#include <iostream>
#include "electricity.h"

using namespace std;

Edge::Edge(Node *_destination, float _maxFlow, float _loss)
{
    destination=_destination;
    maxFlow=_maxFlow;
    loss=_loss;
}

int Edge::getDestination()
{
    return destination->getId();
}

float Edge::getMaxFlow()
{
    return maxFlow;
}

float Edge::getLoss()
{
    return loss;
}
