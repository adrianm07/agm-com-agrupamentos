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

Grafo* AGMGAlgoritmo::grasp(Grafo& original, double alpha, int iteracoes) {
    Grafo* melhorGlobal = nullptr;
    double melhorCustoGlobal = std::numeric_limits<double>::max();

    for (int i = 0; i < iteracoes; i++) {
      //poda ja acontece aqui
      Grafo* solucaoInicial = gulosoRandomizado(original, alpha);

      buscaLocal(*solucaoInicial, original);

      double custo = solucaoInicial->getCusto();

      //atualiza melhor solução global
      if (custo < melhorCustoGlobal) {
        delete melhorGlobal;

        melhorCustoGlobal = custo;
        melhorGlobal = solucaoInicial;

        solucaoInicial = nullptr;
      } 
      
      if(solucaoInicial != nullptr) {
        delete solucaoInicial;
      }
    }

    return melhorGlobal;
}

Grafo* AGMGAlgoritmo::gulosoRandomizado(Grafo& g, double alpha) {
    Grafo* agm = primRandomizado(g, alpha);

    poda(*agm);

    return agm;
}

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

  for(Grupo* grupo : g.getGrupos()) {
    agm->addGrupo(grupo->getId());
  }
  
  vector<ArestaAux> candidatos;

  const auto& vertices = g.getVertices();
  auto it = vertices.begin();
    
  uniform_int_distribution<int> distInicial(0, vertices.size() - 1);
  //escolhe um vértice aleatório "andando" com o iterador
  advance(it, distInicial(rng));
  Vertice* vInicial = it->second;
  
  vInicial->setVisitado(true);
  agm->addVertice(vInicial->getId(), vInicial->getGrupo());
  
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

    Vertice* vOrigem = g.getVertice(origem);

    agm->addVertice(origem, vOrigem->getGrupo());
    agm->addVertice(destino, vDestino->getGrupo());

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

void AGMGAlgoritmo::poda(Grafo& agm) {
  bool removeu;

  do {
    removeu = false;
    
    vector<int> verticesParaRemover;

    for(auto& par : agm.getVertices()) {
      Vertice* v = par.second;
     
      //só analisamos folhas
      if(v->getGrau() != 1) {
        continue;
      }
      
      Grupo* grupo = v->getGrupo();

      //só removemos vértices que pertencem a grupos com mais de um vértice
      if(grupo->getQuantidadeVertices() == 1) {
        continue;
      }
      
      verticesParaRemover.push_back(v->getId());
    }
    
    for(int id : verticesParaRemover) {
      agm.removeVertice(id);
      removeu = true;
    }
  } while(removeu);
}

void AGMGAlgoritmo::buscaLocal(Grafo& agm, Grafo& original) {
  bool melhorou;

  do {
    melhorou = false;

    vector<Vertice*> folhas;

    //encontrar folhas que pertencem a grupos com apenas um vértice
    for (auto& par : agm.getVertices()) {
      Vertice* v = par.second;

      if (v->getGrau() == 1 && v->getGrupo()->getQuantidadeVertices() == 1) {
        folhas.push_back(v);
      }
    }

    //tentar melhorar cada folha
    for (Vertice* folha : folhas) {

      Aresta a = folha->getArestas()[0];
      Vertice* vizinhoAtual = a.getDestino();
      int custoAtual = a.getPeso();

      //remove temporariamente
      agm.removeAresta(folha->getId(), vizinhoAtual->getId());

      bool encontrouMelhoria = false;

      //tenta novas conexões 
      for (auto& par : agm.getVertices()) {
        Vertice* vi = par.second;

        if (vi->getId() == folha->getId()) continue;

        Vertice* viOriginal = original.getVertice(vi->getId());

        //percorre arestas de vi do GRAFO ORIGINAL até encontrar a primeira melhor conexao
        for (Aresta& arestaOriginal : viOriginal->getArestas()) {

          Vertice* destino = arestaOriginal.getDestino();

          //queremos conexão com o GRUPO da folha
          if (destino->getGrupo()->getId() != folha->getGrupo()->getId()) continue;

          int custoNovo = arestaOriginal.getPeso();

          //comparação simples de melhoria
          if (custoNovo < custoAtual) {
              agm.addAresta(folha->getId(), vi->getId(), custoNovo);

              melhorou = true;
              encontrouMelhoria = true;

              break;
          }
        }

        if (encontrouMelhoria) {
          break;
        }
      }

      
      if (!encontrouMelhoria) {
        agm.addAresta(folha->getId(), vizinhoAtual->getId(), custoAtual);
      }
    }

    poda(agm);

  } while (melhorou);
}
