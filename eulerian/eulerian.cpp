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
    vector<Node *> path, subPath;

    unsigned short int nNodes, nEdges;
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
           // cout << "Aresta criada com sucesso " << *e << endl;
        }
    }
    //comeca a analise
    //====================================================
    Node *initNode;//ponteiro do no que comeca o ciclo
    bool isCompletedCicle = false;//true para dizer que completou um ciclo
    bool initCicle = false;//true para saber se comecou um novo ciclo

    for (auto pNodeInit = nodesNotCircuit.begin(); pNodeInit != nodesNotCircuit.end();)
    {
        initNode = *pNodeInit;
        //ve se no comeca um ciclo
        Node *adjacentNode = initNode; //no adjacent para iterar formando circuito. mesmo que no w do circuito
        //procura uma aresta com um no que nao esteja na lista dos que estao no circuito
        for (auto pEdgeCurrent = edgesNotCircuit.begin(); pEdgeCurrent != edgesNotCircuit.end();)
        {
            Edge *searchedEdge = lookupNodeinEdge(adjacentNode, *pEdgeCurrent); //verifica se o no inicial esta em alguma aresta nao pertencente ao circuito   
            if (searchedEdge != nullptr)
            {
                initCicle = true;
                path.push_back(adjacentNode);
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
                auto it = find(nodesNotCircuit.begin(), nodesNotCircuit.end(), adjacentNode);
                if(it != nodesNotCircuit.end()) nodesNotCircuit.erase(it);//apaga no adjacente dos nao pertencentes ao circuito
                pEdgeCurrent = edgesNotCircuit.begin(); //volta para a primeira aresta
                isCompletedCicle = false;               //se achou aresta entao teve mais uma execucao em um ciclo
                //verifica se proximo no e o inicial
                if (initNode == adjacentNode)
                {
                    cout << "==================== Fechou primeiro ciclo ==========================" << endl;
                    pNodeInit = nodesNotCircuit.end() - 1; 
                    pEdgeCurrent = edgesNotCircuit.end();//para sair do ciclo
                    isCompletedCicle = true;
                    path.push_back(initNode);
                    for(auto k = path.begin(); k != path.end(); ++k){
                        cout << (*k)->id << " -> ";
                    }cout << endl;
                    initCicle = false;
                }
            }else ++pEdgeCurrent; //avalia proxima aresta
        }//acabou todas arestas para o no x
        ++pNodeInit;
        if(initCicle == true){
            //iniciou o ciclo e travou
            cout << "acabaram possibilidades e não há para onde ir"<<endl;
            cout << "+------------------+" << endl;
            cout << "| Nao eh euleriano |" << endl;
            cout << "+------------------+" << endl;
            return EXIT_SUCCESS;
        }
        /*if(isCompletedCicle == true){
            pNodeInit = nodesNotCircuit.end();
        }*/
        /*if (edgesNotCircuit.size() == 0)
        {
            pNodeInit = nodesNotCircuit.end(); //para fechar o for dos nos do circuito
        }*/
        //zerou arestas nao analisadas
    }
    cout << "comecando segunda analise" << endl;
    //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
    //reinicia processo
    //iniciando novamente, mas agora com um vertice do circuito
    //precisa ter arestas nao adicionadas no ciruito para poder avaliar
    for (auto pNodeInit = nodesCircuit.begin(); pNodeInit != nodesCircuit.end();)
    {
        initNode = *pNodeInit;
        //ve se no comeca um ciclo
        Node *adjacentNode = initNode; //no adjacent para iterar formando circuito. mesmo que no w do circuito
        //procura uma aresta com um no que nao esteja na lista dos que estao no circuito
        for (auto pEdgeCurrent = edgesNotCircuit.begin(); pEdgeCurrent != edgesNotCircuit.end();)
        {
            Edge *searchedEdge = lookupNodeinEdge(adjacentNode, *pEdgeCurrent); //verifica se o no inicial esta em alguma aresta nao pertencente ao circuito   
            if (searchedEdge != nullptr)
            {
                initCicle = true;
                subPath.push_back(adjacentNode);
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
                    cout << "==================== Fechou ciclo intermediario ==========================" << endl;
                    pNodeInit = nodesCircuit.begin(); //comeca novamente a analsiar do primeiro no do circuito
                    //pEdgeCurrent = edgesNotCircuit.begin(); redundacia
                    isCompletedCicle = true;
                    initCicle = false;
                    auto nodeW = find(path.begin(), path.end(), initNode);
                    path.insert(nodeW, subPath.begin(), subPath.end());
                    subPath.clear();
                    for(auto k = path.begin(); k != path.end(); ++k){
                        cout << (*k)->id << "-> ";
                    }cout << endl;
                }
            }
            else
                ++pEdgeCurrent; //avalia proxima aresta
        }
        //acabou todas arestas para o no x
        ++pNodeInit;
        //iniciou o ciclo e travou
        if(initCicle == true){
            cout << "acabaram possibilidades e não há para onde ir"<<endl;
            cout << "+------------------+" << endl;
            cout << "| Nao eh euleriano |" << endl;
            cout << "+------------------+" << endl;
            return EXIT_SUCCESS;
        }
        //zerou arestas nao analisadas
        if (edgesNotCircuit.size() == 0)
        {
            //cout << "Nao ha mais arestas para analisar" << endl;
            pNodeInit = nodesCircuit.end(); //para fechar o for dos nos do circuito
        }
    }

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
