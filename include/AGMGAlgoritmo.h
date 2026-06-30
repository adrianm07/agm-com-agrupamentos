#ifndef AGMG_ALGORITMO_H
#define AGMG_ALGORITMO_H

#include "Grafo.h"
#include "Grupo.h"
#include <random>
#include <vector>

using std::mt19937;

class AGMGAlgoritmo {
public:
    AGMGAlgoritmo(unsigned seed);

    // Retorna a melhor AGM com agrupamentos encontrada
    Grafo* guloso(Grafo& g);

    // Repete 'iteracoes' vezes com alpha fixo e retorna a melhor solução
    Grafo* gulosoRandomizado(Grafo& g, double alpha, int iteracoes);

    // Adapta as probabilidades dos alphas a cada iteração com base na qualidade
    Grafo* gulosoReativo(Grafo& g, const vector<double>& alphas, int iteracoes);

private:
    std::mt19937 rng;

    Grafo* heuristica(Grafo& g, double alpha); // primModificado + poda
    Grafo* primModificado(Grafo& g, double alpha);
    void poda(Grafo& agm);
};


#endif