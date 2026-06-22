#ifndef GRAFO_H
#define GRAFO_H
#include "Vertice.h"
#include "Grupo.h"
#include <algorithm>
#include <utility>
#include <map>
#include <vector>
#include <set>

using namespace std;

class Grafo {
public:
    Grafo(bool orientado);
    ~Grafo();
    void addVertice(int id, Grupo* grupo = nullptr);
    void removeVertice(int id);
    void addAresta(int origem, int destino, double peso = 1);
    void removeAresta(int origem, int destino);
    bool verificarAdjacencia(int origem, int destino);
    void alterarPesoAresta(int origem, int destino, double peso);
    void imprimirGrafo();
    void lerArquivo(const string& nomeArquivo);
    int getGrau(int id);
    vector<Aresta> listarArestas(int id);
    vector<int> listarVizinhos(int id);
    Grafo* primAGM(double *custo);
    bool isPonderado();
    double getCusto();

    void addGrupo(int id);
    Grupo* getGrupo(int id);
    vector<Grupo*> getGrupos();
    
    map<int, Vertice*>& getVertices();
    Vertice* getVertice(int id);
    int getNumVertices();

private:
    bool orientado;  
    map<int, Vertice*> vertices; 
    vector<Grupo*> grupos;
    int numVertices;
};

#endif