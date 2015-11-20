#include <iostream>
#include "electricity.h"

using namespace std;

PowerNetwork::PowerNetwork()
{
}

void PowerNetwork::addNode(float maxGen,float genCost,float cons)
{
    nodes.push_back(Node(count(),maxGen,genCost,cons));
}

void PowerNetwork::addEdge(int n1, int n2, float max, float loss)
{
    cout << "Edge from " << n1 << " to "<< n2 << " created" << endl;;
    nodes[n1].addEdge(&nodes[n2],max,loss);
    nodes[n2].addEdge(&nodes[n1],max,loss);
}
void PowerNetwork::showMap()
{
    cout << "Map:" << endl;
    for (int i=0; i<count(); i++)
    {
        cout << " Node " << nodes[i].getId();
        if (nodes[i].getEdgesCount()!=0)
        {
            cout << " connected with ";
            listEdges(&nodes[i]);
        }
        cout << endl;
    }
}
void PowerNetwork::listEdges(Node *node)
{
    Edge *edge;
    for (int i=0; i<node->getEdgesCount(); i++)
    {
        if (i!=0)
            cout << ", ";
        edge=node->getEdge(i);
        cout << edge->getDestination();
    }
}

int PowerNetwork::count()
{
    return nodes.size();
}

void PowerNetwork::optimize()
{
    //gen tableau
    int i,j,N,M,k,l;
    k=count(); //nodes count

    N=k*(k+1)+1; //строк
    M=k*k+1; //столбцов

    //create tab
    double **tab = new double* [N];
    for (i=0; i<N; i++)
        tab[i] = new double [M];
    for (i=0; i<N; i++) //init tab
        for (j=0; j<M; j++)
            tab[i][j]=0;

    //economic function
    int line=0;
    for (i=0; i<k; i++)
        tab[line][i+1]=-nodes[i].getGenCost();
    line++;

    //max gen
    for (i=0; i<k; i++)
    {
        tab[line][0]=nodes[i].getMaxGen();
        tab[line][1+i]=-1;
        line++;
    }

    //edges limit
    Edge *edge;
    int dest,edgesCount;
    double **flowsMax = new double* [k];
    double **flowsLoss = new double* [k];
    for (i=0; i<k; i++)
    {
        flowsMax[i] = new double [k];
        flowsLoss[i] = new double [k];
    }
    for (i=0; i<k; i++)
        for (j=0; j<k; j++)
        {
            flowsMax[i][j]=0;
            flowsLoss[i][j]=0;
        }

    //создание матрицы потоков
    for (int src=0; src<k; src++)
    {
        edgesCount=nodes[src].getEdgesCount();
        for (j=0;j<edgesCount;j++)
        {
            edge=nodes[src].getEdge(j);
            dest=edge->getDestination();
            flowsMax[src][dest]=edge->getMaxFlow();
            flowsLoss[src][dest]=edge->getLoss();
        }
    }
    int pos=k+1;
    for (i=0; i<k; i++)
        for (j=i+1;j<k;j++)
        {
                tab[line][0]=flowsMax[i][j];
                tab[line][pos++]=-1;
                line++;
                tab[line][0]=flowsMax[i][j];
                tab[line][pos++]=-1;
                line++;
        }

    //законы кирхгофа для узлов
    for (i=0; i<k; i++)
    {
        tab[line][0]=nodes[i].getCons();
        tab[line][i+1]=-1;

        pos=k+1;
        for (j=0;j<k;j++)
            for (l=j+1;l<k;l++)
            {
                if (i==j || i==l)
                {
                    if (i==l)
                    {
                        tab[line][pos++]=-(1-flowsLoss[j][l]);
                        tab[line][pos++]=1;
                    }
                    else
                    {
                        tab[line][pos++]=1;
                        tab[line][pos++]=-(1-flowsLoss[j][l]);
                    }
                }
                else
                {
                    tab[line][pos++]=0;
                    tab[line][pos++]=0;
                }
            }
        line++;
    }


    cout << "Simplex tab" << endl;
    for (i=0; i<N; i++)
    {
        for (j=0; j<M; j++)
            cout << setw(6) << tab[i][j]<<" ";
        cout << endl;
    }
/*
    cout << "Flows map" << endl;
    for (i=0; i<k; i++)
    {
        for (j=0; j<k; j++)
            cout << setw(6) << flowsMax[i][j]<<" ";
        cout << endl;
    }*/
    //simplex
    int M1, M2, M3;
    M1 = k*k; // M1 <=
    M2 = 0; // M2 >=
    M3 = k; //M3  =

    double *results = new double [M];

    Simplex s(tab, M-1, M1, M2, M3);
    int err=s.calc(results);

    if (err==0)
    {
        //вывод
        cout << "Minimum cost = " << -results[0] << endl;

        for (i=1; i<=k; i++)
        {
            cout << "Gen " << i-1 << " = " << results[i] << endl;
        }

        double **title = new double* [k*(k-1)];
            for (i=0; i<k*(k-1); i++)
                title[i] = new double [2];

        pos=0;
        for (i=0; i<k; i++)
            for (j=i+1;j<k;j++)
            {
                title[pos][0]=i;
                title[pos][1]=j;
                pos++;
            }

        for (i=k+1, j=0; i<M-1; i+=2, j++)
        {
            cout << "Flow " << title[j][0] << "->" << title[j][1] << " = " << results[i] - results[i+1] << endl;
        }

        for (i=0; i<k*(k-1); i++)
            delete [] title[i];
    }

    for (i=0; i<N; i++)
        delete [] tab[i];
    for (i=0; i<k; i++)
        delete [] flowsMax[i];
    for (i=0; i<k; i++)
        delete [] flowsLoss[i];

    delete [] results;
    return;
}

