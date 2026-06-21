#ifndef GRUPO_H
#define GRUPO_H

#include <vector>
#include "Vertice.h"

using namespace std;

class Grupo {
public:
  Grupo(int id);
  ~Grupo();

  int getId();

  void addVertice(Vertice* v);

  vector<Vertice*>& getListaVertices();

  int getQuantidadeVertices();

  Vertice* getVertice(int pos);

  bool contemVertice(int id);

private:
  int id;
  vector<Vertice*> vertices;
};


#endif // GRUPO_H