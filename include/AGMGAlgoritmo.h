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

    Grafo* gulosoRandomizado(Grafo& g, double alpha);
    void buscaLocal(Grafo& g);

private:
    std::mt19937 rng;
    Grafo* primRandomizado(Grafo& g, double alpha);

};

#endif