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
    int value;
    bool visited;
    set<Node *> *tree;
    Node(char idNode, int valueNode) : id(idNode), value(valueNode)
    {
        visited = false;
    };
    Node(){};
};

ostream &operator<<(ostream &os, const Node &node)
{
    os << boolalpha;
    os << "[id = " << node.id << "]";
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

set<Edge *> * generateTreeMinimum(vector<Edge *> &listEdges, const int nVertex)
{
    set<Edge *> * setF = new set<Edge *>;
    int j = 0;
    for (int i = 1; i < nVertex && listEdges.size() > 0;) // i é o contador dos nós que vão sendo anexados na árvore
    {   
        Edge * auxEdge = listEdges[j]; //pega a primeira aresta
        //cout << "[Aresta pega] = " << *auxEdge << endl;
        //apaga apenas da lista, não da heap com o 'delete', pois precisa usar a aresta futuramente
        Node *n1 = auxEdge->fromNode;
        Node *n2 = auxEdge->targetNode;
        //cout << "[No pego] = " << *n1 << endl;
        //cout << "[No pego] = " << *n2 << endl;
        //listEdges.erase(listEdges.begin()); //deleta primeira posição
        ++j;
        if (n1->tree != n2->tree)
        { //verifica se estão na mesma árvore
            setF->insert(auxEdge);
            //cout << "Arvores diferentes, deve-se juntar " << n1->id << " e " << n2->id << endl;
            //precisa deletar a arvore antiga de n2
            //set<Node *> * setDeleted = n2->tree;
            //delete n2->tree; não é ponteiro criado com new para deletar
            //atualiza a árvore do nó 2 com a árvore do nó 1
            set<Node *> * pDeletedTree = n2->tree;//pega ponteiro da arvore a ser deletada
            n1->tree->insert(n2);//árvore do n1 adiciona o nó n2 para juntos fazerem parte da mesma árvore
            //n2->tree = n1->tree; //estão na mesma árvore, a tree do n2 precisa ser a mesma do n1
            //todos os elementos da arvore do no 2 precsam ser atualizados, inclusive o no 2
            for(auto i = pDeletedTree->begin(); i != pDeletedTree->end(); ++i){
                (*i)->tree = n1->tree;
            }
            //cout << n1->tree<<endl;
            //cout <<" === tree"<<endl;
            //cout << n2->tree<<endl;
            ++i;                 //mais um nó foi anexado na árvore
        }
        //cout << "---------------------------------------" << endl;
    }
    return setF;
}

int main(int argc, char const *argv[])
{
    Node nA = Node('A', 12);
    Node nB = Node('B', 47);
    Node nC = Node('C', 55);
    Node nD = Node('D', 31);
    Node nE = Node('E', 90);
    Node nF = Node('F', 78);

    int nVertex = 6; //A, B, C, D, E e F

    Edge AB = Edge(&nA, &nB, 4);
    Edge AD = Edge(&nA, &nD, 3);
    Edge AE = Edge(&nA, &nE, 4);

    Edge BE = Edge(&nB, &nE, 9);
    Edge BF = Edge(&nB, &nF, 5);

    Edge CD = Edge(&nC, &nD, 9);
    Edge CB = Edge(&nC, &nB, 8);
    Edge CF = Edge(&nC, &nF, 2);
    Edge CE = Edge(&nC, &nE, 3);
    
    Edge DF = Edge(&nD, &nF, 7);

    Edge EF = Edge(&nE, &nF, 2);

    vector<Edge *> listEdges;
    listEdges.push_back(&AB);
    listEdges.push_back(&AD);
    listEdges.push_back(&AE);

    listEdges.push_back(&BE);
    listEdges.push_back(&BF);

    listEdges.push_back(&CD);
    listEdges.push_back(&CB);
    listEdges.push_back(&CF);
    listEdges.push_back(&CE);

    listEdges.push_back(&DF);

    listEdges.push_back(&EF);
    

    sort(listEdges.begin(), listEdges.end(), compareByWeight);

    cout << "================== Lista de arestas ordenada para uso do Kruskal ==================" << endl;
    for (auto i = listEdges.begin(); i != listEdges.end(); ++i)
    {
        cout << **i << endl;
    }

    //cria um set para cada vértice
    //for(int i = 0; i < 5; ++i){
    set<Node *> treeA;
    treeA.insert(&nA);
    nA.tree = &treeA;

    set<Node *> treeB;
    treeB.insert(&nB);
    nB.tree = &treeB;

    set<Node *> treeC;
    treeC.insert(&nC);
    nC.tree = &treeC;

    set<Node *> treeD;
    treeD.insert(&nD);
    nD.tree = &treeD;

    set<Node *> treeE;
    treeE.insert(&nE);
    nE.tree = &treeE;

    set<Node *> treeF;
    treeF.insert(&nF);
    nF.tree = &treeF;
    //}
    cout << "Montando arvore geradora minima..."<<endl;
    set<Edge *> * setF = generateTreeMinimum(listEdges, nVertex);//lembrar de dar delete no setF criado na função

    int total = 0;
    cout << "Exibindo arvore geradora minima..." << endl;
    for(auto i = setF->begin(); i != setF->end(); ++i){
        cout << **i << endl;
        total += (*i)->weight;
    }
    cout << "Total: " << total << endl;
    
    delete setF;
    return EXIT_SUCCESS;
}
