#ifndef GRAFO_TESTS_H
#define GRAFO_TESTS_H

#include "Grafo.h"
#include <vector>

class GrafoTests {
public:
  static void testAll();


private:
  class GrafoWrapper {
  private:
    Grafo* grafo;

  public:
    GrafoWrapper(bool orientado) {
        grafo = new Grafo(orientado);
    }

    ~GrafoWrapper() {
        delete grafo;
    }

    void inserirVertice(int v) {
        grafo->addVertice(v);
    }

    void removerVertice(int v) {
        grafo->removeVertice(v);
    }

    void inserirAresta(int u, int v, double peso = 1.0) {
        grafo->addAresta(u, v, peso);
    }

    void removerAresta(int u, int v) {
        grafo->removeAresta(u, v);
    }

    bool verificarAresta(int u, int v) {
        return grafo->verificarAdjacencia(u, v);
    }

    void alterarPesoAresta(int u, int v, double peso) {
        grafo->alterarPesoAresta(u, v, peso);
    }

    void exibirGrafo() {
        grafo->imprimirGrafo();
    }

    int grauVertice(int v) {
        int grau = grafo->getGrau(v);

        //O testAll espera 0 para vertice inexistente
        if (grau < 0) {
            return 0;
        }

        return grau;
    }

    vector<int> listarVizinhos(int v) {
        return grafo->listarVizinhos(v);
    }

    bool saoAdjacentes(int u, int v) {
        return grafo->verificarAdjacencia(u, v);
    }
};


};

#endif