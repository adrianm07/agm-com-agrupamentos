#include "Vertice.h"
#include "Grupo.h"

#include <iostream>

// Construtor
Vertice::Vertice(int id) {
    this->id = id;
    this->visitado = false;
    this->grupo = nullptr;
}


Vertice::~Vertice() {}

int Vertice::getId() {
    return id;
}

void Vertice::setVisitado(bool visitado) {
    this->visitado = visitado;
}

bool Vertice::getVisitado() {
    return visitado;
}

int Vertice::getGrau() {
    return arestas.size();
}

// ADICIONAR ARESTA
void Vertice::addAresta(Vertice *v, double peso) {
    Aresta novaAresta(v, peso);
    arestas.push_back(novaAresta);
}

// REMOVER ARESTA
void Vertice::removeAresta(Vertice *v) {
    for (int i = 0; i < arestas.size(); i++) {
        if (arestas[i].getDestino() == v) {
            arestas.erase(arestas.begin() + i);
            break; 
        }
    }
}

// PEGAR ADJACENTE (Pela posição no vetor)
Vertice* Vertice::getAdjacente(int pos) {
    if (pos >= 0 && pos < arestas.size()) {
        return arestas[pos].getDestino();
    }
    return nullptr; // Retorna nulo se a posição for inválida
}

// Alterar o peso de uma aresta específica
void Vertice::alterarPesoAresta(Vertice *v, double novoPeso) {
    for (auto& aresta : arestas) {
        if (aresta.getDestino() == v) {
            aresta.setPeso(novoPeso);
            break;
        }
    }
}

// Verificar se existe aresta para o vértice V
bool Vertice::temArestaPara(Vertice *v) {
    for (auto& aresta : arestas) {
        if (aresta.getDestino() == v) {
            return true;
        }
    }
    return false;
}

vector<Aresta> Vertice::getArestas() {
    return arestas;
}

void Vertice::setGrupo(Grupo* grupo) {
    this->grupo = grupo;
}

Grupo* Vertice::getGrupo() {
    return grupo;
}