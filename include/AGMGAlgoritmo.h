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

    
private:
    std::mt19937 rng;
    
    Grafo* gulosoRandomizado(Grafo& g, double alpha);
    Grafo* primRandomizado(Grafo& g, double alpha);

    void poda(Grafo& agm);
    void buscaLocal(Grafo& agm, Grafo& original);
};  

#endif