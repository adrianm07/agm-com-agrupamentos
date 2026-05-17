#include "Grafo.h"
#include <iostream>
#include <fstream>
#include <sstream>
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