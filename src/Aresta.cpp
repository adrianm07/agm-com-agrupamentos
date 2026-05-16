#include "Aresta.h"
#include "Vertice.h"

Aresta::Aresta(Vertice *v, double peso) {
    this->destino = v;
    this->peso = peso;
}

Aresta::~Aresta() {}

Vertice *Aresta::getDestino() {
    return destino;
}

double Aresta::getPeso() {
    return peso;
}

void Aresta::setDestino(Vertice *v) {
    destino = v;
}

void Aresta::setPeso(double p) {
    peso = p;
}