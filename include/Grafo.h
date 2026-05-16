#ifndef GRAFO_H
#define GRAFO_H
#include "Vertice.h"
#include <unordered_map>
using namespace std;

class Grafo {
public:
    Grafo(bool orientado, bool ponderado);
    ~Grafo();
    void addVertice(int id);
    void removeVertice(int id);
    void addAresta(int origem, int destino, double peso = 1);
    void removeAresta(int origem, int destino);
    bool verificarAdjacencia(int origem, int destino);
    void alterarPesoAresta(int origem, int destino, double peso);
    void imprimirGrafo();
    void lerArquivo(const string& nomeArquivo);
    

private:
    bool orientado;  
    bool ponderado;  
    unordered_map<int, Vertice*> vertices; 
    int numVertices;
};

#endif