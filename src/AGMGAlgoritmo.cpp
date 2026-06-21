#include "AGMGAlgoritmo.h"
#include "Vertice.h"
#include <algorithm>

using namespace std;

AGMGAlgoritmo::AGMGAlgoritmo(unsigned seed) : rng(seed) {}

struct ArestaAux {
    int origem;
    int destino;
    double peso;

    bool operator>(const ArestaAux& outra) const {
        return (peso > outra.peso);
    }
};

Grafo* AGMGAlgoritmo::primRandomizado(Grafo& g, double alpha) {
  if(alpha < 0.0 || alpha > 1.0) {
    throw invalid_argument(
        "Alpha deve estar entre 0 e 1"
    );
  }

  for(auto& par : g.getVertices()) {
    par.second->setVisitado(false);
  }
  
  Grafo* agm = new Grafo(false);

  vector<ArestaAux> candidatos;

  const auto& vertices = g.getVertices();
  auto it = vertices.begin();
    
  uniform_int_distribution<int> distInicial(0, vertices.size() - 1);
  //escolhe um vértice aleatório "andando" com o iterador
  advance(it, distInicial(rng));
  Vertice* vInicial = it->second;
  
  vInicial->setVisitado(true);
  agm->addVertice(vInicial->getId());
  
  //adiciona as arestas do vértice inicial na lista de candidatos
  for(Aresta& aresta : vInicial->getArestas()) {
    candidatos.push_back({vInicial->getId(), aresta.getDestino()->getId(), aresta.getPeso()});
  }

  int arestasAdicionadas = 0;

  while(!candidatos.empty() && arestasAdicionadas < g.getNumVertices() - 1) {
    double menorPeso = candidatos[0].peso;
    double maiorPeso = candidatos[0].peso;

    for(const ArestaAux& a : candidatos) {
      if(a.peso < menorPeso)
          menorPeso = a.peso;

      if(a.peso > maiorPeso)
          maiorPeso = a.peso;
    }

    double limite = menorPeso + alpha * (maiorPeso - menorPeso);

    //cria lista restrita de candidatos com arestas de pesos menores ou iguais ao limite
    vector<ArestaAux> lrc;
    for(const ArestaAux& a : candidatos) {
      if(a.peso <= limite) {
          lrc.push_back(a);
      }
    }

    //seleciona aresta aleatória da lista restrita
    uniform_int_distribution<int> distLRC(0, lrc.size() - 1);
    int indiceAleatorio = distLRC(rng);
    ArestaAux arestaSelecionada = lrc[indiceAleatorio];

    int origem = arestaSelecionada.origem;
    int destino = arestaSelecionada.destino;
    double peso = arestaSelecionada.peso;
    
    Vertice* vDestino = g.getVertice(destino);

    if(vDestino->getVisitado()) {
      //remove a aresta selecionadada da lista de candidatos
      candidatos.erase(remove_if(candidatos.begin(), candidatos.end(),
        [&origem, &destino](const ArestaAux& a) {
          return a.origem == origem && a.destino == destino;
        }),
        candidatos.end());

      continue;
    }
    
    vDestino->setVisitado(true);

    agm->addAresta(origem, destino, peso);
    arestasAdicionadas++;

    //remove a aresta selecionada da lista de candidatos
    candidatos.erase(remove_if(candidatos.begin(), candidatos.end(),
      [&origem, &destino](const ArestaAux& a) {
        return a.origem == origem && a.destino == destino;
      }),
      candidatos.end());

    //adiciona as arestas do vértice destino na lista de candidatos
    for(Aresta& aresta : vDestino->getArestas()) {
      if(!aresta.getDestino()->getVisitado()) {
        candidatos.push_back({vDestino->getId(), aresta.getDestino()->getId(), aresta.getPeso()});
      }
    }
  }

  return agm;
}
