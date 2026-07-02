#include "AGMGAlgoritmo.h"
#include "Vertice.h"
#include <iostream>
#include <algorithm>
#include <limits>
#include <queue>
#include <vector>

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

// Heurística: primModificado + poda
Grafo* AGMGAlgoritmo::heuristica(Grafo& g, double alpha) {
    Grafo* agm = primModificado(g, alpha);
    poda(*agm);
    return agm;
}

// Guloso puro (alpha = 0): sempre escolhe a aresta de menor peso
Grafo* AGMGAlgoritmo::guloso(Grafo& g) {
    return heuristica(g, 0.0);
}

// Guloso Randomizado
Grafo* AGMGAlgoritmo::gulosoRandomizado(Grafo& g, double alpha, int iteracoes) {
    Grafo* melhor = nullptr;
    double melhorCusto = std::numeric_limits<double>::max();

    for(int i = 0; i < iteracoes; i++) {
        Grafo* candidato = heuristica(g, alpha);
        double custo = candidato->getCusto();

        if(custo < melhorCusto) {
            delete melhor;
            melhor = candidato;
            melhorCusto = custo;
        } else {
            delete candidato;
        }
    }

    return melhor;
}

// Guloso Reativo
Grafo* AGMGAlgoritmo::gulosoReativo(Grafo& g, const vector<double>& alphas, int iteracoes) {
    int n = (int)alphas.size();
    if(n == 0) throw invalid_argument("Lista de alphas vazia");

    vector<double> prob(n, 1.0 / n);
    vector<double> somaCustos(n, 0.0);
    vector<int>    contagem(n, 0);

    Grafo* melhor = nullptr;
    double melhorCusto = std::numeric_limits<double>::max();
    int freqAtualizacao = max(1, iteracoes / 10);

    uniform_real_distribution<double> dist01(0.0, 1.0);

    for(int it = 0; it < iteracoes; it++) {
        double r = dist01(rng);
        double acum = 0.0;
        int idx = n - 1;
        for(int k = 0; k < n; k++) {
            acum += prob[k];
            if(r <= acum) { idx = k; break; }
        }

        Grafo* candidato = heuristica(g, alphas[idx]);
        double custo = candidato->getCusto();

        somaCustos[idx] += custo;
        contagem[idx]++;

        if(custo < melhorCusto) {
            delete melhor;
            melhor = candidato;
            melhorCusto = custo;
        } else {
            delete candidato;
        }

        if((it + 1) % freqAtualizacao == 0 && melhorCusto > 0.0) {
            double somaQ = 0.0;
            vector<double> q(n);

            for(int k = 0; k < n; k++) {
                if(contagem[k] > 0) {
                    q[k] = melhorCusto / (somaCustos[k] / contagem[k]);
                } else {
                    q[k] = 1.0;
                }
                somaQ += q[k];
            }

            for(int k = 0; k < n; k++) {
                prob[k] = q[k] / somaQ;
            }
        }
    }

    return melhor;
}

// Prim Modificado Adaptado para Múltiplos Grupos
Grafo* AGMGAlgoritmo::primModificado(Grafo& g, double alpha) {
    if(alpha < 0.0 || alpha > 1.0) {
        throw invalid_argument("Alpha deve estar entre 0 e 1");
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

    Vertice* vInicial = it->second;
    vInicial->setVisitado(true);

    // --- CORREÇÃO DO VÉRTICE INICIAL ---
    // Adiciona o vértice base na AGM primeiro sem amarras de grupo inicial único
    agm->addVertice(vInicial->getId(), nullptr);
    Vertice* vInicialAGM = agm->getVertice(vInicial->getId());
    
    // Vincula todos os grupos que o vértice original possuía para dentro da AGM
    for (Grupo* grupoOrig : vInicial->getGrupos()) {
        Grupo* grupoAGM = agm->getGrupo(grupoOrig->getId());
        if (grupoAGM != nullptr && vInicialAGM != nullptr) {
            vInicialAGM->addGrupo(grupoAGM);
            grupoAGM->addVertice(vInicialAGM);

        }
    }

    for(Aresta& aresta : vInicial->getArestas()) {
        candidatos.push_back({vInicial->getId(), aresta.getDestino()->getId(), aresta.getPeso()});
    }

    while(!candidatos.empty() && !agm->todosGruposPresentes()) {

        double menorPeso = candidatos[0].peso;
        double maiorPeso = candidatos[0].peso; // caso use weight em sua struct, mantido peso/weight
        for(const ArestaAux& a : candidatos) {
            if(a.peso < menorPeso) menorPeso = a.peso;
            if(a.peso > maiorPeso) maiorPeso = a.peso;
        }

        double limite = menorPeso + alpha * (maiorPeso - menorPeso);

        vector<ArestaAux> lrc;
        for(const ArestaAux& a : candidatos) {
            if(a.peso <= limite) {
                lrc.push_back(a);
            }
        }

        uniform_int_distribution<int> distLRC(0, (int)lrc.size() - 1);
        ArestaAux arestaSelecionada = lrc[distLRC(rng)];

        int origem  = arestaSelecionada.origem;
        int destino = arestaSelecionada.destino;
        double peso = arestaSelecionada.peso;

        Vertice* vDestino = g.getVertice(destino);

        candidatos.erase(
            remove_if(candidatos.begin(), candidatos.end(),
                [&origem, &destino](const ArestaAux& a) {
                    return a.origem == origem && a.destino == destino;
                }),
            candidatos.end()
        );

        if(vDestino->getVisitado()) {
            continue; 
        }

        vDestino->setVisitado(true);

        // --- CORREÇÃO DA EXPANSÃO DOS VÉRTICES ---
        // Cria os nós limpos primeiro
        agm->addVertice(origem,  nullptr);
        agm->addVertice(destino, nullptr);
        agm->addAresta(origem, destino, peso);

        Vertice* vOrigemG   = g.getVertice(origem);
        Vertice* vOrigemAGM = agm->getVertice(origem);
        
        // Mapeia grupos da Origem
        for(Grupo* gr : vOrigemG->getGrupos()) {
            Grupo* grAGM = agm->getGrupo(gr->getId());
            if(grAGM && vOrigemAGM) {
                vOrigemAGM->addGrupo(grAGM);
                grAGM->addVertice(vOrigemAGM);
            }
        }

        // Mapeia grupos do Destino
        for(Grupo* gr : vDestino->getGrupos()) {
            Grupo* grAGM = agm->getGrupo(gr->getId());
            if(grAGM) {
                Vertice* vDestinoAGM = agm->getVertice(destino);
                if(vDestinoAGM) {
                    vDestinoAGM->addGrupo(grAGM);
                    grAGM->addVertice(vDestinoAGM);
                }
            }
        }

        for(Aresta& aresta : vDestino->getArestas()) {
            if(!aresta.getDestino()->getVisitado()) {
                candidatos.push_back({vDestino->getId(), aresta.getDestino()->getId(), aresta.getPeso()});
            }
        }
    }

    return agm;
}

// Poda Adaptada para Múltiplos Grupos
// Regra teórica do problema: Uma folha só pode ser podada se TODOS os grupos que ela representa
// já possuírem pelo menos um OUTRO nó representante ativo na árvore.
void AGMGAlgoritmo::poda(Grafo& agm) {
    bool removeu;

    do {
        removeu = false;
        vector<int> verticesParaRemover;

        for(auto& par : agm.getVertices()) {
            Vertice* v = par.second;

            // Só analisamos folhas da árvore
            if(v->getGrau() != 1) {
                continue;
            }

            // --- CORREÇÃO DA REGRA DE PODA MULTIGRUPO ---
            bool podePodar = true;
            vector<Grupo*> gruposDoVertice = v->getGrupos();

            if(gruposDoVertice.empty()) {
                podePodar = false; 
            }

            for(Grupo* g : gruposDoVertice) {
                // Se algum dos grupos da folha tem apenas 1 nó na árvore toda (ela mesma),
                // ela é essencial para a cobertura desse grupo e NÃO pode ser podada.
                if(g == nullptr || g->getQuantidadeVertices() <= 1) {
                    podePodar = false;
                    break; 
                }
            }

            if(podePodar) {
                verticesParaRemover.push_back(v->getId());
            }
        }

        for(int id : verticesParaRemover) {
            agm.removeVertice(id);
            removeu = true;
        }

    } while(removeu);
}