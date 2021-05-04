#include <iostream>
#include <cmath>
#include <vector>
#include <set>
#include <tuple>        // std::tuple, std::make_tuple, std::tie

using namespace std;

struct Node{
    char id;
    int value;
    bool visited;
    vector<Node *> adj;
    Node(char idNode, int valueNode):id(idNode), value(valueNode){
        visited = false;
    };
    Node(){};
};

ostream& operator<<(ostream& os, const Node & node)
{
    os << boolalpha;
    os << "id: " << node.id << " :: "<< "value: " << node.value << " :: "<< "visited: " << node.visited;
    return os;
}

bool operator<(const Node& n1, const Node& n2)
{
    return std::tie(n1.id) < std::tie(n2.id);
}

//marca o nó como lido e visita os adjacentes possíveis
void mark(Node * node, set<Node> & setNodes){
    node->visited = true;
    for(auto p = node->adj.begin(); p != node->adj.end(); ++p ){
        Node * n = *p;
        if(n->visited == false){
            setNodes.insert(*n);//insere nó adjacente no conjunto
            mark(n, setNodes);
        }
    }
}

//verifica nó por nó a busca em profundidade
void depthSearch(const vector<Node *> lista, unsigned short int & nConexComponents, vector<set<Node>> & listSets){
    unsigned short int nNodes = lista.size();
    //zera lista
    for(unsigned short int i = 0; i < nNodes; i++){
        lista.at(i)->visited = false;
    }
    //busca em profundidade cada nó disponível
    for(unsigned short int i = 0; i < nNodes; i++){
        Node n = *lista.at(i);
        if(n.visited == false){
            ++nConexComponents;
            set<Node> setNodes;
            setNodes.insert(n);
            mark(lista.at(i), setNodes);
            listSets.push_back(setNodes);
        }
    }
}



int main(int argc, char const *argv[])
{
    Node nA = Node('A', 12);
    Node nB = Node('B', 47);
    Node nC = Node('C', 55);
    Node nD = Node('D', 31);
    Node nE = Node('E', 78);
    Node nF = Node('F', 34);
    Node nG = Node('G', 34);
    Node nH = Node('H', 34);

    nA.adj.push_back(&nB);
    nB.adj.push_back(&nA);

    nC.adj.push_back(&nB);
    nB.adj.push_back(&nC);

    nC.adj.push_back(&nE);
    nE.adj.push_back(&nC);
    
    nD.adj.push_back(&nF);
    nF.adj.push_back(&nD);

   /* nB.adj.push_back(&nC);
    nC.adj.push_back(&nB);*/

    /*nC.adj.push_back(&nD);
    nD.adj.push_back(&nC);

    nA.adj.push_back(&nD);
    nD.adj.push_back(&nA);*/

    /*nA.adj.push_back(&nC);
    nC.adj.push_back(&nA);*/

    

  /*  nB.adj.push_back(&nE);
    nE.adj.push_back(&nB);*/

    //lista com todos os nos
    vector<Node *> lista;
    lista.push_back(&nA);
    lista.push_back(&nB);
    lista.push_back(&nC);
    lista.push_back(&nD);
    lista.push_back(&nE);
    lista.push_back(&nF);
    lista.push_back(&nG);
    lista.push_back(&nH);


    //Exibe a lista de adjacência
    unsigned short int nNodes = lista.size();
    //zera lista
    cout << "Lista de adjacencia" << endl;
    for(unsigned short int i = 0; i < nNodes; i++){
        Node * node = lista.at(i);        
        for(auto p = node->adj.begin(); p != node->adj.end(); ++p ){
            cout << node->id << " -> " << (*p)->id << endl;   
        }
        cout << "--------------------------" << endl;
    }
    unsigned short int nConexComponents = 0;
    vector<set<Node>> listSets;
    depthSearch(lista, nConexComponents, listSets);
    cout << "[Ha " << nConexComponents << " componentes conexas no grafo.]" << endl;
    for (auto p = listSets.begin(); p != listSets.end(); ++p){
        cout << "Componente conexa: " << endl;
        set<Node> s = *p;
        for (auto z = s.begin(); z != s.end(); ++z){
            cout << "\t"<<*z << endl;
        }cout <<"---------------------------"<<endl;
    }
    return EXIT_SUCCESS;
}
