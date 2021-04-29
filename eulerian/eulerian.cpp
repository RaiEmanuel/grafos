#include <iostream>
#include <cmath>
#include <vector>
#include <set>
#include <tuple> // std::tuple, std::make_tuple, std::tie
#include <algorithm>
#include <fstream>

using namespace std;

struct Node
{
    char id;
    bool isCircuit = false; //marca se o no ja esta no circuito
    Node(char idNode) : id(idNode){};
    Node(){};
};

ostream &operator<<(ostream &os, const Node &node)
{
    os << boolalpha;
    os << "[id = " << node.id << "]";
    os << "[isCircuit = " << node.isCircuit << "]";
    //<< "[visited = " << node.visited << "]";
    return os;
}

bool operator<(const Node &n1, const Node &n2)
{
    return std::tie(n1.id) < std::tie(n2.id);
}

struct Edge
{
    int weight;
    //bidirecionada
    Node *fromNode;
    Node *targetNode;
    Edge(Node *n1, Node *n2, int w) : fromNode(n1), targetNode(n2), weight(w) {}
};

ostream &operator<<(ostream &os, const Edge &edge)
{

    os << edge.fromNode->id << " --- " << edge.weight << " --- " << edge.targetNode->id;
    return os;
}

bool compareByWeight(const Edge *e1, const Edge *e2)
{
    return e1->weight < e2->weight;
}

Node *getPointerNode(const char idNode, set<Node *> &nodesCircuit, vector<Node *> &nodesNotCircuit)
{
    //procura na lsita dos nos no circuito
    for (auto x = nodesCircuit.begin(); x != nodesCircuit.end(); ++x)
    {
        if ((*x)->id == idNode)
            return *x;
    }
    //procura na lsita dos nos no fora do circuito
    for (auto x = nodesNotCircuit.begin(); x != nodesNotCircuit.end(); ++x)
    {
        if ((*x)->id == idNode)
            return *x;
    }
    return nullptr;
}

Edge *lookupNodeinEdge(Node *node, Edge *edge)
{

    if (edge->fromNode == node || edge->targetNode == node)
        return edge;

    return nullptr;
}

int main(int argc, char const *argv[])
{
    if(argc == 1){
        cout << "Especifique o nome do arquivo .txt";
        return EXIT_FAILURE;
    }
    ifstream file;
    string fileName = argv[1];
    file.open(fileName);

    set<Node *> nodesCircuit;
    vector<Node *> nodesNotCircuit;
    set<Edge *> edgesCircuit;
    set<Edge *> edgesNotCircuit;
    unsigned short int nNodes, nEdges;
    cout << "Digite a quantidade de nos: ";
    file >> nNodes;

    for (unsigned short int i = 0; i < nNodes; ++i)
    {
        Node *n = new Node('A' + i);
        nodesNotCircuit.push_back(n); //deletar no fim
        //[0] - 'A'
        //[1] - 'B'
        //[2] - 'C'
        //[3] - 'D' ...
    }
    file >> nEdges;
    for (unsigned short int i = 0; i < nEdges; ++i)
    {
        char nodeA, nodeB;
        int weight = 1;
        file >> nodeA;
        Node *pNodeA = getPointerNode(nodeA, nodesCircuit, nodesNotCircuit);
        file >> nodeB;
        Node *pNodeB = getPointerNode(nodeB, nodesCircuit, nodesNotCircuit);
        file >> weight;
        if (pNodeA == nullptr || pNodeB == nullptr)
        {
            cout << "No nao encontrado..." << endl;
            --i;
        }
        else
        {
            Edge *e = new Edge(pNodeA, pNodeB, weight); //deletar no fim
            edgesNotCircuit.insert(e);
            //cout << "++++++++++++++++++++ " << lookupNodeinEdge(getPointerNode('C',nodesCircuit, nodesNotCircuit), e) << endl;
            cout << "Aresta criada com sucesso " << *e << endl;
        }
        cout << "-------------------------------" << endl;
    }
    //comeca a analise
    //====================================================
    Node *initNode;
    bool isCompletedCicle = false;
    for (auto pNodeInit = nodesNotCircuit.begin(); pNodeInit != nodesNotCircuit.end();)
    {
        initNode = *pNodeInit;
        //cout << "procurando mais um ciclo a partir de " << *initNode << endl;
        //initNode = *pNodeInit; //tenta proximo no do circuito a achar ligacao com uma aresta ainda nao visitada
        Node *adjacentNode = initNode; //no adjacent para iterar formando circuito. mesmo que no w do circuito
        //procura uma aresta com um no que nao esteja na lista dos que estao no circuito
        for (auto pEdgeCurrent = edgesNotCircuit.begin(); pEdgeCurrent != edgesNotCircuit.end();)
        {
           // cout << "no avaliado no momento " << *adjacentNode << endl;
            Edge *searchedEdge = lookupNodeinEdge(adjacentNode, *pEdgeCurrent); //verifica se o no inicial esta em alguma aresta nao pertencente ao circuito
            //cout << "achou aresta? " << searchedEdge << endl;
            if (searchedEdge != nullptr)
            {
               // cout << "find " << *searchedEdge << endl;
                edgesCircuit.insert(searchedEdge); //inserre a aresta no circuito
                nodesCircuit.insert(adjacentNode); //adiciona o no nos ja pertencentes ao circuito
                if (searchedEdge->fromNode != adjacentNode)
                {
                    adjacentNode = searchedEdge->fromNode; //recebe outra extremidade da aresta. esse no vai procurar uma aresta nova para anexar
                }
                else if (searchedEdge->targetNode != adjacentNode)
                {
                    adjacentNode = searchedEdge->targetNode; //recebe outra extremidade da aresta. esse no vai procurar uma aresta nova para anexar
                }
                edgesNotCircuit.erase(searchedEdge);    //deleta aresta das que estao fora do circuito
                pEdgeCurrent = edgesNotCircuit.begin(); //volta para a primeira aresta
                isCompletedCicle = false;               //se achou aresta entao teve mais uma execucao em um ciclo
                //verifica se proximo no e o inicial
                if (initNode == adjacentNode)
                {
                    cout << "==================== Fechou mais um ciclo ==========================" << endl;
                    pNodeInit = nodesNotCircuit.begin(); //comeca novamente a analsiar do primeiro no do circuito
                    isCompletedCicle = true;
                    for(auto p = edgesCircuit.begin(); p != edgesCircuit.end(); ++p){
                        cout << **p << endl;
                    }
                }
            }
            else
                ++pEdgeCurrent; //avalia proxima aresta
        }
        //acabou todas arestas para o no x
        //cout << "Nao achou nenhum aresta nao visitada com " << *adjacentNode << ", vamos tentar o proximo no" << endl
             //<< endl;
        ++pNodeInit;
        //zerou arestas nao analisadas
        if (edgesNotCircuit.size() == 0)
        {
            //cout << "Nao ha mais arestas para analisar" << endl;
            pNodeInit = nodesNotCircuit.end(); //para fechar o for dos nos do circuito
        }
    }

    //-----------------------------------------------------------------------
    //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
    //reinicia processo
    //iniciando novamente, mas agora com um vertice do circuito

    //isCompletedCicle = false;
    //precisa ter arestas nao adicionadas no ciruito para poder avaliar
    for (auto pNodeInit = nodesCircuit.begin(); pNodeInit != nodesCircuit.end() && edgesNotCircuit.size() != 0;)
    {
        initNode = *pNodeInit;
        //cout << "procurando mais um ciclo a partir de " << *initNode << endl;
        //initNode = *pNodeInit; //tenta proximo no do circuito a achar ligacao com uma aresta ainda nao visitada
        Node *adjacentNode = initNode; //no adjacent para iterar formando circuito. mesmo que no w do circuito
        //procura uma aresta com um no que nao esteja na lista dos que estao no circuito
        for (auto pEdgeCurrent = edgesNotCircuit.begin(); pEdgeCurrent != edgesNotCircuit.end();)
        {
            //cout << "no avaliado no momento " << *adjacentNode << endl;
            Edge *searchedEdge = lookupNodeinEdge(adjacentNode, *pEdgeCurrent); //verifica se o no inicial esta em alguma aresta nao pertencente ao circuito
            //cout << "achou aresta? " << searchedEdge << endl;
            if (searchedEdge != nullptr)
            {
                //cout << "find " << *searchedEdge << endl;
                edgesCircuit.insert(searchedEdge); //inserre a aresta no circuito
                nodesCircuit.insert(adjacentNode); //adiciona o no nos ja pertencentes ao circuito
                if (searchedEdge->fromNode != adjacentNode)
                {
                    adjacentNode = searchedEdge->fromNode; //recebe outra extremidade da aresta. esse no vai procurar uma aresta nova para anexar
                }
                else if (searchedEdge->targetNode != adjacentNode)
                {
                    adjacentNode = searchedEdge->targetNode; //recebe outra extremidade da aresta. esse no vai procurar uma aresta nova para anexar
                }
                edgesNotCircuit.erase(searchedEdge);    //deleta aresta das que estao fora do circuito
                pEdgeCurrent = edgesNotCircuit.begin(); //volta para a primeira aresta
                isCompletedCicle = false;               //se achou aresta entao teve mais uma execucao em um ciclo
                //verifica se proximo no e o inicial
                if (initNode == adjacentNode)
                {
                    cout << "==================== Fechou mais um ciclo ==========================" << endl;
                    pNodeInit = nodesCircuit.begin(); //comeca novamente a analsiar do primeiro no do circuito
                    isCompletedCicle = true;
                    for(auto p = edgesCircuit.begin(); p != edgesCircuit.end(); ++p){
                        cout << **p << endl;
                    }
                }
            }
            else
                ++pEdgeCurrent; //avalia proxima aresta
        }
        //acabou todas arestas para o no x
        //cout << "Nao achou nenhum aresta nao visitada com " << *adjacentNode << ", vamos tentar o proximo no" << endl
            // << endl;
        ++pNodeInit;
        //zerou arestas nao analisadas
        if (edgesNotCircuit.size() == 0)
        {
           // cout << "Nao ha mais arestas para analisar" << endl;
            pNodeInit = nodesCircuit.end(); //para fechar o for dos nos do circuito
        }
    }

    //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

/* 
        6 10
        A B 1
        A C 1
        B C 1
        B D 1
        C E 1
        C D 1
        B E 1
        D F 1
        E F 1
        D E 1
        */


    if (isCompletedCicle == true)
    {
        cout << "+--------------+" << endl;
        cout << "| Eh euleriano |" << endl;
        cout << "+--------------+" << endl;
    }
    else
    {
        cout << "+------------------+" << endl;
        cout << "| Nao eh euleriano |" << endl;
        cout << "+------------------+" << endl;
    }

    return EXIT_SUCCESS;
}
