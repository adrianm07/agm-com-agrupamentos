#include <iostream>
#include <ctime>
#include "Menu.h"

using namespace std;

int main(int argc, char* argv[]) {

    unsigned seed = time(NULL);
    
    cout << "Seed: " << seed << endl;

    cout << argv[1] << endl;

    Grafo grafo(false);
    grafo.lerArquivo(argv[1]);

    
    cout << "--- Grafo ---" << endl;
    grafo.imprimirGrafo();

    
    return 0;
}