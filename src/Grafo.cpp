#include "Grafo.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
#include <stdexcept>
#include <iomanip>
using namespace std;

Grafo::Grafo(bool orientado) {
    this->orientado = orientado;
    this->numVertices = 0;
    cout<< fixed << setprecision(1); 
}

bool Grafo::isPonderado() {
    for (auto& par : vertices) {
        for (Aresta& aresta : par.second->getArestas()) {
            if (aresta.getPeso() != 1.0) {
                return true;
            }
        }
    }
    return false;
}

Grafo::~Grafo() {
   for(auto& par : vertices) {
    delete par.second;
   }

   for(Grupo* g : grupos)
        delete g;
}

void Grafo::addVertice(int id, Grupo* grupo) {
    if (vertices.find(id) == vertices.end()) {
        Vertice* v = new Vertice(id);

        v->setGrupo(grupo);
        if (grupo != nullptr) {
            grupo->addVertice(v);
        }

        vertices[id] = v;
        numVertices++;
    }
}

void Grafo::removeVertice(int id) {
    if (vertices.find(id) == vertices.end()){
        return;
    }

    //remove arestas deste vertice
    Vertice* vRemover = vertices[id];
    for (auto& par : vertices) {
        par.second->removeAresta(vRemover);
    }

    //remove vertice do grupo
    Grupo* g = vRemover->getGrupo();
    if (g != nullptr) {
        vector<Vertice*>& verts = g->getListaVertices();

        for (auto it = verts.begin(); it != verts.end(); ++it) {
            if ((*it)->getId() == id) {
                verts.erase(it);
                break;
            }
        }
    }

    vertices.erase(id);
    delete vRemover;
    numVertices--;
}

void Grafo::addAresta(int origem, int destino, double peso) {
    if (verificarAdjacencia(origem, destino)) {
        return; 
    }

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
    cout << numVertices << endl;
    for (auto& par : vertices) {
        Vertice* v = par.second;
        for (Aresta& a : v->getArestas()) {
            if (!orientado && v->getId() > a.getDestino()->getId()) continue;
            cout << v->getId() << " " << a.getDestino()->getId() << " " << a.getPeso() << endl;
        }
    }
}


void Grafo::lerArquivo(const string& nomeArquivo) {
    ifstream arquivo(nomeArquivo);
    string linha;

    if (!arquivo.is_open()) {
        throw std::runtime_error("Erro ao abrir o arquivo: " + nomeArquivo);
    }

    while (getline(arquivo, linha)) {
        if (linha.empty()) continue;

        stringstream ss(linha);
        int origem, destino;
        double peso = 1.0;

        if(ss >> origem >> destino) {
            if(!(ss >> peso)) {
                peso = 1.0;
            }

            addVertice(origem);
            addVertice(destino);

            addAresta(origem, destino, peso);

        } else {
            ss.clear();
            ss.str(linha);

            if(ss >> origem) {
                addVertice(origem);
            }
        }
    }

    arquivo.close();
    cout << "Arquivo importado com sucesso!" << endl;
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

    if(orientado){
        throw std::invalid_argument("Erro: O algoritmo exige um grafo nao orientado");
    }

    if(!isPonderado()){
        throw std::invalid_argument("Erro: O algoritmo exige um grafo ponderado");
    }

    if(vertices.empty()){
        throw std::runtime_error("Erro: O grafo esta vazio");
    }

    for(auto& par : vertices){
        par.second->setVisitado(false); // seta todos os vertice como não visitados antes de iniciar a execução
    }

    Grafo* agm = new Grafo(false); // cria um grafo nao orientado

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

void Grafo::addGrupo(int id) {
    if(getGrupo(id) == nullptr) {
        grupos.push_back(new Grupo(id));
    }
}

Grupo* Grafo::getGrupo(int id) {
    for(Grupo* g : grupos) {
        if(g->getId() == id) {
            return g;
        }
    }

    return nullptr;
}

vector<Grupo*> Grafo::getGrupos() {
    return grupos;
}

map<int, Vertice*>& Grafo::getVertices() {
    return vertices;
}

Vertice* Grafo::getVertice(int id) {
    auto it = vertices.find(id);

    if(it == vertices.end()) {
        return nullptr;
    }

    return it->second;
}

int Grafo::getNumVertices() {
    return numVertices;
}

double Grafo::getCusto() {
    double custoTotal = 0.0;

    std::set<std::pair<int,int>> visitadas;

    for (auto& par : vertices) {
        Vertice* vertice = par.second;

        for (Aresta& aresta : vertice->getArestas()) {

            int vId = vertice->getId();
            int vizinhoId = aresta.getDestino()->getId();

            //evita contar aresta duas vezes
            std::pair<int,int> arestaPair = std::minmax(vId, vizinhoId);

            //procura aresta no visitadas se não encontrar soma o peso
            if (visitadas.find(arestaPair) == visitadas.end()) {
                visitadas.insert(arestaPair);
                custoTotal += aresta.getPeso();
            }
        }
    }

    return custoTotal;
}
