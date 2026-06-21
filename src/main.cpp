#include <iostream>
#include <ctime>
#include "Menu.h"

using namespace std;

int main() {

    unsigned seed = time(NULL);
    
    cout << "Seed: " << seed << endl;

    Menu::iniciar();

    return 0;
}