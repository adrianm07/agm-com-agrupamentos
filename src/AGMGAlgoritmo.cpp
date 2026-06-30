#include "AGMGAlgoritmo.h"
#include "Vertice.h"
#include <iostream>
#include <algorithm>
#include <limits>
#include <queue>


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

// Guloso Randomizado: repete 'iteracoes' vezes com alpha fixo,
// retorna a solução de menor custo encontrada
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

// Guloso Reativo: ajusta dinamicamente as probabilidades de cada alpha
// com base na qualidade relativa das soluções obtidas por cada um.
// A cada bloco de iterações (frequência de atualização = max(1, iteracoes/10)),
// recalcula P(alpha_i) ∝ melhorGlobal / mediaCusto(alpha_i)
// alphas que produzem soluções mais próximas do melhor global ganham
// maior probabilidade de ser escolhidos nas próximas iterações.
Grafo* AGMGAlgoritmo::gulosoReativo(Grafo& g, const vector<double>& alphas, int iteracoes) {
    int n = (int)alphas.size();
    if(n == 0) throw invalid_argument("Lista de alphas vazia");

    // probabilidade uniforme inicial
    vector<double> prob(n, 1.0 / n);
    // acumuladores por alpha: soma dos custos e contagem de soluções
    vector<double> somaCustos(n, 0.0);
    vector<int>    contagem(n, 0);

    Grafo* melhor = nullptr;
    double melhorCusto = std::numeric_limits<double>::max();

    // frequência de atualização das probabilidades
    int freqAtualizacao = max(1, iteracoes / 10);

    uniform_real_distribution<double> dist01(0.0, 1.0);

    for(int it = 0; it < iteracoes; it++) {

        // seleciona alpha pela roleta (distribuição acumulada)
        double r = dist01(rng);
        double acum = 0.0;
        int idx = n - 1;
        for(int k = 0; k < n; k++) {
            acum += prob[k];
            if(r <= acum) { idx = k; break; }
        }

        Grafo* candidato = heuristica(g, alphas[idx]);
        double custo = candidato->getCusto();

        // atualiza acumuladores do alpha escolhido
        somaCustos[idx] += custo;
        contagem[idx]++;

        // atualiza melhor solução global
        if(custo < melhorCusto) {
            delete melhor;
            melhor = candidato;
            melhorCusto = custo;
        } else {
            delete candidato;
        }

        // atualiza probabilidades periodicamente
        if((it + 1) % freqAtualizacao == 0 && melhorCusto > 0.0) {
            double somaQ = 0.0;
            vector<double> q(n);

            for(int k = 0; k < n; k++) {
                if(contagem[k] > 0) {
                    // q[k] ∝ melhorGlobal / mediaCusto(k) -> alphas melhores têm q maior
                    q[k] = melhorCusto / (somaCustos[k] / contagem[k]);
                } else {
                    q[k] = 1.0; // alpha ainda não usado: mantém peso neutro
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



// Prim Modificado
// Condição de parada: todos os grupos do grafo original cobertos
// alpha = 0.0 -> guloso puro (LRC = só a aresta mínima, vértice inicial fixo)
// 0 < alpha < 1 -> guloso randomizado (LRC restrita, vértice inicial aleatório)
Grafo* AGMGAlgoritmo::primModificado(Grafo& g, double alpha) {
    if(alpha < 0.0 || alpha > 1.0) {
        throw invalid_argument("Alpha deve estar entre 0 e 1");
    }

    // reseta flags de visita
    for(auto& par : g.getVertices()) {
        par.second->setVisitado(false);
    }

    // cria AGM vazia e copia os grupos do grafo original
    Grafo* agm = new Grafo(false);
    for(Grupo* grupo : g.getGrupos()) {
        agm->addGrupo(grupo->getId());
    }

    int totalGrupos = g.getNumGrupos();

    vector<ArestaAux> candidatos;

    // vértice inicial: sempre o primeiro do mapa
    const auto& vertices = g.getVertices();
    auto it = vertices.begin();

    Vertice* vInicial = it->second;
    vInicial->setVisitado(true);

    // addVertice já registra o grupo em agm->gruposPresentes
    Grupo* grupoInicial = g.getGrupo(vInicial->getGrupo() ? vInicial->getGrupo()->getId() : -1);
    agm->addVertice(vInicial->getId(), grupoInicial);

    for(Aresta& aresta : vInicial->getArestas()) {
        candidatos.push_back({vInicial->getId(), aresta.getDestino()->getId(), aresta.getPeso()});
    }

    // ── loop principal ──────────────────────────────────────────────────────
    while(!candidatos.empty() && !agm->todosGruposPresentes()) {

        // calcula menor e maior peso entre os candidatos
        double menorPeso = candidatos[0].peso;
        double maiorPeso = candidatos[0].peso;
        for(const ArestaAux& a : candidatos) {
            if(a.peso < menorPeso) menorPeso = a.peso;
            if(a.peso > maiorPeso) maiorPeso = a.peso;
        }

        // limite da LRC:
        //   alpha=0 -> limite=menorPeso -> só a(s) aresta(s) mínima(s)
        //   alpha=1 -> limite=maiorPeso -> todas as arestas
        double limite = menorPeso + alpha * (maiorPeso - menorPeso);

        // monta a Lista Restrita de Candidatos
        vector<ArestaAux> lrc;
        for(const ArestaAux& a : candidatos) {
            if(a.peso <= limite) {
                lrc.push_back(a);
            }
        }

        // seleciona aresta da LRC
        uniform_int_distribution<int> distLRC(0, (int)lrc.size() - 1);
        ArestaAux arestaSelecionada = lrc[distLRC(rng)];

        int origem  = arestaSelecionada.origem;
        int destino = arestaSelecionada.destino;
        double peso = arestaSelecionada.peso;

        Vertice* vDestino = g.getVertice(destino);

        // remove aresta da lista de candidatos (válida ou não)
        candidatos.erase(
            remove_if(candidatos.begin(), candidatos.end(),
                [&origem, &destino](const ArestaAux& a) {
                    return a.origem == origem && a.destino == destino;
                }),
            candidatos.end()
        );

        if(vDestino->getVisitado()) {
            continue; // vértice já na árvore, descarta
        }

        vDestino->setVisitado(true);

        // resolve o Grupo* dentro do AGM (grupos foram copiados por addGrupo)
        Vertice* vOrigem  = g.getVertice(origem);
        Grupo* gOrigem  = vOrigem->getGrupo()  ? agm->getGrupo(vOrigem->getGrupo()->getId())  : nullptr;
        Grupo* gDestino = vDestino->getGrupo() ? agm->getGrupo(vDestino->getGrupo()->getId()) : nullptr;

        // addVertice registra o grupo em agm->gruposPresentes automaticamente
        agm->addVertice(origem,  gOrigem);
        agm->addVertice(destino, gDestino);
        agm->addAresta(origem, destino, peso);

        // expande candidatos com arestas do novo vértice
        for(Aresta& aresta : vDestino->getArestas()) {
            if(!aresta.getDestino()->getVisitado()) {
                candidatos.push_back({vDestino->getId(), aresta.getDestino()->getId(), aresta.getPeso()});
            }
        }
    }

    return agm;
}

// Poda -> Remove iterativamente folhas cujo grupo já tem outro representante na árvore
void AGMGAlgoritmo::poda(Grafo& agm) {
    bool removeu;

    do {
        removeu = false;

        vector<int> verticesParaRemover;

        for(auto& par : agm.getVertices()) {
            Vertice* v = par.second;

            // só analisamos folhas
            if(v->getGrau() != 1) {
                continue;
            }

            Grupo* grupo = v->getGrupo();

            // só removemos vértices cujo grupo tem mais de 1 representante
            if(grupo == nullptr || grupo->getQuantidadeVertices() == 1) {
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