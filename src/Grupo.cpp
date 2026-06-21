#include "Grupo.h"

Grupo::Grupo(int id) {
  this->id = id;
}

Grupo::~Grupo() {
}

int Grupo::getId() {
  return id;
}

void Grupo::addVertice(Vertice* v) {
  vertices.push_back(v);
  v->setGrupoId(id);
}

vector<Vertice*>& Grupo::getListaVertices() {
  return vertices;
}

int Grupo::getQuantidadeVertices() {
  return vertices.size();
}

Vertice* Grupo::getVertice(int pos) {
  if (pos < 0 || pos >= vertices.size()) {
      return nullptr;
  }

  return vertices[pos];
}

bool Grupo::contemVertice(int id) {
  for (Vertice* v : vertices) {
      if (v->getId() == id) {
          return true;
      }
  }

  return false;
}