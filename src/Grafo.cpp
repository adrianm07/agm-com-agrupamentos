#include "Grafo.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
using namespace std;

Grafo::Grafo(bool orientado, bool ponderado) {
    this->orientado = orientado;
    this->ponderado = ponderado;
    this->numVertices = 0;
}


Grafo::~Grafo() {
   for(auto& par : vertices) {
    delete par.second;
   }
}

void Grafo::addVertice(int id) {
    if (vertices.find(id) == vertices.end()) {
        vertices[id] = new Vertice(id);
        numVertices++;
    }
}

void Grafo::removeVertice(int id) {
    if (vertices.find(id) == vertices.end()){
        return;
    }

    Vertice* vRemover = vertices[id];
    for (auto& par : vertices) {
        par.second->removeAresta(vRemover);
    }
    delete vRemover;
    vertices.erase(id);
    numVertices--;
}
void Grafo::addAresta(int origem, int destino, double peso) {
    addVertice(origem);
    addVertice(destino);
    Vertice* vOrigem = vertices[origem];
    Vertice* vDestino = vertices[destino];
    vOrigem->addAresta(vDestino, peso);
    if (!orientado) {
        vDestino->addAresta(vOrigem, peso);
    }
}

void Grafo::removeAresta(int origem, int destino) {
    if (vertices.find(origem) != vertices.end() && vertices.find(destino) != vertices.end()) {
        vertices[origem]->removeAresta(vertices[destino]);
        if (!orientado) {
            vertices[destino]->removeAresta(vertices[origem]);
        }
    }
}

bool Grafo::verificarAdjacencia(int origem, int destino) {
    if (vertices.find(origem) != vertices.end() && vertices.find(destino) != vertices.end()) {
        return vertices[origem]->temArestaPara(vertices[destino]);
    }
    return false;
}


void Grafo::alterarPesoAresta(int origem, int destino, double peso) {
    if (vertices.find(origem) != vertices.end() && vertices.find(destino) != vertices.end()) {
        vertices[origem]->alterarPesoAresta(vertices[destino], peso);
        if (!orientado) {
            vertices[destino]->alterarPesoAresta(vertices[origem], peso);
        }
    }
}


void Grafo::imprimirGrafo() {
    cout << "--- Representacao do Grafo ---" << endl;
    for (auto& par : vertices) {
        Vertice* v = par.second;
        cout << "No " << v->getId() << " -> ";
        if (v->getGrau() == 0) {
            cout << "(isolado)";
        } else {
            for (int i = 0; i < v->getGrau(); i++) {
                Vertice* adj = v->getAdjacente(i);
                cout << "[" << adj->getId() << "] ";
            }
        }
        cout << endl;
    }
}


void Grafo::lerArquivo(const string& nomeArquivo) {
    ifstream arquivo(nomeArquivo);
    string linha;

    if (!arquivo.is_open()) {
        cerr << "Erro ao abrir o arquivo: " << nomeArquivo << endl;
        return;
    }

    while (getline(arquivo, linha)) {
        if (linha.empty()) continue;

        stringstream ss(linha);
        int origem, destino;
        double peso = 1.0;

        if (ss >> origem) {
            if (ss >> destino) {
                if (ponderado) {
                    ss >> peso;
                }
                addAresta(origem, destino, peso);
            } else {
                addVertice(origem);
            }
        }
    }
    arquivo.close();
}


int Grafo::getGrau(int id) {
    if (vertices.find(id) != vertices.end()) {
        return vertices[id]->getGrau();
    }

    return -1; 
}

vector<Aresta> Grafo::listarArestas(int id) {
    if (vertices.find(id) != vertices.end()) {
        return vertices[id]->getArestas();
    }

    return vector<Aresta>(); 
}

vector<int> Grafo::listarVizinhos(int id) {
    vector<int> vizinhos;

    if (vertices.find(id) != vertices.end()) {
        Vertice* v = vertices[id];

        for (int i = 0; i < v->getGrau(); i++) {
            Vertice* adj = v->getAdjacente(i);
            vizinhos.push_back(adj->getId());
        }
    }

    return vizinhos;
}

// Estrutura auxiliar para a fila de arestas ja que a aresta.cpp não contem a origem
//Ficaria inviável implementar sem isso pois na fila, eu não saberia de onde pra onde a aresta vai
struct ArestaAux {
    int origem;
    int destino;
    double peso;

    bool operator>(const ArestaAux& outra) const {
        return (peso > outra.peso);
    }
};

Grafo* Grafo::primAGM(double* custo) {

    if(!ponderado){
        cerr << "Erro: O algoritmo exige um grafo ponderado" << endl;
        return nullptr;
    }

    if(vertices.empty()){
        cerr << "Erro: O grafo está vazio" << endl;
        return nullptr;
    }

    for(auto& par : vertices){
        par.second->setVisitado(false); // seta todos os vertice como não visitados antes de iniciar a execução
    }

    Grafo* agm = new Grafo(false, true); // cria um grafo nao orientado e ponderado

    priority_queue<ArestaAux, vector<ArestaAux>, greater<ArestaAux>> filaPrioridade; // cria uma fila de ArestaAux e que é um Min-Heap(usando o greater e o operator>)

    Vertice* vInicial = vertices.begin()->second; // define o vertice inicial como o primeiro vertice do mapa
    int idInicial = vInicial->getId();

    vInicial->setVisitado(true); // marca ele como visitado
    agm->addVertice(idInicial); // e adiciona ele na árvore
 
    for(Aresta& aresta : vInicial->getArestas()){
        filaPrioridade.push({idInicial, aresta.getDestino()->getId(), aresta.getPeso()}); // adiciona as arestas do vértice inicial na fila
    }

    double custoTotal = 0;
    int arestasAdicionadas = 0;

    while(!filaPrioridade.empty() && arestasAdicionadas < numVertices - 1){ // repete enquanto a fila não estiver vazia ou enquanto nao satisfizer o limite de arestas da arvore

        ArestaAux arestaAtual = filaPrioridade.top(); // pega a do topo da heap
        filaPrioridade.pop(); // remove a aresta do topo da heap

        int origem = arestaAtual.origem;
        int destino = arestaAtual.destino;
        double peso = arestaAtual.peso;

        Vertice* vDestino = vertices[destino];

        if(vDestino->getVisitado()){ // verifico se a menor aresta é ligada a um nó ja vistado(evitando ciclos)
            continue; // se sim, proxima iteração
        }
        // se não, adiciono a aresta na arvore
        vDestino->setVisitado(true);
        agm->addAresta(origem, destino, peso);
        custoTotal += peso;
        arestasAdicionadas++;

        for(Aresta& aresta : vDestino->getArestas()){ // pra cada aresta do proximo nó, adiciona na fila e o processo continua
            if(!aresta.getDestino()->getVisitado()){
                filaPrioridade.push({destino, aresta.getDestino()->getId(), aresta.getPeso()});
            }
        }

    }

    if(arestasAdicionadas < numVertices - 1){
        cout << "Aviso: O grafo é desconexo." << endl;
    }

    if(custo != nullptr){
        *custo = custoTotal;
    }
    cout << "--- Execucao do Algoritmo de Prim-AGM ---" << endl;
    cout << "Custo total: " << custoTotal << endl;

    return agm;
}
