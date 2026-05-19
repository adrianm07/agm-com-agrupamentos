#include <iostream>
#include <cassert>
#include <algorithm>

#include "GrafoTests.h"
#include "Grafo.h"
#include "Vertice.h"

using namespace std;

void GrafoTests::testAll() {
    testAdicionarVertice();
    testRemoverVertice();

    testAdicionarAresta();
    testAdicionarArestaOrientada();
    testRemoverAresta();

    testVerificarExistenciaAresta();
    testVerificarAdjacencia();
    testAlterarPesoAresta();
    
    testGetGrau();
    testListarVizinhos();
    
    testImprimirGrafo();

    testPrimAGM();
}

void GrafoTests::testAdicionarVertice() {
    Grafo grafo(false, false);

    cout << "\n------------------------------\n";
    cout << "TESTE: ADICIONAR VERTICE\n";
    cout << "ESTADO inicial: grafo vazio\n";
    cout << "ACAO: inserir vertices 1, 2 e 3\n";
    cout << "ESPERADO: todos os vertices existem com grau 0\n";
    cout << "------------------------------\n";

    grafo.addVertice(1);
    grafo.addVertice(2);
    grafo.addVertice(3);

    //Verifica se os vértices foram adicionados corretamente
    //Vertices sem aresta devem ter grau 0
    assert(grafo.getGrau(1) == 0);
    assert(grafo.getGrau(2) == 0);
    assert(grafo.getGrau(3) == 0);

    grafo.imprimirGrafo();

    cout << "OK Vertices adicionados com sucesso!" << endl;
    cout << "----------------------" << endl;
}

void GrafoTests::testRemoverVertice() {
    Grafo grafo(false, false);

    cout << "\n------------------------------\n";
    cout << "TESTE: REMOVER VERTICE\n";
    cout << "ESTADO inicial: vertices 1, 2, 3 com arestas (1-2) e (2-3)\n";
    cout << "ACAO: remover vertice 2\n";
    cout << "ESPERADO: vertice 2 removido e todas as arestas envolvendo 2 removidas\n";
    cout << "------------------------------\n";

    grafo.addVertice(1);
    grafo.addVertice(2);
    grafo.addVertice(3);

    grafo.addAresta(1, 2);
    grafo.addAresta(2, 3);

    grafo.removeVertice(2);

    //Vértice 2 deve ser removido, grau -1 indica que não existe
    assert(grafo.getGrau(2) == -1);

    // Verifica que as arestas envolvendo 2 desapareceram
    assert(grafo.verificarAdjacencia(1, 2) == false);
    assert(grafo.verificarAdjacencia(2, 1) == false);
    assert(grafo.verificarAdjacencia(2, 3) == false);
    assert(grafo.verificarAdjacencia(3, 2) == false);

    //Verifica se o vértice foi removido corretamente
    //Vertices sem aresta devem ter grau 0
    assert(grafo.getGrau(1) == 0);
    assert(grafo.getGrau(3) == 0);

    grafo.imprimirGrafo();

    cout << "OK Vertice removido com sucesso!" << endl;
    cout << "----------------------" << endl;
}

void GrafoTests::testAdicionarAresta() {
    Grafo grafo(false, false);

    cout << "\n------------------------------\n";
    cout << "TESTE: ADICIONAR ARESTA\n";
    cout << "ESTADO inicial: grafo vazio\n";
    cout << "ACAO: inserir arestas (1-4), (1-3) e (2-3)\n";
    cout << "OBSERVACAO: vertices sao criados automaticamente ao adicionar arestas entre vertices nao existentes\n";
    cout << "ESPERADO: todas as arestas existem\n";
    cout << "------------------------------\n";

    grafo.addAresta(1, 4);
    grafo.addAresta(1, 3);
    grafo.addAresta(2, 3);

    //Verifica se as arestas foram adicionadas corretamente
    assert(grafo.verificarAdjacencia(1, 4) == true);
    assert(grafo.verificarAdjacencia(1, 3) == true);
    assert(grafo.verificarAdjacencia(2, 3) == true);
    assert(grafo.getGrau(1) == 2); // Vértice 1 tem arestas para 4 e 3

    //Como o grafo é não orientado, as conexões devem existir nos dois sentidos
    assert(grafo.verificarAdjacencia(4, 1) == true);
    assert(grafo.verificarAdjacencia(3, 1) == true);
    assert(grafo.verificarAdjacencia(3, 2) == true);

    grafo.imprimirGrafo();

    cout << "OK Arestas adicionadas com sucesso!" << endl;
    cout << "----------------------" << endl;
}

void GrafoTests::testAdicionarArestaOrientada() {
    Grafo grafo(true, false); //Grafo orientado

    cout << "\n------------------------------\n";
    cout << "TESTE: ADICIONAR ARESTA ORIENTADA\n";
    cout << "ESTADO inicial: grafo vazio (orientado)\n";
    cout << "ACAO: inserir aresta 1 -> 2\n";
    cout << "ESPERADO: existe 1 -> 2, mas NAO existe 2 -> 1\n";
    cout << "------------------------------\n";

    grafo.addAresta(1, 2);

    //Verifica se a aresta foi adicionada corretamente de 1 para 2
    assert(grafo.verificarAdjacencia(1, 2) == true);

    //No deve existir conexão de 2 para 1 em um grafo orientado
    assert(grafo.verificarAdjacencia(2, 1) == false);

    grafo.imprimirGrafo();

    cout << "OK Grafo orientado funcionando corretamente!" << endl;
    cout << "----------------------" << endl;
}

void GrafoTests::testRemoverAresta() {
    Grafo grafo(false, false);

    cout << "\n------------------------------\n";
    cout << "TESTE: REMOVER ARESTA\n";
    cout << "ESTADO inicial: arestas (1-4), (1-3), (2-3)\n";
    cout << "ACAO: remover aresta (1-4)\n";
    cout << "ESPERADO: (1-4) removida, outras permanecem intactas\n";
    cout << "------------------------------\n";

    grafo.addAresta(1, 4);
    grafo.addAresta(1, 3);
    grafo.addAresta(2, 3);

    grafo.removeAresta(1, 4);

    //Verifica se a aresta foi removida corretamente
    assert(grafo.verificarAdjacencia(1, 4) == false);
    assert(grafo.verificarAdjacencia(4, 1) == false); // Grafo não orientado

    //As outras conexões devem permanecer intactas
    assert(grafo.verificarAdjacencia(1, 3) == true);
    assert(grafo.verificarAdjacencia(3, 1) == true);
    assert(grafo.verificarAdjacencia(2, 3) == true);
    assert(grafo.verificarAdjacencia(3, 2) == true);

    grafo.imprimirGrafo();

    cout << "OK Aresta removida com sucesso!" << endl;
    cout << "----------------------" << endl;
}

void GrafoTests::testRemoverArestaOrientada() {
    Grafo grafo(true, false); // Grafo orientado

    cout << "\n------------------------------\n";
    cout << "TESTE: REMOVER ARESTA ORIENTADA\n";
    cout << "ESTADO inicial: arestas (1->2), (2->1) e (2->3)\n";
    cout << "ACAO: remover aresta (1->2)\n";
    cout << "ESPERADO: (1->2) removida, (2->1) permanece e (2->3) continua intacta\n";
    cout << "------------------------------\n";

    grafo.addAresta(1, 2);
    grafo.addAresta(2, 1);
    grafo.addAresta(2, 3);

    grafo.removeAresta(1, 2);

    //Aresta (1 -> 2) deve ser removida
    assert(grafo.verificarAdjacencia(1, 2) == false);

    //Aresta (2 -> 1) deve permanecer, pois o grafo é orientado
    assert(grafo.verificarAdjacencia(2, 1) == true);

    //Outra aresta deve continuar intacta
    assert(grafo.verificarAdjacencia(2, 3) == true);
    assert(grafo.verificarAdjacencia(3, 2) == false);

    grafo.imprimirGrafo();

    cout << "OK - remocao respeita direcao do grafo\n";
    cout << "------------------------------\n";
}

void GrafoTests::testVerificarExistenciaAresta() {
    Grafo grafo(false, false);

    cout << "\n------------------------------\n";
    cout << "TESTE: VERIFICAR EXISTENCIA DE ARESTA\n";
    cout << "ESTADO inicial: arestas (1-4), (1-3), (2-3)\n";
    cout << "ACAO: verificar existencia de algumas arestas\n";
    cout << "ESPERADO: (1-4), (1-3), (2-3) existem; (1-2) nao existe\n";
    cout << "------------------------------\n";

    grafo.addAresta(1, 4);
    grafo.addAresta(1, 3);
    grafo.addAresta(2, 3);

    //Verifica a existência das arestas
    assert(grafo.verificarAdjacencia(1, 4) == true);
    assert(grafo.verificarAdjacencia(1, 3) == true);
    assert(grafo.verificarAdjacencia(2, 3) == true);

    //Verifica a inexistência de uma aresta
    assert(grafo.verificarAdjacencia(1, 2) == false);

    grafo.imprimirGrafo();

    cout << "OK Verificacao de existencia de aresta realizada com sucesso!" << endl;
    cout << "----------------------" << endl;
}

void GrafoTests::testAlterarPesoAresta() {
    Grafo grafo(false, true); // Grafo ponderado

    cout << "\n------------------------------\n";
    cout << "TESTE: ALTERAR PESO DE ARESTA\n";
    cout << "ESTADO inicial: grafo ponderado com arestas (1-4=2.5), (1-3=1.0), (2-3=3.0)\n";
    cout << "ACAO: alterar peso da aresta (1-4) para 4.0\n";
    cout << "ESPERADO: peso da aresta (1-4) == 4.0\n";
    cout << "------------------------------\n";

    grafo.addAresta(1, 4, 2.5);
    grafo.addAresta(1, 3, 1.0);
    grafo.addAresta(2, 3, 3.0);

    // Altera o peso da aresta (1, 4)
    grafo.alterarPesoAresta(1, 4, 4.0);

    // Verifica se o peso foi alterado corretamente
    vector<Aresta> arestas = grafo.listarArestas(1);

    bool encontrouAresta = false;
    double pesoAlterado = -1;

    for (auto& aresta : arestas) {
        if (aresta.getDestino()->getId() == 4) {
            pesoAlterado = aresta.getPeso();
            encontrouAresta = true;
            break;
        }
    }

    assert(encontrouAresta == true);
    assert(pesoAlterado == 4.0);

    grafo.imprimirGrafo();

    cout << "OK Peso da aresta alterado com sucesso!" << endl;
    cout << "----------------------" << endl;
}

void GrafoTests::testAlterarPesoArestaOrientada() {
    Grafo grafo(true, true); // orientado e ponderado

    cout << "\n------------------------------\n";
    cout << "TESTE: ALTERAR PESO ARESTA ORIENTADA\n";
    cout << "ESTADO inicial: (1 -> 2)=2.0 e (2 -> 1)=3.0\n";
    cout << "ACAO: alterar (1 -> 2) para 5.0\n";
    cout << "ESPERADO: apenas (1 -> 2) alterado, (2 -> 1) permanece 3.0\n";
    cout << "------------------------------\n";

    grafo.addAresta(1, 2, 2.0);
    grafo.addAresta(2, 1, 3.0); // aresta inversa independente

    grafo.alterarPesoAresta(1, 2, 5.0);

    //Verifica se o peso da aresta (1 -> 2) foi alterado corretamente
    vector<Aresta> arestas1 = grafo.listarArestas(1);

    bool encontrouAresta12 = false;
    double pesoAresta12 = -1;

    for (auto& a : arestas1) {
        if (a.getDestino()->getId() == 2) {
            encontrouAresta12 = true;
            pesoAresta12 = a.getPeso();
            break;
        }
    }

    assert(encontrouAresta12 == true);
    assert(pesoAresta12 == 5.0);

    //Verifica se o peso da aresta inversa (2 -> 1) permanece inalterado
    vector<Aresta> arestas2 = grafo.listarArestas(2);

    bool encontrouAresta21 = false;
    double pesoAresta21 = -1;

    for (auto& a : arestas2) {
        if (a.getDestino()->getId() == 1) {
            encontrouAresta21 = true;
            pesoAresta21 = a.getPeso();
            break;
        }
    }

    assert(encontrouAresta21 == true);
    assert(pesoAresta21 == 3.0); //O peso da aresta inversa deve permanecer inalterado

    grafo.imprimirGrafo();

    cout << "OK Peso alterado apenas na direcao correta!" << endl;
    cout << "----------------------" << endl;
}

void GrafoTests::testImprimirGrafo() {
    Grafo grafo(false, false);

    cout << "\n------------------------------\n";
    cout << "TESTE: IMPRIMIR GRAFO\n";
    cout << "ESTADO inicial: grafo vazio\n";
    cout << "ACAO: inserir arestas (1-4), (1-3), (2-3)\n";
    cout << "ESPERADO: grafo com conexoes corretas exibido na impressao\n";
    cout << "------------------------------\n";

    grafo.addAresta(1, 4);
    grafo.addAresta(1, 3);
    grafo.addAresta(2, 3);

    //Validação estrutural do grafo para garantir que a impressão reflete a estrutura correta
    assert(grafo.verificarAdjacencia(1, 4) == true);
    assert(grafo.verificarAdjacencia(1, 3) == true);
    assert(grafo.verificarAdjacencia(2, 3) == true);

    assert(grafo.getGrau(1) == 2);
    assert(grafo.getGrau(2) == 1);
    assert(grafo.getGrau(3) == 2);
    assert(grafo.getGrau(4) == 1);

    grafo.imprimirGrafo();

    cout << "OK Estrutura do grafo validada com sucesso!" << endl;
    cout << "----------------------" << endl;
}

void GrafoTests::testGetGrau() {
    Grafo grafo(false, false);

    cout << "\n------------------------------\n";
    cout << "TESTE: GET GRAU\n";
    cout << "ESTADO inicial: grafo vazio\n";
    cout << "ACAO: inserir arestas (1-4), (1-3), (2-3)\n";
    cout << "ESPERADO: graus -> vertice 1 = 2, vertice 2 = 1, vertice 3 = 2, vertice 4 = 1\n";
    cout << "------------------------------\n";

    grafo.addAresta(1, 4);
    grafo.addAresta(1, 3);
    grafo.addAresta(2, 3);

    //Verifica o grau dos vértices
    assert(grafo.getGrau(1) == 2); // Vértice 1 tem arestas para 4 e 3
    assert(grafo.getGrau(2) == 1); // Vértice 2 tem aresta para 3
    assert(grafo.getGrau(3) == 2); // Vértice 3 tem arestas para 1 e 2
    assert(grafo.getGrau(4) == 1); // Vértice 4 tem aresta para 1

    grafo.imprimirGrafo();

    cout << "OK Grau dos vertices verificado com sucesso!" << endl;
    cout << "----------------------" << endl;
}

void GrafoTests::testListarVizinhos() {
    Grafo grafo(false, false);
    
    cout << "\n------------------------------\n";
    cout << "TESTE: LISTAR VIZINHOS\n";
    cout << "ESTADO inicial: grafo vazio\n";
    cout << "ACAO: inserir arestas (1-4), (1-3), (2-3)\n";
    cout << "ESPERADO: vizinhos -> vertice 1 = {3, 4}, vertice 2 = {3}, vertice 3 = {1, 2}, vertice 4 = {1}\n";
    cout << "------------------------------\n";

    grafo.addAresta(1, 4);
    grafo.addAresta(1, 3);
    grafo.addAresta(2, 3);

    //Vertice 1 tem conexões para 3 e 4
    vector<int> vizinhos1 = grafo.listarVizinhos(1);
    assert(find(vizinhos1.begin(), vizinhos1.end(), 3) != vizinhos1.end());
    assert(find(vizinhos1.begin(), vizinhos1.end(), 4) != vizinhos1.end());
    assert(vizinhos1.size() == 2); //Vertice 1 tem exatamente 2 vizinhos

    //Vertice 2 tem conexão apenas para 3
    vector<int> vizinhos2 = grafo.listarVizinhos(2);
    assert(find(vizinhos2.begin(), vizinhos2.end(), 3) != vizinhos2.end());
    assert(vizinhos2.size() == 1); //Vertice 2 tem exatamente 1 vizinho

  
    //Vertice 3 tem conexões para 1 e 2, mas não para 4
    vector<int> vizinhos3 = grafo.listarVizinhos(3);
    assert(find(vizinhos3.begin(), vizinhos3.end(), 1) != vizinhos3.end());
    assert(find(vizinhos3.begin(), vizinhos3.end(), 2) != vizinhos3.end());
    assert(find(vizinhos3.begin(), vizinhos3.end(), 4) == vizinhos3.end());
    assert(vizinhos3.size() == 2); //Vertice 3 tem exatamente 2 vizinhos

    //Vertice 4 tem conexão apenas para 1
    vector<int> vizinhos4 = grafo.listarVizinhos(4);
    assert(find(vizinhos4.begin(), vizinhos4.end(), 1) != vizinhos4.end());
    assert(vizinhos4.size() == 1); //Vertice 4 tem exatamente 1 vizinho

    grafo.imprimirGrafo();

    cout << "OK Vizinhos listados com sucesso!" << endl;
    cout << "----------------------" << endl;
}

void GrafoTests::testVerificarAdjacencia() {
    Grafo grafo(false, false);

    cout << "\n------------------------------\n";
    cout << "TESTE: VERIFICAR ADJACENCIA\n";
    cout << "ESTADO inicial: grafo vazio\n";
    cout << "ACAO: inserir arestas (1-4), (1-3), (2-3)\n";
    cout << "ESPERADO: (1-4)=true, (1-3)=true, (2-3)=true, (1-2)=false\n";
    cout << "------------------------------\n";

    grafo.addAresta(1, 4);
    grafo.addAresta(1, 3);
    grafo.addAresta(2, 3);

    //Verifica a adjacência entre os vértices
    assert(grafo.verificarAdjacencia(1, 4) == true);
    assert(grafo.verificarAdjacencia(1, 3) == true);
    assert(grafo.verificarAdjacencia(2, 3) == true);
    assert(grafo.verificarAdjacencia(1, 2) == false); // Não existe aresta entre 1 e 2

    grafo.imprimirGrafo();

    cout << "OK Adjacencia verificada com sucesso!" << endl;
    cout << "----------------------" << endl ;
}

void GrafoTests::testVerificarAdjacenciaOrientada() {
    Grafo grafo(true, false); // grafo orientado

    cout << "\n------------------------------\n";
    cout << "TESTE: VERIFICAR ADJACENCIA ORIENTADA\n";
    cout << "ESTADO inicial: grafo vazio\n";
    cout << "ACAO: inserir aresta (1 -> 2)\n";
    cout << "ESPERADO: (1->2)=true, (2->1)=false\n";
    cout << "------------------------------\n";

    grafo.addAresta(1, 2);

    // Direção correta
    assert(grafo.verificarAdjacencia(1, 2) == true);

    // Direção inversa não deve existir
    assert(grafo.verificarAdjacencia(2, 1) == false);

    grafo.imprimirGrafo();

    cout << "OK Adjacencia em grafo orientado verificada com sucesso!" << endl;
    cout << "----------------------" << endl;
}

void GrafoTests::testPrimAGM() {

    Grafo grafo(false, true); // Grafo ponderado

    cout << "\n------------------------------\n";
    cout << "TESTE: ALGORITMO DE PRIM (AGM)\n";
    cout << "ESTADO inicial: Grafo ponderado com 4 vertices e potencial ciclo\n";
    cout << "ACAO: Executar o algoritmo de Prim\n";
    cout << "ESPERADO: AGM com custo total = 8 e apenas 3 arestas validas\n";
    cout << "------------------------------\n";

    grafo.addAresta(1, 2, 4);
    grafo.addAresta(1, 3, 1);
    grafo.addAresta(2, 3, 2);
    grafo.addAresta(3, 4, 5);

    double custo;
    Grafo* agm = grafo.primAGM(&custo);

    assert(agm != nullptr);
    assert(custo == 8);
    
    agm->imprimirGrafo();

    cout << "OK AGM gerada com sucesso!" << endl;
    cout << "----------------------" << endl;

    delete agm;

}