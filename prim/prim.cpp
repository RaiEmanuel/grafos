#include <iostream>
#include <cmath>
#include <vector>
#include <set>
#include <tuple> // std::tuple, std::make_tuple, std::tie
#include <algorithm>
#include <fstream>
#define INFINITYPRIM -2147483648

using namespace std;

struct Node
{
    char id;
    Node *prox = nullptr;
    Node(char idNode) : id(idNode) {}
    Node(){};
};

ostream &operator<<(ostream &os, const Node &node)
{
    os << boolalpha;
    os << "[id = " << node.id << "]";
    if (node.prox != nullptr)
    {
        os << "[id proximo = " << node.prox->id << "]";
    }
    else
        os << "[id proximo = NULL]";
    //<< "[value = " << node.value << "]";
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
    Edge() {}
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

void listEdges(vector<Edge *> listEdges)
{
    for (auto i = listEdges.begin(); i != listEdges.end(); ++i)
    {
        cout << **i << endl;
    }
}

int cost(const char idNode1, const char idNode2, vector<Edge *> &listEdges)
{
    for (auto i = listEdges.begin(); i < listEdges.end(); ++i)
    {
        if ((*i)->fromNode->id == idNode1 && (*i)->targetNode->id == idNode2)
        {
            return (*i)->weight;
        }
        else if ((*i)->fromNode->id == idNode2 && (*i)->targetNode->id == idNode1)
        {
            return (*i)->weight;
        }
    }
    return INFINITYPRIM;
}

set<Edge *> *generateTreeMinimum(vector<Edge *> &listEdges, Node *nodes, const int numNodes)
{
    set<Edge *> *setF = new set<Edge *>; //deletar fora do metodo
    Edge *pEdgeMinimum = listEdges[0];
    cout << "aresta escolhida " << *pEdgeMinimum << endl;
    Node *pNodeU = pEdgeMinimum->fromNode;
    cout << "no u " << *pNodeU << endl;
    Node *pNodeV = pEdgeMinimum->targetNode;
    cout << "no v " << *pNodeV << endl;
    setF->insert(pEdgeMinimum);

    //zera os proximos dos nos da aresta inicial da arvore
    pNodeU->prox = nullptr;
    pNodeV->prox = nullptr;

    for (unsigned short int e = 0; e < numNodes; ++e)
    {
        //custo ate os nos da arvore
        //dois nos sem ligacao tem custo = INFINITOPRIM
        if (nodes[e].id != pNodeU->id && nodes[e].id != pNodeV->id)
        {
            cout << "------------ Node " << nodes[e].id << " --------------" << endl;
            int costNodeEtoU = cost(nodes[e].id, pNodeU->id, listEdges);
            cout << "\t custo to " << pNodeU->id << ": " << costNodeEtoU << endl;
            int costNodeEtoV = cost(nodes[e].id, pNodeV->id, listEdges);
            cout << "\t custo to " << pNodeV->id << ": " << costNodeEtoV << endl;

            if (costNodeEtoU == INFINITYPRIM && costNodeEtoV == INFINITYPRIM)
            {
                nodes[e].prox = pNodeU;
            }
            else if (costNodeEtoU == INFINITYPRIM && costNodeEtoV != INFINITYPRIM)
            {
                nodes[e].prox = pNodeV;
            }
            else if (costNodeEtoU != INFINITYPRIM && costNodeEtoV == INFINITYPRIM)
            {
                nodes[e].prox = pNodeU;
            }
            else if (costNodeEtoU != INFINITYPRIM && costNodeEtoU != INFINITYPRIM && costNodeEtoU < costNodeEtoV)
            {
                nodes[e].prox = pNodeU;
            }
            else if (costNodeEtoU != INFINITYPRIM && costNodeEtoU != INFINITYPRIM && costNodeEtoU >= costNodeEtoV)
            {
                nodes[e].prox = pNodeV;
            }
        }
    }
    unsigned short int counter = 0;
    unsigned short int nEdges = listEdges.size();
    while (counter < numNodes - 2)
    {
        //acha o j a ser usado
        // j.prox != 0 && cost(j, j.prox) eh minimo
        Node *pNodeMinimum;
        int costMinimum = INT_MAX;
        for (unsigned short int j = 0; j < numNodes; ++j)
        {
            if (nodes[j].prox != nullptr && cost(nodes[j].id, nodes[j].prox->id, listEdges) != INFINITYPRIM)
            {
                int costJ = cost(nodes[j].id, nodes[j].prox->id, listEdges);
                if (costJ < costMinimum)
                {
                    costMinimum = costJ;
                    pNodeMinimum = (nodes + j);
                }
            }
        }

        /* pNodeMinimum eh o ponteiro do no j */
        /* costMinimum eh o custo do no j */
        //acha a aresta entre j e j.prox
        Edge *pEdgeJtoProx = nullptr;
        for (auto x = listEdges.begin(); x != listEdges.end(); ++x)
        {
            if ((*x)->fromNode == pNodeMinimum && (*x)->targetNode == pNodeMinimum->prox)
            {
                pEdgeJtoProx = *x;
            }
            else if ((*x)->fromNode == pNodeMinimum->prox && (*x)->targetNode == pNodeMinimum)
            {
                pEdgeJtoProx = *x;
            }
        }
        setF->insert(pEdgeJtoProx);

        pNodeMinimum->prox = nullptr; // seta o proximo de j para nulo porque o no j agora faz parte da arvore

        //ajusta o proximo dos nos restantes em relacao ao novo j
        for (unsigned short int y = 0; y < numNodes; ++y)
        {
            if (nodes[y].prox != nullptr)
            {
                int costYtoNext = cost(nodes[y].id, nodes[y].prox->id, listEdges);
                int costYtoJ = cost(nodes[y].id, pNodeMinimum->id, listEdges);
                //avalia distancia do no analisado do momento com o no j
                if (costYtoNext == INFINITYPRIM && costYtoJ != INFINITYPRIM)
                {
                    //j passa a ser o proximo do no avaliado no momento
                    nodes[y].prox = pNodeMinimum;
                }
                else if (costYtoNext != INFINITYPRIM && costYtoJ != INFINITYPRIM && costYtoNext > costYtoJ)
                {
                    //j passa a ser o proximo do no avaliado no momento
                    nodes[y].prox = pNodeMinimum;
                }
            }
        }
        ++counter;
    }
    return setF;
}

Node *getAdressNode(Node *nodes, unsigned short int numNodes, char id)
{
    for (unsigned short int i = 0; i < numNodes; ++i)
    {
        if (nodes[i].id == id)
            return (nodes + i);
    }
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
    unsigned short int numNodes, numEdges;
    file >> numNodes;
    Node *nodes = new Node[numNodes];

    for (int i = 0; i < numNodes; ++i)
    {
        nodes[i] = Node('A' + i);
    }
    file >> numEdges;
    Edge *edges = new Edge[numEdges];

    vector<Edge *> listEdges;
    for (int i = 0; i < numEdges; ++i)
    {
        char idNode1, idNode2;
        int weight;
        file >> idNode1;
        file >> idNode2;
        Node *pNode1 = getAdressNode(nodes, numNodes, idNode1);
        Node *pNode2 = getAdressNode(nodes, numNodes, idNode2);
        file >> weight;
        if (pNode1 != nullptr && pNode2 != nullptr)
        {
            edges[i] = Edge(pNode1, pNode2, weight);
            listEdges.push_back(edges + i); //adiciona ponteiro da aresta que acabara de ser criada
        }
        else
        {
            cout << "Nó não existente" << endl;
            --i;
        }
    }
    sort(listEdges.begin(), listEdges.end(), compareByWeight);

    cout << "================== Lista de arestas ordenada para uso do Prim ==================" << endl;
    for (auto i = listEdges.begin(); i != listEdges.end(); ++i)
    {
        cout << **i << endl;
    }
    //conjunto F para armazenar as arestas da arvore
    set<Edge *> *setF = generateTreeMinimum(listEdges, nodes, numNodes);
    cout << "Listando arvore geradora minima" << endl;
    long long totalWeight = 0;
    for (auto f = setF->begin(); f != setF->end(); ++f)
    {
        cout << **f << endl;
        totalWeight += (*f)->weight;
    }
    cout << "---------------------------" << endl;
    cout << "Peso total da arvore: " << totalWeight << endl;

    delete setF;
    delete[] edges;
    delete[] nodes;
    return EXIT_SUCCESS;
}
