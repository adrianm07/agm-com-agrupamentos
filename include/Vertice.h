#ifndef VERTICE_H
#define VERTICE_H
#include <vector>
#include "Aresta.h"
using namespace std;



class Vertice {
public:
    Vertice(int id);
    ~Vertice();
    void setVisitado(bool visitado);
    bool getVisitado();
    void addAresta(Vertice *v, double peso);
    void removeAresta(Vertice *v);
    void alterarPesoAresta(Vertice *v, double novoPeso);
    bool temArestaPara(Vertice *v);
    Vertice *getAdjacente(int pos);
    int getGrau();
    int getId();
    vector<Aresta> getArestas();
    int getGrupo();

    void setGrupoId(int grupoId);
    int getGrupoId();

private:
    int id;
    int grupoId;
    
    vector<Aresta> arestas;
    //Para o prim e busca 
    bool visitado;
};

#endif