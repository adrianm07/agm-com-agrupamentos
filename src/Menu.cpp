// Menu.cpp

#include <iostream>
#include <vector>

#include "Menu.h"
#include "GrafoTests.h"

using namespace std;

void Menu::iniciar() {

    int opcao;

    do {
        cout << "\n=====================================\n";
        cout << "        SISTEMA DE GRAFOS\n";
        cout << "=====================================\n";
        cout << "1 - Executar bateria de testes\n";
        cout << "2 - Criar e manipular um grafo\n";
        cout << "0 - Sair\n";
        cout << "=====================================\n";
        cout << "Escolha uma opcao: ";

        cin >> opcao;

        switch (opcao) {
            case 1: {
                GrafoTests testes;

                testes.testAll();

                break;
            }
            case 2: {
                menuInterativo();

                break;
            }
            case 0: {
                cout << "\nEncerrando programa...\n";

                break;
            }
            default: {
                cout << "\nOpcao invalida!\n";
            }
        }
    } while (opcao != 0);
}

void Menu::menuInterativo() {

    bool orientado;

    bool importar = false;


    cout << "\nO grafo sera orientado?\n";
    cout << "0 - Nao\n";
    cout << "1 - Sim\n";
    cout << "Opcao: ";

    cin >> orientado;

    cout << "\nO arquivo sera importado?\n";
    cout << "0 - Nao\n";
    cout << "1 - Sim\n";
    cout << "Opcao: ";

    cin >> importar;

    Grafo grafo(orientado);

    if(importar){
        cout << "\nDigite o caminho do arquivo: ";
        string nomeArquivo;
        cin >> nomeArquivo;
        grafo.lerArquivo(nomeArquivo);
    }

    int opcao;

    do {
        exibirMenu();

        cin >> opcao;

        switch (opcao) {
            case 1: {

                int id;

                cout << "\nDigite o id do vertice: ";
                cin >> id;

                grafo.addVertice(id);

                cout << "Vertice adicionado com sucesso!\n";

                break;
            }
            case 2: {

                int id;

                cout << "\nDigite o id do vertice: ";
                cin >> id;

                grafo.removeVertice(id);

                cout << "Vertice removido com sucesso!\n";

                break;
            }
            case 3: {

                int origem;
                int destino;
                double peso = 1.0;

                cout << "\nOrigem: ";
                cin >> origem;

                cout << "Destino: ";
                cin >> destino;

                cout << "Peso (opcional, digite 1.0 se nao for ponderado): ";
                cin >> peso;

                grafo.addAresta(origem, destino, peso);

                cout << "Aresta adicionada com sucesso!\n";

                break;
            }
            case 4: {

                int origem;
                int destino;

                cout << "\nOrigem: ";
                cin >> origem;

                cout << "Destino: ";
                cin >> destino;

                grafo.removeAresta(origem, destino);

                cout << "Aresta removida com sucesso!\n";

                break;
            }
            case 5: {

                int origem;
                int destino;

                cout << "\nOrigem: ";
                cin >> origem;

                cout << "Destino: ";
                cin >> destino;

                bool existe = grafo.verificarAdjacencia(origem, destino);

                if (existe) {
                    cout << "Existe aresta entre os vertices!\n";
                }
                else {  
                    cout << "Nao existe aresta entre os vertices!\n";
                }

                break;
            }

            case 6: {
                int origem;
                int destino;
                double peso;

                cout << "\nOrigem: ";
                cin >> origem;

                cout << "Destino: ";
                cin >> destino;

                cout << "Novo peso: ";
                cin >> peso;

                grafo.alterarPesoAresta(origem, destino, peso);

                cout << "Peso alterado com sucesso!\n";

                break;
            }
            case 7: {

                int id;

                cout << "\nDigite o vertice: ";
                cin >> id;

                int grau = grafo.getGrau(id);

                cout << "Grau do vertice " << id << ": " << grau << endl;

                break;
            }
            case 8: {

                int id;

                cout << "\nDigite o vertice: ";
                cin >> id;

                vector<int> vizinhos = grafo.listarVizinhos(id);

                cout << "Vizinhos de " << id << ": ";

                if (vizinhos.empty()) {
                    cout << "(nenhum)";
                }

                for (int vizinho : vizinhos) {
                    cout << vizinho << " ";
                }

                cout << endl;

                break;
            }

            case 9: {
                double custo;
                Grafo* agm = grafo.primAGM(&custo);
                cout << "Impressao AGM\n";
                agm->imprimirGrafo();
                delete agm;
                break;
            }

            case 10: {
                grafo.imprimirGrafo();

                break;
            }
            case 0: {
                cout << "\nVoltando ao menu principal...\n";

                break;
            }

            default: {
                cout << "\nOpcao invalida!\n";
            }
        }
    } while (opcao != 0);
}

void Menu::exibirMenu() {

    cout << "\n=====================================\n";
    cout << "           MENU DO GRAFO\n";
    cout << "=====================================\n";
    cout << "1 - Adicionar vertice\n";
    cout << "2 - Remover vertice\n";
    cout << "3 - Adicionar aresta\n";
    cout << "4 - Remover aresta\n";
    cout << "5 - Verificar adjacencia\n";
    cout << "6 - Alterar peso da aresta\n";
    cout << "7 - Obter grau de um vertice\n";
    cout << "8 - Listar vizinhos\n";
    cout << "9 - Algoritmo de Prim-AGM\n";
    cout << "10 - Exibir grafo\n";
    cout << "0 - Voltar\n";
    cout << "=====================================\n";
    cout << "Escolha uma opcao: ";
}